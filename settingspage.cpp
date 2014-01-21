#include "settingspage.h"
#include "teamfoundationsettings.h"
#include "teamfoundationplugin.h"

#include <coreplugin/icore.h>
#include <extensionsystem/pluginmanager.h>
#include <vcsbase/vcsbaseconstants.h>
#include <utils/pathchooser.h>

#include <QCoreApplication>
#include <QTextStream>
#include <QFileDialog>

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
    return rc;
}

void SettingsPageWidget::setSettings(const TeamFoundationSettings &s)
{
    m_ui.pathChooser->setPath(s.binaryPath());
    m_ui.tfptPathChooser->setPath(s.tftpBinaryPath());
    m_ui.usernameLineEdit->setText(s.stringValue(TeamFoundationSettings::userNameKey));
    m_ui.passwordLineEdit->setText(s.stringValue(TeamFoundationSettings::passwordKey));
    m_ui.timeOutSpinBox->setValue(s.intValue(TeamFoundationSettings::timeoutKey));
}

QString SettingsPageWidget::searchKeywords() const
{
    QString rc;
    QLatin1Char sep(' ');
    QTextStream(&rc)
            << sep << m_ui.generalGroupBox->title()
            << sep << m_ui.commandLabel->text()
            << sep << m_ui.tfptLabel->text()
            << sep << m_ui.userGroupBox->title()
            << sep << m_ui.usernameLabel->text()
            << sep << m_ui.passwordLabel->text()
            << sep << m_ui.miscGroupBox->title()
            << sep << m_ui.timeOutLabel->text()
               ;
    rc.remove(QLatin1Char('&'));
    return rc;
}

SettingsPage::SettingsPage() :
    m_widget(0)
{
    setId("T.TeamFoundation");
    setDisplayName(tr("TeamFoundation"));
}

QWidget *SettingsPage::createPage(QWidget *parent)
{
    m_widget = new SettingsPageWidget(parent);
    m_widget->setSettings(TeamFoundationPlugin::instance()->settings());
    if (m_searchKeywords.isEmpty())
        m_searchKeywords = m_widget->searchKeywords();
    return m_widget;
}

void SettingsPage::apply()
{
    TeamFoundationPlugin::instance()->setSettings(m_widget->settings());
}

bool SettingsPage::matches(const QString &s) const
{
    return m_searchKeywords.contains(s, Qt::CaseInsensitive);
}
