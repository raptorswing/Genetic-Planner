#-------------------------------------------------
#
# Project created by QtCreator 2012-04-30T15:09:54
#
#-------------------------------------------------

QT       += core gui network

TARGET = GeneticPlanner
TEMPLATE = app


SOURCES += main.cpp\
        gui/MainWindow.cpp \
    Individual.cpp \
    Planner.cpp \
    FitnessRunnable.cpp \
    gui/UAVParametersWidget.cpp \
    UAVParameters.cpp \
    SensorDefinition.cpp \
    gui/SensorParametersWidget.cpp

HEADERS  += gui/MainWindow.h \
    Individual.h \
    Planner.h \
    FitnessRunnable.h \
    gui/UAVParametersWidget.h \
    UAVParameters.h \
    SensorDefinition.h \
    gui/SensorParametersWidget.h

FORMS    += gui/MainWindow.ui \
    gui/UAVParametersWidget.ui \
    gui/SensorParametersWidget.ui

#Linkage for MapGraphics library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MapGraphics/release/ -lMapGraphics
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MapGraphics/debug/ -lMapGraphics
else:unix:!symbian: LIBS += -L$$OUT_PWD/../MapGraphics/ -lMapGraphics

INCLUDEPATH += $$PWD/../MapGraphics
DEPENDPATH += $$PWD/../MapGraphics

RESOURCES += \
    resources.qrc
