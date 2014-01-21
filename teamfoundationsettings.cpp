#include "teamfoundationsettings.h"

#include <utils/environment.h>
#include <utils/hostosinfo.h>

namespace TeamFoundation {
namespace Internal {

const QLatin1String TeamFoundationSettings::powerToolsBinaryPathKey("PowerToolsBinaryPath");
const QLatin1String TeamFoundationSettings::passwordKey("Password");

TeamFoundationSettings::TeamFoundationSettings()
{
    setSettingsGroup(QLatin1String("TeamFoundation"));
    declareKey(binaryPathKey, QLatin1String("tf" QTC_HOST_EXE_SUFFIX));
    declareKey(powerToolsBinaryPathKey, QLatin1String("tfpt" QTC_HOST_EXE_SUFFIX));
    declareKey(passwordKey, QLatin1String(""));
}

int TeamFoundationSettings::timeOut() const
{
    return 1000 * intValue(timeoutKey);
}

QString TeamFoundationSettings::tftpBinaryPath() const
{
    if (m_tfptBinaryPath.isEmpty()) {
        m_tfptBinaryPath = Utils::Environment::systemEnvironment().searchInPath(
                    stringValue(powerToolsBinaryPathKey), stringValue(pathKey).split(
                        Utils::HostOsInfo::pathListSeparator()));
    }
    return m_tfptBinaryPath;
}

} // namespace Internal
} // namespace TeamFoundation
