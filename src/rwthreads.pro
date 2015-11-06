QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rwthreads
TEMPLATE = app
CONFIG += debug

SOURCES  += main.cpp \
            mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

#Reader
SOURCES += reader/reader.cpp

HEADERS += reader/reader.h

#Buffer
SOURCES += buffer/buffer.cpp

HEADERS += buffer/buffer.h
