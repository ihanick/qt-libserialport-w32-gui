#-------------------------------------------------
#
# Project created by QtCreator 2015-02-08T05:38:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtlibserialport-gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serialport.c \
    windows.c \
    libserialportq.cpp

HEADERS  += mainwindow.h \
    libserialport.h \
    libserialport_internal.h \
    libserialportq.h

FORMS    += mainwindow.ui

LIBS += -lsetupapi

DEFINES += SP_PRIV= SP_API= STDC_HEADERS=1 HAVE_SYS_TYPES_H=1 HAVE_SYS_STAT_H=1 HAVE_STDLIB_H=1 HAVE_STRING_H=1 HAVE_MEMORY_H=1 HAVE_STRINGS_H=1 HAVE_INTTYPES_H=1 HAVE_STDINT_H=1 HAVE_UNISTD_H=1
