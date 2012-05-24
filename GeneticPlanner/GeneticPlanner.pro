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
    gui/SensorParametersWidget.cpp \
    gui/PlanningControlWidget.cpp \
    gui/PlanningWizard.cpp \
    PlanningProblem.cpp \
    tasks/PathTask.cpp \
    tasks/FlyThroughTask.cpp \
    gui/PaletteWidget.cpp \
    tasks/EndingTask.cpp \
    tasks/NoFlyTask.cpp \
    TaskAreaObject.cpp \
    gui/MWCommand.cpp \
    TaskArea.cpp \
    Position.cpp \
    Pose.cpp \
    ProblemModelAdapter.cpp

HEADERS  += gui/MainWindow.h \
    Individual.h \
    Planner.h \
    FitnessRunnable.h \
    gui/UAVParametersWidget.h \
    UAVParameters.h \
    SensorDefinition.h \
    gui/SensorParametersWidget.h \
    gui/PlanningControlWidget.h \
    gui/PlanningWizard.h \
    PlanningProblem.h \
    tasks/PathTask.h \
    tasks/FlyThroughTask.h \
    gui/PaletteWidget.h \
    tasks/EndingTask.h \
    tasks/NoFlyTask.h \
    TaskAreaObject.h \
    gui/MWCommand.h \
    TaskArea.h \
    Position.h \
    Pose.h \
    ProblemModelAdapter.h

FORMS    += gui/MainWindow.ui \
    gui/UAVParametersWidget.ui \
    gui/SensorParametersWidget.ui \
    gui/PlanningControlWidget.ui \
    gui/PlanningWizard.ui \
    gui/PaletteWidget.ui

#Linkage for MapGraphics library
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../MapGraphics/release/ -lMapGraphics
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../MapGraphics/debug/ -lMapGraphics
else:unix:!symbian: LIBS += -L$$OUT_PWD/../MapGraphics/ -lMapGraphics

INCLUDEPATH += $$PWD/../MapGraphics
DEPENDPATH += $$PWD/../MapGraphics

RESOURCES += \
    resources.qrc
