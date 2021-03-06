XUP.OTHERS_PLATFORM_TARGET_DEFAULT = /media/ramdisk/bodeasy/tst_bodeasyd
XUP.OTHERS_PLATFORM_TARGET_RELEASE = /media/ramdisk/bodeasy/bodeasy
XUP.OTHERS_PLATFORM_TARGET_DEBUG = /media/ramdisk/bodeasy/tst_bodeasyd
XUP.QT_VERSION = Qt (4.8.0rc1)

#-------------------------------------------------
#
# Project created by QtCreator 2011-01-25T18:44:07
#
#-------------------------------------------------

TARGET = bodeasy
TEMPLATE = app
DESTDIR = .
BUILD_PATH = build
UNIX_RAM_DISK = /media/ramdisk
unix:exists( $${UNIX_RAM_DISK} ) {
    BUILD_PATH = $${UNIX_RAM_DISK}/$${TARGET}
    DESTDIR = $${BUILD_PATH}
}

CONFIG -= warn_on warn_off release debug debug_and_release
CONFIG *= debug warn_on qt

QT = core gui sql declarative

# Mac universal build from 10.3 to up to 10.6
macx {
    SDK_PATH = $$(MAC_SDKS_PATH)
    isEmpty( SDK_PATH ):SDK_PATH = /Developer/SDKs
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.3
    QMAKE_MAC_SDK = $${SDK_PATH}/MacOSX10.6.sdk
    CONFIG *= x86 x86_64 ppc app_bundle
    # this link is required for building the ppc port to avoid the undefined __Unwind_Resume symbol
    CONFIG( ppc ):LIBS *= -lgcc_eh
}

INCLUDEPATH *= . \
    src

CONFIG( debug, debug|release ) {
    message( "Debug build." )
    MOC_DIR = $${BUILD_PATH}/debug/moc
    RCC_DIR = $${BUILD_PATH}/debug/rcc
    OBJECTS_DIR = $${BUILD_PATH}/debug/obj
    UI_DIR = $${BUILD_PATH}/debug/ui
    TARGET = $${TARGET}d
} else {
    message( "Release build." )
    MOC_DIR = $${BUILD_PATH}/release/moc
    RCC_DIR = $${BUILD_PATH}/release/rcc
    OBJECTS_DIR = $${BUILD_PATH}/release/obj
    UI_DIR = $${BUILD_PATH}/release/ui
}

include( src/sql/sql.pri )
include( src/utils/modeltest/modeltest.pri )

RESOURCES *= src/ressources.qrc

# try to read variable environment
KDCHART_DIR = $$(KDCHART_DIR)

isEmpty( KDCHART_DIR ) {
    win32 {
       KDCHART_DIR = C:/KDAB/KDChart-2.4.3
    }
}

!isEmpty( KDCHART_DIR ) {
    DEFINES *= KDCHART_STATICLIB HAVE_KDCHART
    INCLUDEPATH *= $${KDCHART_DIR}/include
    LIBS *= -L$${KDCHART_DIR}/lib -ltesttools
    win32:LIBS *= -lkdchartd
    else:LIBS *= -lkdchart2

    HEADERS *= src/graphicsdialog.h \
        src/widgets/charts/activitychart.h \
        src/widgets/charts/weightchart.h \
        src/widgets/charts/mensurationschart.h \
        src/legendmodel.h

    SOURCES *= src/graphicsdialog.cpp \
        src/widgets/charts/activitychart.cpp \
        src/widgets/charts/weightchart.cpp \
        src/widgets/charts/mensurationschart.cpp \
        src/legendmodel.cpp

    message( "Using KDChart at: $$KDCHART_DIR" )
} else {
    message( "KDChart path wasn't defined, graphics won't be enabled" )
    message( "You can help find the install path using:" )
    message( "Unix and derivated: export KDCHART_DIR=path/to/install/dir && qmake" )
    message( "Windows: SET KDCHART_DIR=path/to/install/dir && qmake" )
}

FORMS *= src/usersdialog.ui \
    src/exercisewidget.ui \
    src/exercisesdialog.ui \
    src/sessionsdialog.ui \
    src/mensurationsdialog.ui \
    src/mainwindow.ui \
    src/sessionframe.ui \
    src/widgets/sessioncontrolwidget.ui \
    src/settingsdialog.ui \
    src/graphicsdialog.ui \
    src/planningwindow.ui \
    src/plandialog.ui

HEADERS *= src/usersdialog.h \
    src/exercisewidget.h \
    src/exercisesdialog.h \
    src/sessionsdialog.h \
    src/mensurationsdialog.h \
    src/ruleritem.h \
    src/mainwindow.h \
    src/sessionframe.h \
    src/widgets/pChronometer.h \
    src/widgets/pHeaderView.h \
    src/widgets/pView.h \
    src/SessionProxy.h \
    src/views/icon/SessionIconModel.h \
    src/views/icon/SessionIconDelegate.h \
    src/views/icon/SessionIconView.h \
    src/views/calendar/abstractcalendarmodel.h \
    src/views/calendar/basiccalendarmodel.h \
    src/views/calendar/calendaritem.h \
    src/exercisewidgetdata.h \
    src/widgets/sessioncontrolwidget.h \
    src/settingsdialog.h \
    src/settings.h \
    src/planningwindow.h \
    src/views/calendar/calendarview.h \
    src/views/calendar/basiccalendaritem.h \
    src/plandialog.h

SOURCES *= src/main.cpp \
    src/usersdialog.cpp \
    src/exercisewidget.cpp \
    src/exercisesdialog.cpp \
    src/sessionsdialog.cpp \
    src/mensurationsdialog.cpp \
    src/ruleritem.cpp \
    src/mainwindow.cpp \
    src/sessionframe.cpp \
    src/widgets/pChronometer.cpp \
    src/widgets/pHeaderView.cpp \
    src/widgets/pView.cpp \
    src/SessionProxy.cpp \
    src/views/icon/SessionIconModel.cpp \
    src/views/icon/SessionIconDelegate.cpp \
    src/views/icon/SessionIconView.cpp \
    src/views/calendar/abstractcalendarmodel.cpp \
    src/views/calendar/basiccalendarmodel.cpp \
    src/views/calendar/calendaritem.cpp \
    src/widgets/sessioncontrolwidget.cpp \
    src/settingsdialog.cpp \
    src/planningwindow.cpp \
    src/views/calendar/calendarview.cpp \
    src/views/calendar/basiccalendaritem.cpp \
    src/plandialog.cpp

OTHER_FILES *= src/views/calendar/qml/CalendarView.qml \
               src/views/calendar/qml/GridDelegate.qml
