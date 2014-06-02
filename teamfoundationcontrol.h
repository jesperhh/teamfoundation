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

#ifndef TEAMFOUNDATIONCONTROL_H
#define TEAMFOUNDATIONCONTROL_H

#include <coreplugin/iversioncontrol.h>

namespace TeamFoundation {
namespace Internal {

class TeamFoundationPlugin;

class TeamFoundationControl : public Core::IVersionControl
{
    Q_OBJECT
public:
    explicit TeamFoundationControl(TeamFoundationPlugin *plugin);
    QString displayName() const;
    Core::Id id() const;

    bool managesDirectory(const QString &directory, QString *topLevel = 0) const;
    bool managesFile(const QString &workingDirectory, const QString &fileName) const;
    bool isConfigured() const;
    bool supportsOperation(Operation operation) const;
    OpenSupportMode openSupportMode(const QString &fileName) const;
    bool vcsOpen(const QString &fileName);
    bool vcsAdd(const QString &fileName);
    bool vcsDelete(const QString &filename);
    bool vcsMove(const QString &from, const QString &to);
    bool vcsCreateRepository(const QString &directory);
    bool vcsCheckout(const QString &directory, const QByteArray &url);
    QString vcsGetRepositoryURL(const QString &directory);
    bool vcsAnnotate(const QString &file, int line);

    void emitFilesChanged(const QStringList &files);
    void emitRepositoryChanged(const QString &repository);
    void emitConfigurationChanged();


private:
    TeamFoundationPlugin *m_plugin;
};

} // namespace Internal
} // namespace TeamFoundation

#endif // TEAMFOUNDATIONCONTROL_H
