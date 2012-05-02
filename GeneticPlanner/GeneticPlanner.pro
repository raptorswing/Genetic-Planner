#-------------------------------------------------
#
# Project created by QtCreator 2012-04-30T15:09:54
#
#-------------------------------------------------

QT       += core gui network

TARGET = GeneticPlanner
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    Individual.cpp \
    Planner.cpp \
    FitnessRunnable.cpp

HEADERS  += MainWindow.h \
    Individual.h \
    Planner.h \
    FitnessRunnable.h

FORMS    += MainWindow.ui

#Linkage for MapGraphics library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MapGraphics/release/ -lMapGraphics
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MapGraphics/debug/ -lMapGraphics
else:unix:!symbian: LIBS += -L$$OUT_PWD/../MapGraphics/ -lMapGraphics

INCLUDEPATH += $$PWD/../MapGraphics
DEPENDPATH += $$PWD/../MapGraphics
