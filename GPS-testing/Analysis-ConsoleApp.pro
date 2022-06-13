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
    headers/waypoints.h \
    headers/analysis/analysis-route.h \
    headers/analysis/analysis-track.h \
    headers/gpx/gpx-parser.h \
    headers/xml/xml-element.h \
    headers/xml/xml-parser.h

SOURCES += \
    apps/analysis-main.cpp

SOURCES += \
    src/dataFiles.cpp \
    src/earth.cpp \
    src/geometry.cpp \
    src/position.cpp \
    src/analysis/analysis-route.cpp \
    src/analysis/analysis-track.cpp \
    src/gpx/gpx-parser.cpp \
    src/xml/xml-element.cpp \
    src/xml/xml-parser.cpp

INCLUDEPATH += headers/ headers/analysis/ headers/gpx/ headers/xml/

OBJECTS_DIR = $$_PRO_FILE_PWD_/bin/
DESTDIR = $$_PRO_FILE_PWD_/bin/
TARGET = analysis-consoleApp
