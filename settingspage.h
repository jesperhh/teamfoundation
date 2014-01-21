#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include "ui_settingspage.h"

#include <vcsbase/vcsbaseoptionspage.h>

#include <QWidget>
#include <QPointer>
#include <QString>

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

    QString searchKeywords() const;

private:
    Ui::SettingsPage m_ui;
};


class SettingsPage : public VcsBase::VcsBaseOptionsPage
{
    Q_OBJECT

public:
    SettingsPage();

    QWidget *createPage(QWidget *parent);
    void apply();
    void finish() { }
    bool matches(const QString &) const;

private:
    QString m_searchKeywords;
    SettingsPageWidget* m_widget;
};

} // namespace TeamFoundation
} // namespace Internal

#endif  // SETTINGSPAGE_H
