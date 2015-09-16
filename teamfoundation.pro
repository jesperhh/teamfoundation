DEFINES += TEAMFOUNDATION_LIBRARY

# TeamFoundation files

SOURCES += \
        teamfoundationplugin.cpp \
        teamfoundationcontrol.cpp \
        teamfoundationsettings.cpp \
        settingspage.cpp \
        teamfoundationclient.cpp

HEADERS += \
        teamfoundationplugin.h \
        teamfoundation_global.h \
        teamfoundationconstants.h \
        teamfoundationcontrol.h \
        teamfoundationsettings.h \
        settingspage.h \
        teamfoundationclient.h


FORMS += settingspage.ui

OTHER_FILES += TeamFoundation.json.in \
            wizard/wizard.json \
            wizard/icon.png

# Qt Creator linking

## set the QTC_SOURCE environment variable to override the setting here
QTCREATOR_SOURCES = $$(QTC_SOURCE)
isEmpty(QTCREATOR_SOURCES) {
    QTCREATOR_SOURCES=$$files(../qt-creator*)
	QTCREATOR_SOURCES=$$first(QTCREATOR_SOURCES)
}

## set the QTC_BUILD environment variable to override the setting here
IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE) {
    IDE_BUILD_TREE=$$files(../build-qtcreator-*)
    IDE_BUILD_TREE=$$first(IDE_BUILD_TREE)
}

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%\QtProject\qtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on Mac
# USE_USER_DESTDIR = yes

PROVIDER = TeamFoundation

include($$QTCREATOR_SOURCES/src/qtcreatorplugin.pri)

