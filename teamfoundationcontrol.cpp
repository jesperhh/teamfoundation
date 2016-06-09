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

#include "teamfoundationcontrol.h"
#include "teamfoundationplugin.h"
#include "teamfoundationclient.h"
#include "teamfoundationconstants.h"

#include <projectexplorer/session.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projecttree.h>

#include <QDir>
#include <utils/synchronousprocess.h>
#include <utils/qtcassert.h>
#include <utils/algorithm.h>

using namespace TeamFoundation;
using namespace TeamFoundation::Internal;

TeamFoundationControl::TeamFoundationControl(TeamFoundationPlugin *plugin) :
    m_plugin(plugin)
{ }

QString TeamFoundationControl::displayName() const
{
    return QLatin1String("Team Foundation");
}

Core::Id TeamFoundationControl::id() const
{
    return Core::Id(TeamFoundation::Constants::VCS_ID_TEAMFOUNDATION);
}

bool TeamFoundationControl::isConfigured() const
{
    const Utils::FileName binary = m_plugin->settings().binaryPath();
    QFileInfo fi = binary.toFileInfo();
    return fi.exists() && fi.isFile() && fi.isExecutable();
}

bool TeamFoundationControl::supportsOperation(Operation operation) const
{
    bool result = isConfigured();
    switch (operation)
    {
    case AddOperation:
    case DeleteOperation:
    case MoveOperation:
    case AnnotateOperation:
    case InitialCheckoutOperation:
        break;
    case CreateRepositoryOperation:
    case SnapshotOperations:
        result = false;
        break;
    }

    return result;
}

Core::IVersionControl::OpenSupportMode TeamFoundationControl::openSupportMode(const QString &fileName) const
{
    Q_UNUSED(fileName);
    return Core::IVersionControl::OpenMandatory;
}

bool TeamFoundationControl::vcsOpen(const QString & fileName)
{
    return m_plugin->client()->checkoutFile(fileName);
}

bool TeamFoundationControl::vcsAdd(const QString &fileName)
{
    return m_plugin->client()->addFile(fileName);
}

bool TeamFoundationControl::vcsDelete(const QString &fileName)
{
    return m_plugin->client()->deleteFile(fileName);
}

bool TeamFoundationControl::vcsMove(const QString &from, const QString &to)
{
    return m_plugin->client()->moveFile(from, to);
}

bool TeamFoundationControl::managesDirectory(const QString &directory, QString *topLevel) const
{
    bool isManaged = managesDirectoryEx(directory);
    if (!isManaged)
        return false;

    QTC_ASSERT(topLevel, return true);
    *topLevel = directory;

    return true;
}

bool TeamFoundationControl::managesDirectoryEx(const QString &directory) const
{
    if (m_managedDirectories.IsTopLevelOrSubDirectory(directory))
        return true;

    if (m_plugin->client()->managesDirectory(directory)) {
        m_managedDirectories.Add(directory);
        return true;
    }

    return false;
}

bool TeamFoundationControl::managesFile(const QString &workingDirectory, const QString &fileName) const
{
    QDir dir(workingDirectory);
    const QString absoluteFileName = dir.absoluteFilePath(fileName);
    return m_plugin->client()->managesFile(absoluteFileName);
}

bool TeamFoundationControl::vcsCheckout(const QString &directory, const QByteArray &url)
{
    Q_UNUSED(directory)
    Q_UNUSED(url);
    return false;
}

QString TeamFoundationControl::vcsGetRepositoryURL(const QString &directory)
{
    return m_plugin->client()->repositoryUrl(directory);
}

bool TeamFoundationControl::vcsCreateRepository(const QString &directory )
{
    Q_UNUSED(directory)
    return false;
}

bool TeamFoundationControl::vcsAnnotate(const QString &file, int line)
{
    Q_UNUSED(line)
    return m_plugin->client()->annotateFile(file);
}

void TeamFoundationControl::emitFilesChanged(const QStringList & files)
{
    emit filesChanged(files);
}

void TeamFoundationControl::emitRepositoryChanged(const QString & repository)
{
    emit repositoryChanged(repository);
}

void TeamFoundationControl::emitConfigurationChanged()
{
    emit configurationChanged();
}


Core::ShellCommand *TeamFoundation::Internal::TeamFoundationControl::createInitialCheckoutCommand(const QString &url, const Utils::FileName &baseDirectory, const QString &localName, const QStringList &extraArgs)
{
    const TeamFoundationSettings settings = TeamFoundationPlugin::instance()->settings();
    const QString workingDirectory = baseDirectory.toString();
    const QString checkoutPath = baseDirectory.toString() + QLatin1Char('/') + localName;

    QString collection, workspace;
    QString workspacePrefix = QLatin1Literal("/workspace:");
    Q_FOREACH(QString var, extraArgs)
    {
        if (var.startsWith(QLatin1Literal("/collection:")))
            collection = var;
        else if (var.startsWith(workspacePrefix))
            workspace = var.remove(0, workspacePrefix.length());
    }

    QStringList mapArgs, getArgs;
    mapArgs << QLatin1String("workfold")
         << QLatin1String("/map")
         << url
         << checkoutPath;

    getArgs << QLatin1String("get") << localName << QLatin1String("/recursive");


    VcsBase::VcsCommand *command = new VcsBase::VcsCommand(workingDirectory, QProcessEnvironment::systemEnvironment());

    if (!collection.isEmpty())
    {
        mapArgs << collection;
    }

    if(!workspace.isEmpty())
    {
        QStringList makeDirArgs;
        makeDirArgs << QLatin1String("/c") << QLatin1String("mkdir") << localName;
        command->addJob(Utils::FileName::fromLatin1("cmd"), makeDirArgs, -1);

        QStringList workspaceArgs;
        workspaceArgs << QLatin1String("workspace")
            << QLatin1String("/new")
            << workspace
            << collection;

        TeamFoundationClient::addAuthentication(workspaceArgs);
        command->addJob(settings.binaryPath(), workspaceArgs, -1, checkoutPath);

        mapArgs << QLatin1Literal("/workspace:") + workspace;
    }

    TeamFoundationClient::addAuthentication(mapArgs);
    TeamFoundationClient::addAuthentication(getArgs);
    command->addJob(settings.binaryPath(), mapArgs, -1);
    command->addJob(settings.binaryPath(), getArgs, -1);
    return command;
}

////////////////////////////////////////////////////////////////////////////////
void TeamFoundationControl::DirectoryTree::Add(const QString& directory)
{
    QString dir = terminate(directory);

    for (int i = 0; i < m_entries.size(); ++i) {
        // candidate shadowed?
        if (IsParentOrSame(m_entries[i], dir))
            return;

        // candidate shadows?
        if (IsParentOrSame(dir, m_entries[i])) {
            m_entries[i] = m_entries[m_entries.size()-1];
            m_entries.pop_back();
            --i;
        }
    }

    m_entries << dir;
}

QString TeamFoundationControl::DirectoryTree::terminate(const QString& directory)
{
    // QDir::separator() returns '\\' on Windows
    // which is not what we want
    if (!directory.endsWith(QLatin1Char('/')))
        return directory + QLatin1Char('/');
    return directory;
}

bool TeamFoundationControl::DirectoryTree::IsParentOrSame(const QString& lsh, const QString& rhs)
{
    return rhs.startsWith(lsh, Qt::CaseInsensitive);
}

bool TeamFoundationControl::DirectoryTree::IsTopLevelOrSubDirectory(const QString& directory) const
{
    QString dir = terminate(directory);
    auto* p = &dir;
    return Utils::contains(m_entries, [p] (const QString& entry) { return IsParentOrSame(entry, *p); });
}


