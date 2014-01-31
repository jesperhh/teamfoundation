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

#ifndef TEAMFOUNDATION_GLOBAL_H
#define TEAMFOUNDATION_GLOBAL_H

#include <QtGlobal>

#if defined(TEAMFOUNDATION_LIBRARY)
#  define TEAMFOUNDATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TEAMFOUNDATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TEAMFOUNDATION_GLOBAL_H

