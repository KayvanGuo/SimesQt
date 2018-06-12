#-------------------------------------------------
#
# Project created by QtCreator 2015-03-28T23:49:34
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += widgets
QT += widgets printsupport


DEFINES += BCCWIN DAVE_LITTLE_ENDIAN

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DaveQt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    plcqtlib.cpp \
    qcustomplot.cpp \
    plotwindow.cpp \
    item.cpp \
    ConSiemensPlcs.cpp

HEADERS  += mainwindow.h \
    plcqtlib.h \
    qcustomplot.h \
    plotwindow.h \
    item.h \
    libnodave/nodave.h \
    ConSiemensPlcs.h

FORMS    += mainwindow.ui \
    plotwindow.ui \
    ConSiemensPlcs.ui

#win32: LIBS += -L$$PWD/libnodave/ -llibnodave
#INCLUDEPATH += $$PWD/libnodave
#DEPENDPATH += $$PWD/libnodave

## add the lib by myself

win32: LIBS += -L$$PWD/libnodave/ -llibnodave

INCLUDEPATH += $$PWD/libnodave
DEPENDPATH += $$PWD/libnodave
