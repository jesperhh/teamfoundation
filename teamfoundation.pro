DEFINES += TEAMFOUNDATION_LIBRARY

# TeamFoundation files

SOURCES += \
        teamfoundationplugin.cpp \
        teamfoundationcontrol.cpp \
        teamfoundationsettings.cpp \
        settingspage.cpp \
        teamfoundationclient.cpp \
        checkoutwizard.cpp \
        checkoutwizardpage.cpp

HEADERS += \
        teamfoundationplugin.h \
        teamfoundation_global.h \
        teamfoundationconstants.h \
        teamfoundationcontrol.h \
        teamfoundationsettings.h \
        settingspage.h \
        teamfoundationclient.h \
        checkoutwizard.h \
        checkoutwizardpage.h


FORMS += settingspage.ui

RESOURCES += teamfoundation.qrc

OTHER_FILES += TeamFoundation.pluginspec.in

# Qt Creator linking

## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES):QTCREATOR_SOURCES=C:/Qt/CreatorFromSource/qt-creator-opensource-src-3.0.0/

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE):IDE_BUILD_TREE=C:/Qt/CreatorFromSource/build-qtcreator-Desktop_Qt_5_2_0_MSVC2012_32bit-Release/

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on Mac
# USE_USER_DESTDIR = yes

PROVIDER = TeamFoundation

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.

QTC_PLUGIN_NAME = TeamFoundation
QTC_LIB_DEPENDS += \
    # nothing here at this time

QTC_PLUGIN_DEPENDS += \
    coreplugin \
    vcsbase

QTC_PLUGIN_RECOMMENDS += \
    # optional plugin dependencies. nothing here at this time

###### End _dependencies.pri contents ######

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)

