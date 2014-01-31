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

#ifndef CHECKOUTWIZARD_H
#define CHECKOUTWIZARD_H

#include <vcsbase/basecheckoutwizard.h>

namespace TeamFoundation {
namespace Internal {

class CheckoutWizard : public VcsBase::BaseCheckoutWizard
{
    Q_OBJECT
public:
    CheckoutWizard();

private:
    QList<QWizardPage*> createParameterPages(const QString &path);
    VcsBase::Command *createCommand(const QList<QWizardPage*> &parameterPage,
                                    QString *checkoutPath);
};

} // namespace Internal
} // namespace TeamFoundation

#endif // CHECKOUTWIZARD_H
