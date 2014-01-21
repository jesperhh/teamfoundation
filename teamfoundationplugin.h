#ifndef TEAMFOUNDATION_H
#define TEAMFOUNDATION_H

#include "teamfoundation_global.h"
#include "teamfoundationsettings.h"

#include <vcsbase/vcsbaseclient.h>
#include <vcsbase/vcsbaseplugin.h>
#include <coreplugin/icontext.h>

QT_BEGIN_NAMESPACE
class QAction;
QT_END_NAMESPACE

namespace Core {
class ActionManager;
class ActionContainer;
class Id;
class Command;
}

namespace Locator {
class CommandLocator;
}

namespace Utils {
class ParameterAction;
}

namespace TeamFoundation {
namespace Internal {

class TeamFoundationClient;

class TeamFoundationPlugin : public VcsBase::VcsBasePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "TeamFoundation.json")

public:
    TeamFoundationPlugin();
    ~TeamFoundationPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);

    const TeamFoundationSettings &settings() const;
    void setSettings(const TeamFoundationSettings &s);

    const TeamFoundationClient *client() const;

    static TeamFoundationPlugin *instance();

private slots:

protected:
    void createMenus();
    Core::Command *createFileCommand(const Core::Context &context, Core::ActionContainer* container,
                                     const QString &emptyText, const QString &parameterText, const char *actionId);
    void updateActions(VcsBase::VcsBasePlugin::ActionState);
    bool submitEditorAboutToClose();

private:
    TeamFoundationSettings m_settings;
    Locator::CommandLocator *m_commandLocator;
    QAction *m_menuAction;
    QList<Utils::ParameterAction*> m_fileActionList;
    TeamFoundationClient *m_teamFoundationClient;
    static TeamFoundationPlugin *m_teamFoundationPluginInstance;
};

} // namespace Internal
} // namespace TeamFoundation

#endif // TEAMFOUNDATION_H

