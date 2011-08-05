XUPProjectSettings {
	OTHERS_PLATFORM_TARGET_RELEASE	= /media/ramdisk/bodeasy/bodeasy
	QT_VERSION	= Qt System (4.7.2)
	MAC_PLATFORM_TARGET_RELEASE	= bodeasy.app
}

#-------------------------------------------------
#
# Project created by QtCreator 2011-01-25T18:44:07
#
#-------------------------------------------------

TARGET	= bodeasy
TEMPLATE	= app
DESTDIR	= .
BUILD_PATH	= build
UNIX_RAM_DISK	= /media/ramdisk
unix:exists( $${UNIX_RAM_DISK} ) {
	BUILD_PATH	= $${UNIX_RAM_DISK}/$${TARGET}
	DESTDIR = $${BUILD_PATH}
}

!win32:CONFIG	-= warn_on warn_off release debug debug_and_release
CONFIG	*= qt debug warn_on

QT	= core gui sql

INCLUDEPATH	*= . src

CONFIG( debug, debug|release ) {
	message( "Debug build." )
	MOC_DIR	= $${BUILD_PATH}/debug/moc
	RCC_DIR	= $${BUILD_PATH}/debug/rcc
	OBJECTS_DIR	= $${BUILD_PATH}/debug/obj
	UI_DIR	= $${BUILD_PATH}/debug/ui
	TARGET	= $${TARGET}d
} else {
	message( "Release build." )
	MOC_DIR	= $${BUILD_PATH}/release/moc
	RCC_DIR	= $${BUILD_PATH}/release/rcc
	OBJECTS_DIR	= $${BUILD_PATH}/release/obj
	UI_DIR	= $${BUILD_PATH}/release/ui
}

include( src/sql/sql.pri )
include( src/utils/modeltest/modeltest.pri )

RESOURCES	*= src/ressources.qrc

win32 {
	INCLUDEPATH *= C:/Qwt-6.0.0/include
	LIBS += -LC:/Qwt-6.0.0/lib -lqwt
}

unix {
	INCLUDEPATH	*= /usr/local/qwt-6.0.1/include
	LIBS += -L/usr/local/qwt-6.0.1/lib -lqwt
}

FORMS	*= src/usersdialog.ui \
	src/exercisewidget.ui \
	src/exercisesdialog.ui \
	src/sessionsdialog.ui \
	src/mensurationsdialog.ui \
	src/mainwindow.ui \
	src/sessionframe.ui \
	src/widgets/sessioncontrolwidget.ui \
	src/settingsdialog.ui

HEADERS	*= src/usersdialog.h \
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
	src/exercisewidgetdata.h \
	src/widgets/sessioncontrolwidget.h \
	src/settingsdialog.h \
	src/settings.h \
	src/graphicsdialog.h \
	src/widgets/plots/activityplot.h \
	src/widgets/plots/weightplot.h

SOURCES	*= src/main.cpp \
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
	src/widgets/sessioncontrolwidget.cpp \
	src/settingsdialog.cpp \
	src/graphicsdialog.cpp \
	src/widgets/plots/activityplot.cpp \
	src/widgets/plots/weightplot.cpp
