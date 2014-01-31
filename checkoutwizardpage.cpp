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

#include "checkoutwizardpage.h"

#include <QLineEdit>

namespace TeamFoundation {
namespace Internal {

CheckoutWizardPage::CheckoutWizardPage(QWidget *parent) :
    VcsBase::BaseCheckoutWizardPage(parent), collectionEdit(0)
{
    setTitle(tr("Location"));
    setSubTitle(tr("Specify server path, checkout directory and path."));
    setRepositoryLabel(tr("Server path:"));
    setBranchSelectorVisible(false);

    setRepository(QLatin1String("$/"));

    collectionEdit = new QLineEdit(this);
    collectionEdit->setObjectName(QLatin1String("collectionEdit"));
    addRepositoryControl(tr("Collection: (leave empty for default"), collectionEdit);

    workspaceEdit = new QLineEdit(this);
    workspaceEdit->setObjectName(QLatin1String("workspaceEdit"));
    addRepositoryControl(tr("Workspace: (leave empty for default)"), workspaceEdit);
}

QString CheckoutWizardPage::collection() const
{
    return collectionEdit->text();
}

QString CheckoutWizardPage::workspace() const
{
    return workspaceEdit->text();
}

QString CheckoutWizardPage::directoryFromRepository(const QString &repoIn) const
{
    QString repo = repoIn.trimmed();
    const QChar slash = QLatin1Char('/');
    // remove host
    const int slashPos = repo.lastIndexOf(slash);
    if (slashPos != -1)
        repo.remove(0, slashPos + 1);
    repo.replace(QLatin1String("$"), QLatin1String(""));
    return repo;
}

} // namespace Internal
} // namespace TeamFoundation
