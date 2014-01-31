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
    QString password() const;
    QString username() const;
    int timeOut() const;
    QString tftpBinaryPath() const;
private:
    mutable QString m_tfptBinaryPath;
};

} // namespace Internal
} // namespace TeamFoundation

#endif // TEAMFOUNDATIONSETTINGS_H
