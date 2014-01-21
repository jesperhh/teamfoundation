#ifndef TEAMFOUNDATIONCONTROL_H
#define TEAMFOUNDATIONCONTROL_H

#include <coreplugin/iversioncontrol.h>

namespace TeamFoundation {
namespace Internal {

class TeamFoundationPlugin;

// Just a proxy for TeamFoundationPlugin
class TeamFoundationControl : public Core::IVersionControl
{
    Q_OBJECT
public:
    explicit TeamFoundationControl(TeamFoundationPlugin *plugin);
    QString displayName() const;
    Core::Id id() const;

    bool managesDirectory(const QString &directory, QString *topLevel = 0) const;
    bool managesFile(const QString &workingDirectory, const QString &fileName) const;
    bool isConfigured() const;
    bool supportsOperation(Operation operation) const;
    OpenSupportMode openSupportMode() const;
    bool vcsOpen(const QString &fileName);
    bool vcsAdd(const QString &fileName);
    bool vcsDelete(const QString &filename);
    bool vcsMove(const QString &from, const QString &to);
    bool vcsCreateRepository(const QString &directory);
    bool vcsCheckout(const QString &directory, const QByteArray &url);
    QString vcsGetRepositoryURL(const QString &directory);
    bool vcsAnnotate(const QString &file, int line);

private:
    TeamFoundationPlugin *m_plugin;
};

} // namespace Internal
} // namespace TeamFoundation

#endif // TEAMFOUNDATIONCONTROL_H
