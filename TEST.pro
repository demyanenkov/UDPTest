QT       += core network gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TEST
TEMPLATE = app

SOURCES += main.cpp mainwindow.cpp
HEADERS +=          mainwindow.h
FORMS +=            mainwindow.ui

DISTFILES += README.md
