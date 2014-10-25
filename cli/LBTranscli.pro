#-------------------------------------------------
#
# Project created by QtCreator 2014-10-25T18:49:34
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = lbtrans-cli
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

TEMPLATE = app


SOURCES += main.cpp \
    ../cbaidutranslater.cpp \
    ../entities/cbaidutranslateresult.cpp

HEADERS += \
    ../cbaidutranslater.h \
    ../entities/cbaidutranslateresult.h
