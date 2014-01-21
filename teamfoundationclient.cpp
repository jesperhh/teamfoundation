#include "teamfoundationclient.h"

#include "teamfoundationplugin.h"

#include <coreplugin/iversioncontrol.h>
#include <utils/synchronousprocess.h>
#include <QFileInfo>

#define RUN_PREAMBLE_1(file) \
    const QFileInfo file##Info(file); \
    QStringList parameters;

#define RUN_PREAMBLE_2(file1, file2) \
    const QFileInfo file1##Info(file1); \
    const QFileInfo file2##Info(file2); \
    QStringList parameters;

using namespace TeamFoundation::Internal;

TeamFoundationClient::TeamFoundationClient(TeamFoundationPlugin *plugin) : QObject(plugin),
    m_plugin(plugin)
{
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

    const TeamFoundationResponse resp = runTf(fromInfo.absolutePath(), parameters);
    return !resp.error;
}

bool TeamFoundationClient::compareFile(const QString &fileName)
{
    RUN_PREAMBLE_1(fileName)

    parameters << QLatin1String("difference");
    parameters << fileName;
    parameters << QLatin1String("/format:Visual");

    const TeamFoundationResponse resp = runTf(fileNameInfo.absolutePath(), parameters);
    return !resp.error;
}

bool TeamFoundationClient::undoFile(const QString &fileName)
{
    RUN_PREAMBLE_1(fileName)

    parameters << QLatin1String("undo");
    parameters << fileName;

    const TeamFoundationResponse resp = runTf(fileNameInfo.absolutePath(), parameters);
    return !resp.error;
}

bool TeamFoundationClient::checkInFile(const QString &fileName)
{
    RUN_PREAMBLE_1(fileName)

    parameters << QLatin1String("checkin");
    parameters << fileName;

    const TeamFoundationResponse resp = runTf(fileNameInfo.absolutePath(), parameters);
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
    return true;
}

bool TeamFoundationClient::managesFile(const QString &fileName) const
{
    RUN_PREAMBLE_1(fileName)

    parameters << QLatin1String("info");
    parameters << fileName;

    const TeamFoundationResponse resp = runTf(fileNameInfo.absolutePath(), parameters, 0);
    return !resp.error;
    return true;
}

bool TeamFoundationClient::managesDirectory(const QString &directory) const
{
    QStringList parameters;
    parameters << QLatin1String("info");
    parameters << directory;

    const TeamFoundationResponse resp = runTf(directory, parameters , 0);
    return !resp.error;
    return true;
}

void TeamFoundationClient::addCurrentFile()
{
    const QString filename = m_plugin->currentState().currentFile();
    addFile(filename);
}

void TeamFoundationClient::deleteCurrentFile()
{
    const QString filename = m_plugin->currentState().currentFile();
    deleteFile(filename);
}

void TeamFoundationClient::compareCurrentFile()
{
    const QString filename = m_plugin->currentState().currentFile();
    compareFile(filename);
}

void TeamFoundationClient::undoCurrentFile()
{
    const QString filename = m_plugin->currentState().currentFile();
    undoFile(filename);
}

void TeamFoundationClient::checkInCurrentFile()
{
    const QString filename = m_plugin->currentState().currentFile();
    checkInFile(filename);
}

void TeamFoundationClient::annotateCurrentFile()
{
    const QString filename = m_plugin->currentState().currentFile();
    annotateFile(filename);
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
        const QString &executable,
        const QStringList &arguments,
        unsigned flags) const
{
    TeamFoundationResponse response;
    if (!m_plugin->versionControl()->isConfigured())
    {
        response.error = true;
        response.message = tr("Team Foundation is not configured!");
        return response;
    }

    QStringList allArguments(arguments);
    allArguments.append(QLatin1String("/prompt"));

    const Utils::SynchronousProcessResponse runVcsResponse =
            m_plugin->runVcs(workingDirectory,
                             executable,
                             allArguments,
                             m_plugin->settings().timeOut(),
                             flags);

    response.error = runVcsResponse.result != Utils::SynchronousProcessResponse::Finished;
    if (response.error)
        response.message = runVcsResponse.exitMessage(executable, m_plugin->settings().timeOut());

    response.standardError = runVcsResponse.stdErr;
    response.standardOut = runVcsResponse.stdOut;
    return response;
}
