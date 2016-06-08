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
#include <vcsbase/vcscommand.h>

namespace TeamFoundation {
namespace Internal {

class TeamFoundationPlugin;

struct TeamFoundationResponse
{
    TeamFoundationResponse() : exitCode(0) {}
    bool error() const { return exitCode != 0; }
    int exitCode;
    QString standardOut;
    QString standardError;
};

class TeamFoundationClient: public QObject
{
    Q_OBJECT

    friend class TeamFoundationPlugin;
public:
    explicit TeamFoundationClient(TeamFoundationPlugin *plugin);

    bool addFile(const QString &file) const;
    bool deleteFile(const QString &file) const;
    bool moveFile(const QString &from, const QString &to) const;
    bool compare(const QString &path);
    bool undo(const QString &path);
    bool checkIn(const QString &path);
    bool checkoutFile(const QString &fileName) const;
    bool annotateFile(const QString &fileName) const;
    bool getLatest(const QString &path, bool force) const;
    bool history(const QString &path) const;

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
    void forceGetLatestCurrentFile();
    void historyCurrentFile();

    // Project level command slots
    void historyProject();
    void getLatestProject();
    void forceGetLatestProject();
    void undoProject();
    void compareProject();
    void checkInProject();

    // settings
    void configurationChanged();

private:
    TeamFoundationResponse runTf(const QString &workingDirectory, const QStringList &arguments,
                                 unsigned flags = VcsBase::VcsCommand::ShowStdOut) const;
    TeamFoundationResponse runTfpt(const QString &workingDirectory, const QStringList &arguments,
                                   unsigned flags = VcsBase::VcsCommand::ShowStdOut) const;
    TeamFoundationResponse runVcs(const QString &workingDirectory,	const Utils::FileName &executable,
                                  const QStringList &arguments, unsigned flags) const;

    bool manages(const QString &directory, const QString& command) const;
    QString repositoryUrl(const QString &fileName, const QString& command) const;

    static QString getRepositoryUrl(const QString &workspaceInfo);
    /// Adds /recursive to the paramter list if path is a directory
    void addRecursive(QStringList &arguments, const QString &path) const;

    TeamFoundationPlugin* m_plugin;

    enum TfVersion
    {
        TfVersion_None,
        TfVersion_10,
        TfVersion_BetterThan10
    };

    TfVersion m_tfVersion = TfVersion_None;

};

} // namespace Internal
} // namespace TeamFoundation

#endif // TEAMFOUNDATIONCLIENT_H
