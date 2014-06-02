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

#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include "ui_settingspage.h"

#include <vcsbase/vcsbaseoptionspage.h>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

namespace TeamFoundation {
namespace Internal {

class TeamFoundationSettings;

class SettingsPageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsPageWidget(QWidget *parent = 0);

    TeamFoundationSettings settings() const;
    void setSettings(const TeamFoundationSettings &);

private:
    Ui::SettingsPage m_ui;
};


class SettingsPage : public VcsBase::VcsBaseOptionsPage
{
    Q_OBJECT

public:
    SettingsPage();

    QWidget *widget();
    void apply();
    void finish();

private:
    QPointer<SettingsPageWidget> m_widget;
};

} // namespace TeamFoundation
} // namespace Internal

#endif  // SETTINGSPAGE_H
