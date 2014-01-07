#ifndef TEAMFOUNDATION_H
#define TEAMFOUNDATION_H

#include "teamfoundation_global.h"

#include <vcsbase/vcsbaseclient.h>
#include <vcsbase/vcsbaseplugin.h>
#include <coreplugin/icontext.h>

namespace TeamFoundation {
namespace Internal {

class TeamFoundationPlugin : public VcsBase::VcsBasePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "TeamFoundation.json")

public:
    TeamFoundationPlugin();
    ~TeamFoundationPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
//    void extensionsInitialized();
//    ShutdownFlag aboutToShutdown();

private slots:
//    void triggerAction();

protected:
    void updateActions(VcsBase::VcsBasePlugin::ActionState);
    bool submitEditorAboutToClose();

private:
};

} // namespace Internal
} // namespace TeamFoundation

#endif // TEAMFOUNDATION_H

