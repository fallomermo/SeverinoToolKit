#-------------------------------------------------
#
# Project created by QtCreator 2017-06-23T16:35:27
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SeverinoToolKit
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    bancodedados.cpp \
    cadastrocolaborador.cpp \
    cadastroempresa.cpp \
    cadastrofilial.cpp \
    controledao.cpp \
    ecoclinicrepasses.cpp \
    eventos.cpp \
    guiainssfolha.cpp \
    pesquisar.cpp \
    planodecontas.cpp \
    planosaude.cpp \
    principal.cpp \
    relacaocolaborador.cpp \
    main.cpp \
    exportararquivo.cpp

HEADERS += \
    bancodedados.h \
    cadastrocolaborador.h \
    cadastroempresa.h \
    cadastrofilial.h \
    controledao.h \
    ecoclinicrepasses.h \
    eventos.h \
    guiainssfolha.h \
    pesquisar.h \
    planodecontas.h \
    planosaude.h \
    principal.h \
    relacaocolaborador.h \
    exportararquivo.h

FORMS += \
    guiainssfolha.ui \
    pesquisar.ui \
    planodecontas.ui \
    planosaude.ui \
    principal.ui \
    relacaocolaborador.ui

RESOURCES += \
    thema.qrc
