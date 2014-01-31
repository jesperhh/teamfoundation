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

#include "checkoutwizard.h"
#include "checkoutwizardpage.h"
#include "teamfoundationplugin.h"
#include "teamfoundationconstants.h"
#include "teamfoundationclient.h"

#include <coreplugin/iversioncontrol.h>
#include <vcsbase/command.h>
#include <vcsbase/vcsconfigurationpage.h>
#include <utils/qtcassert.h>

#include <QIcon>

namespace TeamFoundation {
namespace Internal {

CheckoutWizard::CheckoutWizard()
{
    setId(QLatin1String(TeamFoundation::Constants::VCS_ID_TEAMFOUNDATION));
    setIcon(QIcon(QLatin1String(":/teamfoundation/images/teamfoundation.png")));
    setDescription(tr("Maps a Team Foundation Server folder and tries to load the contained project."));
    setDisplayName(tr("Team Foundation Checkout"));
}

QList<QWizardPage*> CheckoutWizard::createParameterPages(const QString &path)
{
    QList<QWizardPage*> rc;
    const Core::IVersionControl *vc = TeamFoundationPlugin::instance()->versionControl();
    if (!vc->isConfigured())
        rc.append(new VcsBase::VcsConfigurationPage(vc));
    CheckoutWizardPage *cwp = new CheckoutWizardPage;
    cwp->setPath(path);
    rc.append(cwp);
    return rc;
}

VcsBase::Command *CheckoutWizard::createCommand(const QList<QWizardPage*> &parameterPages,
                                                QString *checkoutPath)
{
    // Collect parameters for the checkout command.
    const CheckoutWizardPage *cwp = qobject_cast<const CheckoutWizardPage *>(parameterPages.front());
    QTC_ASSERT(cwp, return 0);
    const TeamFoundationSettings settings = TeamFoundationPlugin::instance()->settings();
    const QString directory = cwp->directory();
    QStringList mapArgs, getArgs;
    mapArgs << QLatin1String("workfold")
         << QLatin1String("/map")
         << cwp->repository()
         << directory;

    getArgs << QLatin1String("get") << directory << QLatin1String("/recursive");

    const QString workingDirectory = cwp->path();
    *checkoutPath = workingDirectory + QLatin1Char('/') + directory;
    VcsBase::Command *command = new VcsBase::Command(settings.binaryPath(), workingDirectory,
                                                     QProcessEnvironment::systemEnvironment());

    if (!cwp->collection().isEmpty())
    {
        mapArgs << QLatin1Literal("/collection:") + cwp->collection();
    }

    if(!cwp->workspace().isEmpty())
    {
        QStringList workspaceArgs;
        workspaceArgs << QLatin1String("workspace")
            << QLatin1String("/new")
            << cwp->workspace()
            << QLatin1Literal("/collection:") + cwp->collection();

        TeamFoundationClient::addAuthentication(workspaceArgs);
        command->addJob(workspaceArgs, -1);

        mapArgs << QLatin1Literal("/workspace:") + cwp->workspace();
    }

    TeamFoundationClient::addAuthentication(mapArgs);
    TeamFoundationClient::addAuthentication(getArgs);
    command->addJob(mapArgs, -1);
    command->addJob(getArgs, -1);
    return command;
}

} // namespace Internal
} // namespace TeamFoundation
