#ifndef METARETENCAOESTRUTURADA_H
#define METARETENCAOESTRUTURADA_H

#include <QMap>
#include <QDate>
#include <QDebug>
#include <QLocale>
#include <QPixmap>
#include <QMovie>
#include <QWidget>
#include <QThread>
#include <QToolButton>
#include <QModelIndex>
#include <QMapIterator>
#include <QMessageBox>


#include "objetoretencao.h"
#include "exportararquivo.h"
#include "controledao.h"
#include "responsavelselecaoagregado.h"
#include "detalhesretencao.h"

class DetalhesRetencao;

class CaixaDeMensagem;

namespace Ui {
class MetaRetencaoEstruturada;
}

class MetaRetencaoEstruturada : public QWidget
{
    Q_OBJECT

public:
    explicit MetaRetencaoEstruturada(QWidget *parent = 0);
    ~MetaRetencaoEstruturada();

    QMap<int, ObjetoRetencao *> getMapRetencao() const;
    void setMapRetencao(const QMap<int, ObjetoRetencao *> &value);

signals:
    void setProgressoValue(int);
    void setMinimumValue(int);
    void setMaximumValue(int);
    void fecharCaixaDeMensagem();
    void obterMetaRetencao(QDate, QDate);
    void finishThread();

private slots:
    void definirParametrosIniciais();
    void filtroItemTabela(QString);
    void focusPeriodoInicial();
    void focusPeriodoFinal();
    void getDatatable();
    void inserirItemTabela(int, int, QString);
    void inserirItemTabela(int, int, QDate);
    void inserirItemTabela(int, int, double);
    void inserirItemTabela(int, int, int);
    void inserirItemTabela(int, int, QWidget*);
    void inserirLinhaTabela(int, int, ResponsavelSelecaoAgregado *);
    void preencherTabela(const QMap<int, ObjetoRetencao *>);
    void exportarParaExcel();
    void caixaMensagemUsuario(QString);
    void atualizarResultados(QModelIndex);
    void setRetencao(const QMap<int, ObjetoRetencao *> &value);
    void removerItemTabela();
    void detalhesRetencao(QModelIndex);
    void detalhesRetencao();
    QMap<QString, ResponsavelSelecaoAgregado*> agregarValores(const QMap<int, ObjetoRetencao*>);
    void itemClicado(QModelIndex);

private:
    Ui::MetaRetencaoEstruturada *ui;
    DetalhesRetencao *detalhes;

    ControleDAO *controle;
    QMap<int, ObjetoRetencao*> mapRetencao;
    CaixaMensagemProgresso *caixaMensagem;
};

#endif // METARETENCAOESTRUTURADA_H
