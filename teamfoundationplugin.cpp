#include "teamfoundationplugin.h"
#include "teamfoundationconstants.h"

#include "teamfoundationcontrol.h"
#include "teamfoundationclient.h"
#include "settingspage.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/coreconstants.h>

#include <locator/commandlocator.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>

#include <utils/qtcassert.h>
#include <utils/parameteraction.h>

#include <QtPlugin>

using namespace TeamFoundation::Internal;

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

Core::Command* TeamFoundationPlugin::createFileCommand(const Core::Context &context, Core::ActionContainer* container,
                                             const QString &emptyText, const QString &parameterText, const char *actionId)
{
    Utils::ParameterAction *action = new Utils::ParameterAction(
                emptyText, parameterText, Utils::ParameterAction::EnabledWithParameter, this);

    Core::Command *command = Core::ActionManager::registerAction(action, Core::Id(actionId), context);
    command->setAttribute(Core::Command::CA_UpdateText);
    container->addAction(command);
    m_commandLocator->appendCommand(command);
    m_fileActionList.append(action);
    return command;
}

void TeamFoundationPlugin::createMenus()
{
    Core::Context context(Core::Constants::C_GLOBAL);
    Core::Command* command;

    // Create menu item for Team Foundation
    Core::ActionContainer* container = Core::ActionManager::createMenu(Core::Id(Constants::MENU_ID));

    const QString prefix = QLatin1String("tfs");
    m_commandLocator = new Locator::CommandLocator("Team Foundation", prefix, prefix);
    addAutoReleasedObject(m_commandLocator);


    command = createFileCommand(context, container, tr("Add current File"), tr("Add \"%1\""), "TeamFoundation.Add");
    connect(command->action(), SIGNAL(triggered()), m_teamFoundationClient, SLOT(addCurrentFile()));

    command = createFileCommand(context, container, tr("Delete current File"), tr("Delete \"%1\""), "TeamFoundation.Delete");
    connect(command->action(), SIGNAL(triggered()), m_teamFoundationClient, SLOT(deleteCurrentFile()));

    command = createFileCommand(context, container, tr("Compare current File"), tr("Compare \"%1\""), "TeamFoundation.Compare");
    connect(command->action(), SIGNAL(triggered()), m_teamFoundationClient, SLOT(compareCurrentFile()));

    command = createFileCommand(context, container, tr("Undo current File"), tr("Undo \"%1\""), "TeamFoundation.Undo");
    connect(command->action(), SIGNAL(triggered()), m_teamFoundationClient, SLOT(undoCurrentFile()));

    command = createFileCommand(context, container, tr("Check In current File"), tr("Check In \"%1\""), "TeamFoundation.CheckIn");
    connect(command->action(), SIGNAL(triggered()), m_teamFoundationClient, SLOT(checkInCurrentFile()));

    command = createFileCommand(context, container, tr("Annotate current File"), tr("Annotate \"%1\""), "TeamFoundation.Annotate");
    connect(command->action(), SIGNAL(triggered()), m_teamFoundationClient, SLOT(annotateCurrentFile()));

    // Request the Tools menu and add the Bazaar menu to it
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

    initializeVcs(new TeamFoundationControl(this));

    m_teamFoundationPluginInstance = this;

    m_settings.readSettings(Core::ICore::settings());

    m_teamFoundationClient = new TeamFoundationClient(this);
    addAutoReleasedObject(new SettingsPage);

    createMenus();

    return true;
}

TeamFoundationPlugin *TeamFoundationPlugin::instance()
{
    QTC_ASSERT(m_teamFoundationPluginInstance, return m_teamFoundationPluginInstance);
    return m_teamFoundationPluginInstance;
}

const TeamFoundationSettings &TeamFoundationPlugin::settings() const
{
    return this->m_settings;
}

void TeamFoundationPlugin::setSettings(const TeamFoundationSettings &s)
{
    if (s != m_settings) {
        m_settings = s;
        m_settings.writeSettings(Core::ICore::settings());
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
    if (!enableMenuAction(as, m_menuAction))
    {
        m_commandLocator->setEnabled(false);
        return;
    }

    const bool hasTopLevel = currentState().hasTopLevel();
    m_commandLocator->setEnabled(hasTopLevel);

    const QString currentFile = currentState().currentFileName();
    for (Utils::ParameterAction *action: m_fileActionList)
    {
        action->setParameter(currentFile);
    }
}

Q_EXPORT_PLUGIN2(TeamFoundation, TeamFoundationPlugin)

