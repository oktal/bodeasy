#-------------------------------------------------
#
# Project created by QtCreator 2011-01-25T18:44:07
#
#-------------------------------------------------

QT       += core gui sql

TARGET = bodeasy
TEMPLATE = app


SOURCES += src/main.cpp \
        src/usersdialog.cpp \
    src/exercise.cpp \
    src/user.cpp \
    src/usersmanager.cpp \
    src/usersmodel.cpp

HEADERS  += src/usersdialog.h \
    src/exercise.h \
    src/user.h \
    src/usersmanager.h \
    src/usersmodel.h

FORMS    += src/usersdialog.ui

CONFIG(debug)
{
    MOC_DIR = bin/debug/moc
    RCC_DIR = bin/debug/rcc
    OBJECTS_DIR = bin/debug/obj
    DESTDIR = bin/debug
    UI_DIR = bin/debug/ui
}

CONFIG(release)
{
    MOC_DIR = bin/release/moc
    RCC_DIR = bin/release/rcc
    OBJECTS_DIR = bin/release/obj
    DESTDIR = bin/release
    UI_DIR = bin/release/ui
}
