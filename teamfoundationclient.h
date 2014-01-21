#ifndef TEAMFOUNDATIONCLIENT_H
#define TEAMFOUNDATIONCLIENT_H

#include <QObject>
#include <vcsbase/vcsbaseplugin.h>

namespace TeamFoundation {
namespace Internal {

class TeamFoundationPlugin;

struct TeamFoundationResponse
{
    TeamFoundationResponse() : error(false) {}
    bool error;
    QString standardOut;
    QString standardError;
    QString message;
};

class TeamFoundationClient: public QObject
{
    Q_OBJECT

public:
    explicit TeamFoundationClient(TeamFoundationPlugin *plugin);

    bool addFile(const QString &file) const;
    bool deleteFile(const QString &file) const;
    bool moveFile(const QString &from, const QString &to) const;
    bool compareFile(const QString &file);
    bool undoFile(const QString &file);
    bool checkInFile(const QString &file);
    bool checkoutFile(const QString &fileName) const;
    bool annotateFile(const QString &fileName) const;
    bool managesFile(const QString &fileName) const;
    bool managesDirectory(const QString &directory) const;
private slots:
    void addCurrentFile();
    void deleteCurrentFile();
    void compareCurrentFile();
    void undoCurrentFile();
    void checkInCurrentFile();
    void annotateCurrentFile();

private:
    TeamFoundationResponse runTf(const QString &workingDirectory, const QStringList &arguments,
                                 unsigned flags = VcsBase::VcsBasePlugin::ShowStdOutInLogWindow) const;
    TeamFoundationResponse runTfpt(const QString &workingDirectory, const QStringList &arguments,
                                   unsigned flags = VcsBase::VcsBasePlugin::ShowStdOutInLogWindow) const;
    TeamFoundationResponse runVcs(const QString &workingDirectory,	const QString &executable,
                                  const QStringList &arguments, unsigned flags) const;

    TeamFoundationPlugin* m_plugin;

    friend class TeamFoundationPlugin;
};

} // namespace Internal
} // namespace TeamFoundation

#endif // TEAMFOUNDATIONCLIENT_H
