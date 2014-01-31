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

#ifndef TEAMFOUNDATIONCLIENT_H
#define TEAMFOUNDATIONCLIENT_H

#include <vcsbase/vcsbaseplugin.h>

namespace TeamFoundation {
namespace Internal {

class TeamFoundationPlugin;

struct TeamFoundationResponse
{
    TeamFoundationResponse() : error(false) {}
    bool error;
    QString standardOut;
    QString standardError;
};

class TeamFoundationClient: public QObject
{
    Q_OBJECT

public:
    explicit TeamFoundationClient(TeamFoundationPlugin *plugin);

    bool addFile(const QString &file) const;
    bool deleteFile(const QString &file) const;
    bool moveFile(const QString &from, const QString &to) const;
    bool compare(const QString &file);
    bool undo(const QString &file);
    bool checkIn(const QString &file);
    bool checkoutFile(const QString &fileName) const;
    bool annotateFile(const QString &fileName) const;
    bool getLatest(const QString &fileName) const;
    bool history(const QString &fileName) const;

    bool managesFile(const QString &fileName) const;    
    bool managesDirectory(const QString &directory) const;
    QString repositoryUrl(const QString &fileName) const;

    /// Adds authentication information if it is specified
    static void addAuthentication(QStringList &arguments);
private slots:
    // File level command slots
    void addCurrentFile();
    void deleteCurrentFile();
    void compareCurrentFile();
    void undoCurrentFile();
    void checkInCurrentFile();
    void annotateCurrentFile();
    void getLatestCurrentFile();
    void historyCurrentFile();

    // Project level command slots
    void historyProject();
    void getLatestProject();
    void undoProject();
    void compareProject();
    void checkInProject();

private:
    TeamFoundationResponse runTf(const QString &workingDirectory, const QStringList &arguments,
                                 unsigned flags = VcsBase::VcsBasePlugin::ShowStdOutInLogWindow) const;
    TeamFoundationResponse runTfpt(const QString &workingDirectory, const QStringList &arguments,
                                   unsigned flags = VcsBase::VcsBasePlugin::ShowStdOutInLogWindow) const;
    TeamFoundationResponse runVcs(const QString &workingDirectory,	const QString &executable,
                                  const QStringList &arguments, unsigned flags) const;

    static QString getRepositoryUrl(const QString &workspaceInfo);
    /// Adds /recursive to the paramter list if path is a directory
    void addRecursive(QStringList &arguments, const QString &path) const;

    TeamFoundationPlugin* m_plugin;

    friend class TeamFoundationPlugin;
};

} // namespace Internal
} // namespace TeamFoundation

#endif // TEAMFOUNDATIONCLIENT_H
