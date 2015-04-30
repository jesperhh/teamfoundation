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

#include <QDir>

using namespace TeamFoundation;
using namespace TeamFoundation::Internal;

TeamFoundationControl::TeamFoundationControl(TeamFoundationPlugin *plugin) :
    m_plugin(plugin)
{
}

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
    bool isManaged = m_plugin->client()->managesDirectory(directory);
    if (!isManaged)
        return false;

    QDir dir(directory);
    while (true)
    {
        *topLevel = dir.absolutePath();
        if (!dir.cdUp() || !m_plugin->client()->managesDirectory(dir.absolutePath()))
            break;
    }

    return true;
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
