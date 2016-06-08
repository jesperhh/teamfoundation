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

#include "settingspage.h"
#include "teamfoundationsettings.h"
#include "teamfoundationplugin.h"

using namespace TeamFoundation::Internal;
using namespace Utils;

SettingsPageWidget::SettingsPageWidget(QWidget *parent) :
    QWidget(parent)
{
    m_ui.setupUi(this);
    m_ui.pathChooser->setExpectedKind(PathChooser::ExistingCommand);
    m_ui.pathChooser->setPromptDialogTitle(tr("Team Foundation Command"));
    m_ui.tfptPathChooser->setExpectedKind(PathChooser::ExistingCommand);
    m_ui.tfptPathChooser->setPromptDialogTitle(tr("Team Foundation Power Tools Command"));
}

TeamFoundationSettings SettingsPageWidget::settings() const
{
    TeamFoundationSettings rc;
    rc.setValue(TeamFoundationSettings::binaryPathKey, m_ui.pathChooser->rawPath());
    rc.setValue(TeamFoundationSettings::powerToolsBinaryPathKey, m_ui.tfptPathChooser->rawPath());
    rc.setValue(TeamFoundationSettings::userNameKey, m_ui.usernameLineEdit->text());
    rc.setValue(TeamFoundationSettings::passwordKey, m_ui.passwordLineEdit->text());
    rc.setValue(TeamFoundationSettings::timeoutKey, m_ui.timeOutSpinBox->value());
    rc.setValue(TeamFoundationSettings::revertUnchangedFilesBeforeCheckinKey, m_ui.revertUnchangedFilesBeforeCheckin->isChecked());
    return rc;
}

void SettingsPageWidget::setSettings(const TeamFoundationSettings &s)
{
    m_ui.pathChooser->setFileName(s.binaryPath());
    m_ui.tfptPathChooser->setFileName(s.tftpBinaryPath());
    m_ui.usernameLineEdit->setText(s.stringValue(TeamFoundationSettings::userNameKey));
    m_ui.passwordLineEdit->setText(s.stringValue(TeamFoundationSettings::passwordKey));
    m_ui.timeOutSpinBox->setValue(s.intValue(TeamFoundationSettings::timeoutKey));
    m_ui.revertUnchangedFilesBeforeCheckin->setChecked(s.boolValue(TeamFoundationSettings::revertUnchangedFilesBeforeCheckinKey));
}

SettingsPage::SettingsPage() :
    m_widget(0)
{
    setId("T.TeamFoundation");
    setDisplayName(tr("TeamFoundation"));
}

QWidget *SettingsPage::widget()
{
    if (!m_widget) {
        m_widget = new SettingsPageWidget;
        m_widget->setSettings(TeamFoundationPlugin::instance()->settings());
    }
    return m_widget;
}

void SettingsPage::apply()
{
    TeamFoundationPlugin::instance()->setSettings(m_widget->settings());
}

void SettingsPage::finish()
{
    delete m_widget;
}
