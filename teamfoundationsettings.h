#ifndef TEAMFOUNDATIONSETTINGS_H
#define TEAMFOUNDATIONSETTINGS_H

#include <vcsbase/vcsbaseclientsettings.h>

namespace TeamFoundation {
namespace Internal {

class TeamFoundationSettings : public VcsBase::VcsBaseClientSettings
{
public:
    static const QLatin1String powerToolsBinaryPathKey;
    static const QLatin1String passwordKey;
    TeamFoundationSettings();
    int timeOut() const;
    QString tftpBinaryPath() const;
private:
    mutable QString m_tfptBinaryPath;
};

} // namespace Internal
} // namespace TeamFoundation

#endif // TEAMFOUNDATIONSETTINGS_H
