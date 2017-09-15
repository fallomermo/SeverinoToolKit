#-------------------------------------------------
#
# Project created by QtCreator 2017-06-23T16:35:27
#
#-------------------------------------------------

QT       += core gui sql network

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
QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder


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
    exportararquivo.cpp \
    objetoretencao.cpp \
    metaretencao.cpp \
    homeinicio.cpp \
    metaretencaoestruturada.cpp \
    higienizacaocrednosso.cpp \
    responsavelselecaoagregado.cpp \
    threadobject.cpp \
    caixamensagemprogresso.cpp \
    detalhesretencao.cpp \
    objetocrednossoretorno.cpp \
    objetocrednossohigienizacao.cpp \
    objetocrednossobeneficios.cpp \
    processararquivocrednosso.cpp \
    objetocadastroupdatefile.cpp \
    blissbeneficios.cpp \
    validacao.cpp \
    updatedatatablecolumm.cpp \
    truncararquivostexto.cpp \
    controlador.cpp \
    downloadahgoraafd.cpp \
    login.cpp \
    ecoclinic.cpp \
    usuarios.cpp \
    relacaocolaboradores.cpp

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
    exportararquivo.h \
    objetoretencao.h \
    metaretencao.h \
    homeinicio.h \
    metaretencaoestruturada.h \
    higienizacaocrednosso.h \
    responsavelselecaoagregado.h \
    threadobject.h \
    caixamensagemprogresso.h \
    detalhesretencao.h \
    objetocrednossoretorno.h \
    objetocrednossohigienizacao.h \
    objetocrednossobeneficios.h \
    processararquivocrednosso.h \
    objetocadastroupdatefile.h \
    blissbeneficios.h \
    validacao.h \
    updatedatatablecolumm.h \
    truncararquivostexto.h \
    controlador.h \
    downloadahgoraafd.h \
    login.h \
    ecoclinic.h \
    usuarios.h \
    relacaocolaboradores.h

FORMS += \
    guiainssfolha.ui \
    pesquisar.ui \
    planodecontas.ui \
    planosaude.ui \
    principal.ui \
    relacaocolaborador.ui \
    metaretencao.ui \
    homeinicio.ui \
    metaretencaoestruturada.ui \
    higienizacaocrednosso.ui \
    caixamensagemprogresso.ui \
    detalhesretencao.ui \
    processararquivocrednosso.ui \
    blissbeneficios.ui \
    truncararquivostexto.ui \
    downloadahgoraafd.ui \
    login.ui \
    ecoclinic.ui \
    relacaocolaboradores.ui

RESOURCES += \
    thema.qrc
