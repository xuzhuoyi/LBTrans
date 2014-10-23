#-------------------------------------------------
#
# Project created by QtCreator 2014-07-14T16:43:25
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lbtrans
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    cbaidutranslater.cpp \
    controls/qgiflabel.cpp \
    entities/cbaidutranslateresult.cpp

HEADERS  += mainwindow.h \
    cbaidutranslater.h \
    controls/qgiflabel.h \
    entities/cbaidutranslateresult.h

FORMS    += \
    mainwindow.ui \
    aboutdialog.ui

TRANSLATIONS += $$PWD/language/zh.ts

OTHER_FILES += \
    .gitignore

RESOURCES += \
    res.qrc

win32:{
    RC_ICONS = $$PWD/res/windowIcon.ico
}

INCLUDEPATH += $$PWD $$PWD/controls

OBJECTS_DIR += $$OUT_PWD/tmp
MOC_DIR     += $$OUT_PWD/tmp
UI_DIR      += $$OUT_PWD/tmp
RCC_DIR     += $$OUT_PWD/tmp
DESTDIR     += $$OUT_PWD
