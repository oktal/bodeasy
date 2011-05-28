XUPProjectSettings {
	OTHERS_PLATFORM_TARGET_RELEASE	= bodeasy
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

!win32:CONFIG	-= warn_on warn_off release debug debug_and_release
CONFIG	+= qt release warn_on

QT	= core gui sql

INCLUDEPATH	+= . src


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

FORMS	+= src/usersdialog.ui \
	src/exercisewidget.ui \
	src/exercisesdialog.ui \
	src/sessionsdialog.ui \
	src/mensurationsdialog.ui \
	src/mainwindow.ui \
	src/sessionframe.ui

HEADERS	+= src/usersdialog.h \
	src/exercisewidget.h \
	src/exercisesdialog.h \
	src/sessionsdialog.h \
	src/mensurationsdialog.h \
	src/ruleritem.h \
	src/mainwindow.h \
	src/sessionframe.h \
	src/widgets/pChronometer.h \
	src/widgets/pHeaderView.h \
	src/widgets/pView.h

SOURCES	+= src/main.cpp \
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
	src/widgets/pView.cpp

RESOURCES	+= src/ressources.qrc
