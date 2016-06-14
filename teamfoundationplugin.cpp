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

#include "teamfoundationplugin.h"
#include "teamfoundationconstants.h"

#include "teamfoundationcontrol.h"
#include "teamfoundationclient.h"
#include "settingspage.h"

#include <coreplugin/icore.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/locator/commandlocator.h>

#include <QMenu>
#include <QFileInfo>

#include <utils/qtcassert.h>
#include <utils/parameteraction.h>

using namespace TeamFoundation::Internal;

const char TEAMFOUNDATION_CONTEXT[] = "Team Foundation Context";

TeamFoundationPlugin *TeamFoundationPlugin::m_teamFoundationPluginInstance = 0;

TeamFoundationPlugin::TeamFoundationPlugin()
{
    // Create your members
}

TeamFoundationPlugin::~TeamFoundationPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

QAction* TeamFoundationPlugin::createFileAction(
        Core::CommandLocator* locator,
        const Core::Context &context,
        Core::ActionContainer* container,
        const QString &emptyText,
        const QString &parameterText,
        const char *actionId)
{
    Utils::ParameterAction *action = new Utils::ParameterAction(
                emptyText, parameterText, Utils::ParameterAction::EnabledWithParameter, this);
    Core::Command *command = Core::ActionManager::registerAction(action, actionId, context);
    command->setAttribute(Core::Command::CA_UpdateText);
    container->addAction(command);
    locator->appendCommand(command);
    m_fileActions.append(action);
    return (action);
}

QAction* TeamFoundationPlugin::createProjectAction(
        Core::CommandLocator* locator,
        Core::ActionContainer* container,
        Core::Context context,
        const QString &actionText,
        const char *actionId)
{
    QAction *action = new QAction(actionText, this);
    m_projectActions.append(action);
    Core::Command *command = Core::ActionManager::registerAction(action, actionId, context);
    locator->appendCommand(command);
    container->addAction(command);
    return action;
}

QAction* TeamFoundationPlugin::createGlobalAction(
        Core::CommandLocator* locator,
        Core::ActionContainer* container,
        const QString &actionText,
        const char *actionId)
{
    QAction *action = new QAction(actionText, this);
    Core::Command *command = Core::ActionManager::registerAction(action, actionId);
    locator->appendCommand(command);
    container->addAction(command);
    return action;
}

void TeamFoundationPlugin::createMenus(const Core::Context &context)
{
    Core::ActionContainer* container = Core::ActionManager::createMenu(Core::Id(Constants::MENU_ID));

    const QString prefix = QLatin1String("tfs");
    Core::CommandLocator* locator = new Core::CommandLocator("Team Foundation", prefix, prefix);
    addAutoReleasedObject(locator);

    Core::ActionContainer *currentFileMenu = Core::ActionManager::createMenu("TeamFoundation.CurrentFileMenu");
    currentFileMenu->menu()->setTitle(tr("Current &File"));
    container->addMenu(currentFileMenu);

    QAction* action;
    action = createFileAction(locator, context, currentFileMenu, tr("Add current file"), tr("Add %1"), "TeamFoundation.Add");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(addCurrentFile()));

    action = createFileAction(locator, context, currentFileMenu, tr("Delete current file"), tr("Delete %1"), "TeamFoundation.Delete");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(deleteCurrentFile()));

    action = createFileAction(locator, context, currentFileMenu, tr("Compare current file"), tr("Compare %1"), "TeamFoundation.Compare");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(compareCurrentFile()));

    action = createFileAction(locator, context, currentFileMenu, tr("Undo current file"), tr("Undo %1"), "TeamFoundation.Undo");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(undoCurrentFile()));

    action = createFileAction(locator, context, currentFileMenu, tr("Check in current file"), tr("Check In %1"), "TeamFoundation.CheckIn");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(checkInCurrentFile()));

    action = createFileAction(locator, context, currentFileMenu, tr("Annotate current file"), tr("Annotate %1"), "TeamFoundation.Annotate");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(annotateCurrentFile()));

    action = createFileAction(locator, context, currentFileMenu, tr("History..."), tr("History for %1"), "TeamFoundation.History");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(historyCurrentFile()));

    action = createFileAction(locator, context, currentFileMenu, tr("Get latest..."), tr("Get latest %1"), "TeamFoundation.GetLatest");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(getLatestCurrentFile()));

    action = createFileAction(locator, context, currentFileMenu, tr("Force get latest..."), tr("Force get latest %1"), "TeamFoundation.ForceGetLatest");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(forceGetLatestCurrentFile()));

    action = createFileAction(locator, context, currentFileMenu, tr("Shelve..."), tr("Shelve %1"), "TeamFoundation.Shelve");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(shelveCurrentFile()));

    Core::ActionContainer *currentProjectMenu = Core::ActionManager::createMenu("TeamFoundation.CurrentProjectMenu");
    currentProjectMenu->menu()->setTitle(tr("Current &Project"));
    container->addMenu(currentProjectMenu);

    action = createProjectAction(locator, currentProjectMenu, context, tr("History (Project)"), "TeamFoundation.HistoryProject");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(historyProject()));

    action = createProjectAction(locator, currentProjectMenu, context, tr("Get latest (Project)"), "TeamFoundation.GetLatestProject");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(getLatestProject()));

    action = createProjectAction(locator, currentProjectMenu, context, tr("Force get latest (Project)"), "TeamFoundation.ForceGetLatestProject");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(forceGetLatestProject()));

    action = createProjectAction(locator, currentProjectMenu, context, tr("Undo (Project)"), "TeamFoundation.UndoProject");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(undoProject()));

    action = createProjectAction(locator, currentProjectMenu, context, tr("Compare (Project)"), "TeamFoundation.CompareProject");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(compareProject()));

    action = createProjectAction(locator, currentProjectMenu, context, tr("Check in (Project)"), "TeamFoundation.CheckInProject");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(checkInProject()));

    action = createProjectAction(locator, currentProjectMenu, context, tr("Undo unchanged (Project)"), "TeamFoundation.UndoUnchangedProject");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(revertUnchangedProject()));

    action = createProjectAction(locator, currentProjectMenu, context, tr("Shelve (Project)"), "TeamFoundation.ShelveProject");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(shelveProject()));

    Core::ActionContainer *respositoryMenu = Core::ActionManager::createMenu("TeamFoundation.RepositoryMenu");
    respositoryMenu->menu()->setTitle(tr("&Repository"));
    container->addMenu(respositoryMenu);


    action = createGlobalAction(locator, respositoryMenu, tr("Shelve (Repository)"), "TeamFoundation.ShelveRepository");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(shelveRepository()));

    action = createGlobalAction(locator, respositoryMenu, tr("Unshelve (Repository)"), "TeamFoundation.UnshelveRepository");
    connect(action, SIGNAL(triggered()), m_teamFoundationClient, SLOT(unshelveRepository()));

    Core::ActionContainer *toolsMenu = Core::ActionManager::actionContainer(Core::Id(Core::Constants::M_TOOLS));
    toolsMenu->addMenu(container);

    QMenu *menu = container->menu();
    menu->setTitle(tr("Team Foundation"));
    m_menuAction = menu->menuAction();
}

bool TeamFoundationPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    Core::Context context(TEAMFOUNDATION_CONTEXT);

    initializeVcs(new TeamFoundationControl(this), context);

    m_teamFoundationPluginInstance = this;

    m_settings.readSettings(Core::ICore::settings());

    m_teamFoundationClient = new TeamFoundationClient(this);
    addAutoReleasedObject(new SettingsPage);

    createMenus(context);

    return true;
}

TeamFoundationPlugin *TeamFoundationPlugin::instance()
{
    QTC_ASSERT(m_teamFoundationPluginInstance, return m_teamFoundationPluginInstance);
    return m_teamFoundationPluginInstance;
}

void TeamFoundationPlugin::emitChangedSignal(const QString &path) const
{
    TeamFoundationControl* control = static_cast<TeamFoundationControl*>(versionControl());

    QFileInfo info(path);
    if (info.isFile())
    {
        control->emitFilesChanged(QStringList(path));
    }
    else if (info.isDir())
    {
        control->emitRepositoryChanged(path);
    }
    else if (path.isEmpty())
    {
        control->emitConfigurationChanged();
    }
}

TeamFoundationSettings TeamFoundationPlugin::settings() const
{
    return this->m_settings;
}

void TeamFoundationPlugin::setSettings(const TeamFoundationSettings &s)
{
    if (s != m_settings) {
        m_settings = s;
        m_settings.writeSettings(Core::ICore::settings());
        emitChangedSignal(QString());
    }
}

const TeamFoundationClient *TeamFoundationPlugin::client() const
{
    return this->m_teamFoundationClient;
}

bool TeamFoundationPlugin::submitEditorAboutToClose()
{
    return true;
}

void TeamFoundationPlugin::updateActions(VcsBase::VcsBasePlugin::ActionState as)
{
    foreach (QAction *projectAction, m_projectActions)
        projectAction->setEnabled(as == VcsBase::VcsBasePlugin::VcsEnabled);

    const QString fileName = currentState().currentFileName();
    foreach (Utils::ParameterAction *fileAction, m_fileActions) {
        fileAction->setEnabled(as == VcsBase::VcsBasePlugin::VcsEnabled);
        fileAction->setParameter(fileName);
    }
}

#ifdef WITH_TESTS
#include <QTest>
void TeamFoundationPlugin::testGetRepositoryUrl_data()
{
    QTest::addColumn<QString>("info_output");
    QTest::addColumn<QString>("result");

    QTest::newRow("Success 1")
        << QString(QLatin1String(
        "Local information:\n"
        "  Local path :C:\\Code\\PluginTest\n"
        "  Server path:$/PluginTest\n"
        "  Changeset  :2\n"
        "  Change     :none\n"
        "  Type       :folder\n"
        "Server information:\n"
        "  Server path  : $/PluginTest\n"
        "  Changeset    : 2\n"
        "  Deletion ID  : 0\n"
        "  Lock         : none\n"
        "  Lock owner   : \n"
        "  Last modified: 16. januar 2014 23:41:47\n"
        "  Type         : folder\n"))
        << QString(QLatin1String ("$/PluginTest"));

    QTest::newRow("Success 2")
        << QString(QLatin1String(
        "Local information:\n"
        "  Local path  : C:\\Code\\PluginTest\n"
        "  Server path : $/Plugin Test\n"
        "  Changeset   : 2\n"
        "  Change      : none\n"
        "  Type        : folder\n"
        "Server information:\n"
        "  Server path  : $/PluginTest\n"
        "  Changeset    : 2\n"
        "  Deletion ID  : 0\n"
        "  Lock         : none\n"
        "  Lock owner   : \n"
        "  Last modified: 16. januar 2014 23:41:47\n"
        "  Type         : folder\n"))
        << QString(QLatin1String ("$/PluginTest"));

    QTest::newRow("Failure")
        << QString(QLatin1String("There is no working folder mapping for C:\\Code\\PluginTest\n"))
        << QString(QLatin1String (""));
}

void TeamFoundationPlugin::testGetRepositoryUrl()
{
    QFETCH(QString, info_output);
    QFETCH(QString, result);
    const QString output = TeamFoundationClient::getRepositoryUrl(info_output);
    QCOMPARE(output, result);
}
#endif
