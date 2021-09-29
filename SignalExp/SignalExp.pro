#-------------------------------------------------
#
# Project created by QtCreator 2018-01-25T09:57:38
#
#-------------------------------------------------

QT       += core gui gui-private
load(qt_parts)
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport

LIBS += $$PWD\libs\SignalRecog.dll


TARGET = SignalExp
TEMPLATE = app

include(QtXlsxWriter-master/src/xlsx/qtxlsx.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    msg.cpp \
    sigclassificator.cpp \
    sigdata.cpp \
    qcustomplot.cpp \
		dlgtrain.cpp \
    sigfactory.cpp \
    xi2classificator.cpp \
    dlgresp.cpp \
    statclassificator.cpp \
    dlgnewgroup.cpp \
    dlgselect.cpp \
    dlgmark.cpp \
    sigmath.cpp \
    dlgscale.cpp \
    dlgnorm.cpp \
    dlgselgroup.cpp \
    dlgmean.cpp \
    dlgxi2.cpp \
    mdclassificator.cpp \
    xi4classificator.cpp \
    nnclassificator.cpp \
    dlgselrange.cpp \
    dlgnnet.cpp \
    dlggenerate.cpp \
    Alglib/fasttransforms.cpp \
    Alglib/ap.cpp \
    Alglib/alglibinternal.cpp


HEADERS  += mainwindow.h \
    msg.h \
    res.h \
    sigdata.h \
    sigclassificator.h \
    qcustomplot.h \
		dlgtrain.h \
    sigfactory.h \
    xi2classificator.h \
    dlgresp.h \
    SignalRecog.h \
    statclassificator.h \
    dlgnewgroup.h \
    dlgselect.h \
    selrule.h \
    dlgmark.h \
    sigmath.h \
    dlgscale.h \
    dlgnorm.h \
    dlgselgroup.h \
    dlgmean.h \
    dlgxi2.h \
    mdclassificator.h \
    xi4classificator.h \
    nnclassificator.h \
    dlgselrange.h \
    dlgnnet.h \
    dlggenerate.h \
    rndmt.h \
    Alglib/fasttransforms.h \
    Alglib/alglibinternal.h \
    Alglib/ap.h



FORMS    += mainwindow.ui \
    dlgtrain.ui \
    dlgresp.ui \
    dlgnewgroup.ui \
    dlgselect.ui \
    dlgmark.ui \
    dlgscale.ui \
    dlgnorm.ui \
    dlgselgroup.ui \
    dlgmean.ui \
    dlgxi2.ui \
    dlgselrange.ui \
    dlgnnet.ui \
    dlggenerate.ui

DISTFILES += \
    ToDo.txt
