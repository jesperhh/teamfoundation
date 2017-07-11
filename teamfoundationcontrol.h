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
#include <QStringList>

namespace TeamFoundation {
namespace Internal {

class TeamFoundationPlugin;

class TeamFoundationControl : public Core::IVersionControl
{
    Q_OBJECT
public:
    explicit TeamFoundationControl(TeamFoundationPlugin *plugin);
    virtual QString displayName() const override;
    virtual Core::Id id() const override;

    virtual bool isVcsFileOrDirectory(const Utils::FileName & fileName) const override;
    virtual bool managesDirectory(const QString &directory, QString *topLevel = 0) const override;
    virtual bool managesFile(const QString &workingDirectory, const QString &fileName) const override;
    virtual bool isConfigured() const  override;
    virtual bool supportsOperation(Operation operation) const  override;
    virtual OpenSupportMode openSupportMode(const QString &fileName) const  override;
    virtual bool vcsOpen(const QString &fileName)  override;
    virtual bool vcsAdd(const QString &fileName) override;
    virtual bool vcsDelete(const QString &filename) override;
    virtual bool vcsMove(const QString &from, const QString &to) override;
    virtual bool vcsCreateRepository(const QString &directory) override;
    bool vcsCheckout(const QString &directory, const QByteArray &url);
    QString vcsGetRepositoryURL(const QString &directory);
    virtual bool vcsAnnotate(const QString &file, int line) override;
    virtual Core::ShellCommand *createInitialCheckoutCommand(const QString &url, const Utils::FileName &baseDirectory, const QString &localName, const QStringList &extraArgs)  override;

    void emitFilesChanged(const QStringList &files);
    void emitRepositoryChanged(const QString &repository);
    void emitConfigurationChanged();

private:
    bool managesDirectoryEx(const QString &directory) const;

private:
    class DirectoryTree
    {
    public:
        void Add(const QString& directory);
        bool IsTopLevelOrSubDirectory(const QString& directory) const;
    private:
        static bool IsParentOrSame(const QString& lhs, const QString& rhs);
        static QString terminate(const QString& directory);
    private:
        QStringList m_entries;
    };

    TeamFoundationPlugin *m_plugin;
    mutable DirectoryTree m_managedDirectories;
};

} // namespace Internal
} // namespace TeamFoundation

#endif // TEAMFOUNDATIONCONTROL_H
