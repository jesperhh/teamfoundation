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

#include "teamfoundationsettings.h"

#include <utils/environment.h>

namespace TeamFoundation {
namespace Internal {

const QLatin1String TeamFoundationSettings::powerToolsBinaryPathKey("PowerToolsBinaryPath");
const QLatin1String TeamFoundationSettings::passwordKey("Password");
const QLatin1String TeamFoundationSettings::revertUnchangedFilesBeforeCheckinKey("RevertUnchangedBeforeCheckin");


TeamFoundationSettings::TeamFoundationSettings()
{
    setSettingsGroup(QLatin1String("TeamFoundation"));
    declareKey(binaryPathKey, QLatin1String("tf" QTC_HOST_EXE_SUFFIX));
    declareKey(powerToolsBinaryPathKey, QLatin1String("tfpt" QTC_HOST_EXE_SUFFIX));
    declareKey(passwordKey, QString());
    declareKey(revertUnchangedFilesBeforeCheckinKey, true);
}

QString TeamFoundationSettings::password() const
{
    return stringValue(passwordKey);
}

QString TeamFoundationSettings::username() const
{
    return stringValue(userNameKey);
}

int TeamFoundationSettings::timeOut() const
{
    return 1000 * intValue(timeoutKey);
}

Utils::FileName TeamFoundationSettings::tftpBinaryPath() const
{
    if (m_tfptBinaryPath.isEmpty()) {
        m_tfptBinaryPath = Utils::Environment::systemEnvironment().searchInPath(
                    stringValue(powerToolsBinaryPathKey), stringValue(pathKey).split(
                        Utils::HostOsInfo::pathListSeparator()));
    }
    return m_tfptBinaryPath;
}

bool TeamFoundationSettings::revertUnchangedFilesBeforeCheckin() const
{
    return boolValue(revertUnchangedFilesBeforeCheckinKey);
}

} // namespace Internal
} // namespace TeamFoundation
