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

#ifndef TEAMFOUNDATION_H
#define TEAMFOUNDATION_H

#include "teamfoundation_global.h"
#include "teamfoundationsettings.h"

#include <vcsbase/vcsbaseplugin.h>
#include <coreplugin/icontext.h>

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

namespace Core {
class CommandLocator;
class ActionManager;
class ActionContainer;
class Id;
class Command;
}

namespace Utils {
class ParameterAction;
}

namespace TeamFoundation {
namespace Internal {

class TeamFoundationClient;
class TeamFoundationControl;

class TeamFoundationPlugin : public VcsBase::VcsBasePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "TeamFoundation.json")

public:
    TeamFoundationPlugin();
    ~TeamFoundationPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    TeamFoundationSettings settings() const;
    void setSettings(const TeamFoundationSettings &s);
    const TeamFoundationClient *client() const;
    static TeamFoundationPlugin *instance();

    /// emit changed signal in VersionControl - for files emits emitFilesChanged, for directories emits
    /// emitRepositoryChanged, and for the empty string, emits emitConfigurationChanged.
    void emitChangedSignal(const QString &path) const;
private slots:
#ifdef WITH_TESTS
    void testGetRepositoryUrl_data();
    void testGetRepositoryUrl();
#endif

protected:
    void createMenus(const Core::Context &context);
    QAction *createFileAction(
            Core::CommandLocator* locator,
            const Core::Context &context,
            Core::ActionContainer* container,
            const QString &emptyText,
            const QString &parameterText,
            const char *actionId);
    QAction *createProjectAction(
            Core::CommandLocator* locator,
            Core::ActionContainer* container,
            Core::Context context,
            const QString &actionText,
            const char *actionId);
    QAction *createGlobalAction(
            Core::CommandLocator* locator,
            Core::ActionContainer* container,
            const QString &actionText,
            const char *actionId);
    void updateActions(VcsBase::VcsBasePlugin::ActionState);
    bool submitEditorAboutToClose();

private:
    TeamFoundationSettings m_settings;
    QAction *m_menuAction;
    QList<Utils::ParameterAction*> m_fileActions;
    QList<QAction*> m_projectActions;
    TeamFoundationClient *m_teamFoundationClient;
    static TeamFoundationPlugin *m_teamFoundationPluginInstance;
};

} // namespace Internal
} // namespace TeamFoundation

#endif // TEAMFOUNDATION_H

