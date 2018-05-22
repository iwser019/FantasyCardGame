#-------------------------------------------------
#
# Project created by QtCreator 2017-10-08T17:46:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FantasyCardGame
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    gamesession.cpp \
    randomizer.cpp \
    card.cpp \
    effect.cpp \
    player.cpp \
    gameform.cpp \
    namegen.cpp \
    transmitters.cpp \
    effectselector.cpp \
    util.cpp \
    consolegameform.cpp \
    gamecontroller.cpp \
    cardfactory.cpp \
    effectfactory.cpp \
    cardpool.cpp \
    gamestatemanager.cpp \
    gamestate.cpp \
    command.cpp

HEADERS  += mainwindow.h \
    gamesession.h \
    randomizer.h \
    card.h \
    effect.h \
    player.h \
    gameform.h \
    defs.h \
    namegen.h \
    transmitters.h \
    effectselector.h \
    util.h \
    consolegameform.h \
    gamecontroller.h \
    cardfactory.h \
    effectfactory.h \
    cardpool.h \
    gamestatemanager.h \
    gamestate.h \
    command.h

FORMS    += mainwindow.ui \
    gameform.ui \
    consolegameform.ui

CONFIG   += c++11 c++14
