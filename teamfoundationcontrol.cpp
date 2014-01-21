#include "teamfoundationcontrol.h"
#include "teamfoundationplugin.h"
#include "teamfoundationsettings.h"
#include "teamfoundationclient.h"

#include <vcsbase/vcsbaseconstants.h>
#include <vcsbase/vcsbaseplugin.h>
#include <utils/synchronousprocess.h>

#include <QFileInfo>
#include <QStringList>
#include <QString>
#include <qdir.h>

#define RUN_PREAMBLE_1(file) \
    const QFileInfo file##Info(file); \
    QStringList parameters;

#define RUN_PREAMBLE_2(file1, file2) \
    const QFileInfo file1##Info(file1); \
    const QFileInfo file2##Info(file2); \
    QStringList parameters;

using namespace TeamFoundation;
using namespace TeamFoundation::Internal;

TeamFoundationControl::TeamFoundationControl(TeamFoundationPlugin *plugin) :
    m_plugin(plugin)
{
}

QString TeamFoundationControl::displayName() const
{
    return QLatin1String("Team Foundation");
}

Core::Id TeamFoundationControl::id() const
{
    return Core::Id("T.TeamFoundation");
}

bool TeamFoundationControl::isConfigured() const
{
    const QString binary = m_plugin->settings().binaryPath();
    if (binary.isEmpty())
    {
        return false;
    }

    QFileInfo fi(binary);
    return fi.exists() && fi.isFile() && fi.isExecutable();
}

bool TeamFoundationControl::supportsOperation(Operation operation) const
{
    bool result = isConfigured();
    switch (operation)
    {
    case AddOperation:
    case DeleteOperation:
    case MoveOperation:
    case CheckoutOperation:
    case GetRepositoryRootOperation:
        break;
    case CreateRepositoryOperation:
    case SnapshotOperations:
    case AnnotateOperation:
        result = false;
        break;
    }

    return result;
}

Core::IVersionControl::OpenSupportMode TeamFoundationControl::openSupportMode() const
{
    return Core::IVersionControl::OpenMandatory;
}

bool TeamFoundationControl::vcsOpen(const QString & fileName)
{
    return m_plugin->client()->checkoutFile(fileName);
}

bool TeamFoundationControl::vcsAdd(const QString &fileName)
{
    return m_plugin->client()->addFile(fileName);
}

bool TeamFoundationControl::vcsDelete(const QString &fileName)
{
    return m_plugin->client()->deleteFile(fileName);
}

bool TeamFoundationControl::vcsMove(const QString &from, const QString &to)
{
    return m_plugin->client()->moveFile(from, to);
}

bool TeamFoundationControl::managesDirectory(const QString &directory, QString *topLevel) const
{
    bool isManaged = m_plugin->client()->managesDirectory(directory);
    if (!isManaged)
        return false;

    QDir dir(directory);
    while (true)
    {
        *topLevel = dir.absolutePath();
        if (!dir.cdUp() || !m_plugin->client()->managesDirectory(dir.absolutePath()))
            break;
    }

    return true;
}

bool TeamFoundationControl::managesFile(const QString &workingDirectory, const QString &fileName) const
{
    QDir dir(workingDirectory);
    const QString absoluteFileName = dir.absoluteFilePath(fileName);
    return m_plugin->client()->managesFile(absoluteFileName);
}

bool TeamFoundationControl::vcsCheckout(const QString &directory, const QByteArray &url)
{
    Q_UNUSED(directory)
    Q_UNUSED(url);
    return true;
}

QString TeamFoundationControl::vcsGetRepositoryURL(const QString &directory)
{
    Q_UNUSED(directory)
    return QLatin1String("");
}

bool TeamFoundationControl::vcsCreateRepository(const QString &directory )
{
    Q_UNUSED(directory)
    return false;
}

bool TeamFoundationControl::vcsAnnotate(const QString &file, int line)
{
    Q_UNUSED(file)
    Q_UNUSED(line)
    return true;
}
