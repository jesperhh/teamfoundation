/****************************************************************************
**
** Team Foundation Server plugin for Qt Creator
** Copyright (C) 2014 Jesper Hellesø Hansen
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/

#include "teamfoundationclient.h"
#include "teamfoundationplugin.h"

#include <coreplugin/iversioncontrol.h>
#include <utils/synchronousprocess.h>
#include <utils/qtcassert.h>
#include <QFileInfo>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#define RUN_PREAMBLE_1(file) \
    const QFileInfo file##Info(file); \
    QStringList parameters;

#define RUN_PREAMBLE_2(file1, file2) \
    const QFileInfo file1##Info(file1); \
    const QFileInfo file2##Info(file2); \
    QStringList parameters;

using namespace TeamFoundation::Internal;
using namespace VcsBase;
const int SuppressCompletely =
        VcsCommand::SuppressStdErr |
        VcsCommand::SuppressFailMessage |
        VcsCommand::SuppressCommandLogging;

TeamFoundationClient::TeamFoundationClient(TeamFoundationPlugin *plugin) : QObject(plugin),
    m_plugin(plugin)
{
    connect(plugin->versionControl(), SIGNAL(configurationChanged()), this, SLOT(configurationChanged()));

    configurationChanged();
}

bool TeamFoundationClient::addFile(const QString &fileName) const
{
    RUN_PREAMBLE_1(fileName)

    parameters << QLatin1String("add");
    parameters << fileName;
    const TeamFoundationResponse resp = runTf(fileNameInfo.absolutePath(), parameters);
    return !resp.error;
}

bool TeamFoundationClient::deleteFile(const QString &fileName) const
{
    RUN_PREAMBLE_1(fileName)
    parameters << QLatin1String("delete");
    parameters << fileName;

    const TeamFoundationResponse resp = runTf(fileNameInfo.absolutePath(), parameters);
    return !resp.error;
}

bool TeamFoundationClient::moveFile(const QString &from, const QString &to) const
{
    RUN_PREAMBLE_2(from, to)

    parameters << QLatin1String("rename");
    parameters << fromInfo.absoluteFilePath();
    parameters << toInfo.absoluteFilePath();

    const TeamFoundationResponse resp = runTf(fromInfo.absolutePath(), parameters,
            VcsCommand::ShowStdOut | VcsCommand::FullySynchronously);
    return !resp.error;
}

bool TeamFoundationClient::compare(const QString &path)
{
    RUN_PREAMBLE_1(path)

    parameters << QLatin1String("difference");
    parameters << path;
    parameters << QLatin1String("/format:Visual");
    parameters << QLatin1String("/prompt");

    addRecursive(parameters, path);

    const TeamFoundationResponse resp = runTf(pathInfo.absolutePath(), parameters);
    return !resp.error;
}

bool TeamFoundationClient::undo(const QString &path)
{
    RUN_PREAMBLE_1(path)

    parameters << QLatin1String("undo");
    parameters << path;

    addRecursive(parameters, path);

    const TeamFoundationResponse resp = runTf(pathInfo.absolutePath(), parameters,
        VcsCommand::ShowStdOut | VcsCommand::FullySynchronously);
    if (!resp.error)
    {
        m_plugin->emitChangedSignal(path);
    }
    return !resp.error;
}

bool TeamFoundationClient::checkIn(const QString &path)
{
    RUN_PREAMBLE_1(path)

    parameters << QLatin1String("checkin");
    parameters << path;
    parameters << QLatin1String("/prompt");

    addRecursive(parameters, path);

    const TeamFoundationResponse resp = runTf(pathInfo.absolutePath(), parameters);
    return !resp.error;
}

bool TeamFoundationClient::checkoutFile(const QString &fileName) const
{
    RUN_PREAMBLE_1(fileName)

    parameters << QLatin1String("checkout");
    parameters << fileName;

    const TeamFoundationResponse resp = runTf(fileNameInfo.absolutePath(), parameters);
    return !resp.error;
}

bool TeamFoundationClient::annotateFile(const QString &fileName) const
{
    RUN_PREAMBLE_1(fileName)

    parameters << QLatin1String("annotate");
    parameters << fileName;

    const TeamFoundationResponse resp = runTfpt(fileNameInfo.absolutePath(), parameters);
    return !resp.error;
}

bool TeamFoundationClient::getLatest(const QString &path) const
{
    RUN_PREAMBLE_1(path)

    parameters << QLatin1String("get");
    parameters << path;
    parameters << QLatin1String("/prompt");

    addRecursive(parameters, path);

    const TeamFoundationResponse resp = runTf(pathInfo.absolutePath(), parameters);
    if (!resp.error)
    {
        m_plugin->emitChangedSignal(path);
    }
    return !resp.error;
}

bool TeamFoundationClient::history(const QString &path) const
{
    RUN_PREAMBLE_1(path)

    parameters << QLatin1String("history");
    parameters << path;
    parameters << QLatin1String("/prompt");

    addRecursive(parameters, path);

    const TeamFoundationResponse resp = runTf(pathInfo.absolutePath(), parameters);
    return !resp.error;
}

bool TeamFoundationClient::managesFile(const QString &fileName) const
{
    switch (m_tfVersion) {
    case TfVersion_10:
        return manages(fileName, QStringLiteral("properties"));
    default:
        return manages(fileName, QStringLiteral("info"));
    }
}

bool TeamFoundationClient::managesDirectory(const QString &directory) const
{
    switch (m_tfVersion) {
    case TfVersion_10:
        return manages(directory, QStringLiteral("properties"));
    default:
        return manages(directory, QStringLiteral("info"));
    }
}

bool TeamFoundationClient::manages(const QString &path, const QString& cmd) const
{
    QStringList parameters;
    parameters << cmd;
    parameters << path;

    const TeamFoundationResponse resp = runTf(path, parameters, SuppressCompletely);
    return !resp.error;
}

QString TeamFoundationClient::repositoryUrl(const QString &fileName) const
{
    switch (m_tfVersion) {
    case TfVersion_10:
        return repositoryUrl(fileName, QStringLiteral("properties"));
    default:
        return repositoryUrl(fileName, QStringLiteral("info"));
    }
}

QString TeamFoundationClient::repositoryUrl(const QString &fileName, const QString& command) const
{
    RUN_PREAMBLE_1(fileName)
    parameters << command;
    parameters << fileName;

    const TeamFoundationResponse resp = runTf(fileNameInfo.absolutePath(), parameters, SuppressCompletely);
    if (resp.error || resp.standardOut.isNull() || resp.standardOut.isEmpty())
        return QString();

    return getRepositoryUrl(resp.standardOut);
}

QString TeamFoundationClient::getRepositoryUrl(const QString &workspaceInfo)
{
    QString result;
    QRegExp r(QLatin1String("Server path\\s*:\\s*([^\\s]*)\n"));
    r.setMinimal(true); // lame regexp suppport!
    QTC_ASSERT(r.isValid(), return result);
    if (r.indexIn(workspaceInfo) != -1)
    {
        result = r.cap(1);
    }
    return result;
}

void TeamFoundationClient::addCurrentFile()
{
    QTC_ASSERT(m_plugin->currentState().hasFile(), return);
    const QString filename = m_plugin->currentState().currentFile();
    addFile(filename);
}

void TeamFoundationClient::deleteCurrentFile()
{
    QTC_ASSERT(m_plugin->currentState().hasFile(), return);
    const QString filename = m_plugin->currentState().currentFile();
    deleteFile(filename);
}

void TeamFoundationClient::compareCurrentFile()
{
    QTC_ASSERT(m_plugin->currentState().hasFile(), return);
    const QString filename = m_plugin->currentState().currentFile();
    compare(filename);
}

void TeamFoundationClient::undoCurrentFile()
{
    QTC_ASSERT(m_plugin->currentState().hasFile(), return);
    const QString filename = m_plugin->currentState().currentFile();
    undo(filename);
}

void TeamFoundationClient::checkInCurrentFile()
{
    QTC_ASSERT(m_plugin->currentState().hasFile(), return);
    const QString filename = m_plugin->currentState().currentFile();
    checkIn(filename);
}

void TeamFoundationClient::annotateCurrentFile()
{
    QTC_ASSERT(m_plugin->currentState().hasFile(), return);
    const QString filename = m_plugin->currentState().currentFile();
    annotateFile(filename);
}

void TeamFoundationClient::getLatestCurrentFile()
{
    QTC_ASSERT(m_plugin->currentState().hasFile(), return);
    const QString filename = m_plugin->currentState().currentFile();
    getLatest(filename);
}

void TeamFoundationClient::historyCurrentFile()
{
    QTC_ASSERT(m_plugin->currentState().hasFile(), return);
    const QString filename = m_plugin->currentState().currentFile();
    history(filename);
}

void TeamFoundationClient::historyProject()
{
    QTC_ASSERT(m_plugin->currentState().hasTopLevel(), return);
    const QString topLevel = m_plugin->currentState().topLevel();
    history(topLevel);
}

void TeamFoundationClient::getLatestProject()
{
    QTC_ASSERT(m_plugin->currentState().hasTopLevel(), return);
    const QString topLevel = m_plugin->currentState().topLevel();
    getLatest(topLevel);
}

void TeamFoundationClient::undoProject()
{
    QTC_ASSERT(m_plugin->currentState().hasTopLevel(), return);
    const QString topLevel = m_plugin->currentState().topLevel();
    undo(topLevel);
}

void TeamFoundationClient::compareProject()
{
    QTC_ASSERT(m_plugin->currentState().hasTopLevel(), return);
    const QString topLevel = m_plugin->currentState().topLevel();
    compare(topLevel);
}

void TeamFoundationClient::checkInProject()
{
    QTC_ASSERT(m_plugin->currentState().hasTopLevel(), return);
    const QString topLevel = m_plugin->currentState().topLevel();
    checkIn(topLevel);
}

TeamFoundationResponse TeamFoundationClient::runTf(const QString &workingDirectory,
                                                   const QStringList &arguments,
                                                   unsigned flags) const
{
    return runVcs(workingDirectory, m_plugin->settings().binaryPath(), arguments, flags);
}

TeamFoundationResponse TeamFoundationClient::runTfpt(const QString &workingDirectory,
                                                     const QStringList &arguments,
                                                     unsigned flags) const
{
    return runVcs(workingDirectory, m_plugin->settings().tftpBinaryPath(), arguments, flags);
}

TeamFoundationResponse TeamFoundationClient::runVcs(
        const QString &workingDirectory,
        const Utils::FileName &executable,
        const QStringList &arguments,
        unsigned flags) const
{
    TeamFoundationResponse response;
    if (!m_plugin->versionControl()->isConfigured())
    {
        response.error = true;
        return response;
    }

    QStringList allArguments(arguments);
    addAuthentication(allArguments);

    const Utils::SynchronousProcessResponse runVcsResponse =
            m_plugin->runVcs(workingDirectory,
                             executable,
                             allArguments,
                             m_plugin->settings().timeOut(),
                             flags);

    response.error = runVcsResponse.result != Utils::SynchronousProcessResponse::Finished;
    response.standardError = runVcsResponse.stdErr;
    response.standardOut = runVcsResponse.stdOut;
    return response;
}

void TeamFoundationClient::addAuthentication(QStringList &arguments)
{
    TeamFoundationPlugin* plugin = TeamFoundationPlugin::instance();
    const QString username = plugin->settings().username();
    const QString password = plugin->settings().password();
    if (username.isEmpty() || password.isEmpty())
        return;

    const QString parameter = QString(QLatin1String("/login:%1,%2")).arg(username, password);
    arguments.append(parameter);
}

void TeamFoundationClient::addRecursive(QStringList &arguments, const QString &path) const
{
    QFileInfo fileInfo(path);
    if (fileInfo.isDir())
        arguments << QLatin1String("/recursive");
}

void TeamFoundationClient::configurationChanged()
{
    m_tfVersion = TfVersion_None;
    if (!m_plugin->settings().binaryPath().isEmpty()) {
        TeamFoundationResponse response = runTf(QStringLiteral("C:/"), QStringList(), SuppressCompletely);
        QRegularExpression re(QStringLiteral("Version (\\d+)"));
        QRegularExpressionMatch match = re.match(response.standardOut);
        if (match.hasMatch()) {
            QString str = match.captured(1);
            int version = str.toInt();
            switch (version) {
            case 10:
                m_tfVersion = TfVersion_10;
                break;
            default:
                m_tfVersion = TfVersion_BetterThan10;
                break;
            }
        }
    }
}
