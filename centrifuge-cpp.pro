QT += core websockets
QT -= gui

CONFIG += c++11

TARGET = centrifuge-cpp
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp centrifuge.cpp \
    consolereader.cpp

HEADERS += centrifuge.h \
    consolereader.h
