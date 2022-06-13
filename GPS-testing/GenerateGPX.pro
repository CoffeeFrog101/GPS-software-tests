TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++17 -Wall -Wfatal-errors

HEADERS += \
    headers/dataFiles.h \
    headers/earth.h \
    headers/geometry.h \
    headers/points.h \
    headers/position.h \
    headers/types.h \
    headers/gridworld/gridworld-model.h \
    headers/gridworld/gridworld-route.h \
    headers/gridworld/gridworld-track.h \
    headers/xml/xml-generator.h \
    headers/xml/xml-element.h


SOURCES += \
    apps/generateGPX.cpp

SOURCES += \
    src/dataFiles.cpp \
    src/earth.cpp \
    src/geometry.cpp \
    src/position.cpp \
    src/gridworld/gridworld-model.cpp \
    src/gridworld/gridworld-route.cpp \
    src/gridworld/gridworld-track.cpp \
    src/xml/xml-generator.cpp \
    src/xml/xml-element.cpp


INCLUDEPATH += headers/ headers/xml/ headers/gridworld

OBJECTS_DIR = $$_PRO_FILE_PWD_/bin/
DESTDIR = $$_PRO_FILE_PWD_/bin/
TARGET = generateGPX
