QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rwthreads
TEMPLATE = app
CONFIG += debug

SOURCES  += main.cpp \
            mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

#Buffer
SOURCES += buffer/buffer.cpp

HEADERS += buffer/buffer.h

#Controller
SOURCES += controller/controller.cpp

HEADERS += controller/controller.h

#Cryptographer
SOURCES += cryptographer/cryptographer.cpp

HEADERS += cryptographer/cryptographer.h

#Reader
SOURCES += reader/reader.cpp

HEADERS += reader/reader.h

#Writer
SOURCES += writer/writer.cpp

HEADERS += writer/writer.h
