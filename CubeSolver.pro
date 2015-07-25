#-------------------------------------------------
#
# Project created by QtCreator 2015-07-25T07:50:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CubeSolver
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    rcube.cpp \
    cubesolver.cpp

HEADERS  += mainwindow.h \
    rcube.h \
    cubesolver.h

FORMS    += mainwindow.ui
