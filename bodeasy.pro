#-------------------------------------------------
#
# Project created by QtCreator 2011-01-25T18:44:07
#
#-------------------------------------------------

TARGET = bodeasy
TEMPLATE = app
DESTDIR = .
BUILD_PATH = build

CONFIG -= warn_on warn_off release debug debug_and_release
CONFIG	*= qt warn_on release
QT       *= core gui sql

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

FORMS    *= src/usersdialog.ui

HEADERS  *= src/usersdialog.h \
         src/user.h \
         src/usersmanager.h \
         src/usersmodel.h \
         src/exercise.h \
         src/exercisesmanager.h

SOURCES *= src/main.cpp \
        src/usersdialog.cpp \
        src/user.cpp \
        src/usersmanager.cpp \
        src/usersmodel.cpp \
        src/exercise.cpp \
        src/exercisesmanager.cpp
