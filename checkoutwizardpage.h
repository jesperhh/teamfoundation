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

#ifndef CHECKOUTWIZARDPAGE_H
#define CHECKOUTWIZARDPAGE_H

#include <vcsbase/basecheckoutwizardpage.h>

namespace TeamFoundation {
namespace Internal {

class CheckoutWizardPage : public VcsBase::BaseCheckoutWizardPage {
    Q_OBJECT
public:
    CheckoutWizardPage(QWidget *parent = 0);
    QString collection() const;
    QString workspace() const;
protected:
    QString directoryFromRepository(const QString &r) const;

private:
    QLineEdit *collectionEdit;
    QLineEdit *workspaceEdit;
};

} // namespace Internal
} // namespace TeamFoundation
#endif // CHECKOUTWIZARDPAGE_H
