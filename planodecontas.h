#ifndef PLANODECONTAS_H
#define PLANODECONTAS_H

#include <QMap>
#include <QDate>
#include <QFile>
#include <QImage>
#include <QMovie>
#include <QWidget>
#include <QMultiMap>
#include <QMessageBox>
#include <QModelIndex>
#include <QFileDialog>
#include <QMapIterator>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QProgressDialog>
#include <QTableWidgetItem>


// Inlcuindo clases manuais do projeto
#include "cadastroempresa.h"
#include "cadastrofilial.h"
#include "controledao.h"
#include "pesquisar.h"
#include "exportararquivo.h"


namespace Ui {
class PlanoDeContas;
}

class PlanoDeContas : public QWidget, QModelIndex
{
    Q_OBJECT

public:
    explicit PlanoDeContas(QWidget *parent = 0);
    PlanoDeContas(QWidget *parent, QMap<int, CadastroEmpresa*> ce, QMap<int, CadastroFilial *> cf);
    ~PlanoDeContas();

    QMap<int, CadastroEmpresa *> getMapEmpresas() const;

    QMap<int, CadastroFilial *> getMapFiliais() const;

signals:
    void statusProgresso(bool);

private slots:
    void atualizarTabela();
    void empresaSelecionada();
    void filialSelecionada();
    void filialSelecionada(QString);
    void competenciaSelecionada();
    void focusInicioPeriodo();
    void focusFinalPeriodo();
    void focusProcessar();
    void progressoRequisicao(bool);
    void caixaMensagemUsuario(QString);

    QString pesquisarItem();
    void pesquisarEmpresa();
    void pesquisarFilial();
    void setEmpresa(QString);
    void setFilial(QString);
    void getDatatable();
    void inserirItemTabela(int,int,QString);
    void inserirItemTabela(int,int,QDate);
    void inserirItemTabela(int, int, double);
    void inserirItemTabela(int,int,int);
    void inserirLinhaTabela(int, int, Eventos *);
    void exportarParaExcel();
    void atualizarResultados(QModelIndex);
    void mensagemInfoUsuario(QString);
    void filtroItemTabela(QString);

private:
    Ui::PlanoDeContas *ui;
    QMap<int, CadastroEmpresa*> mapEmpresas;
    QMap<int, CadastroFilial*> mapFiliais;
    CadastroEmpresa *_cemp;
    CadastroFilial *_cfil;
    ControleDAO *controle;
    bool _statusProgresso;
};

#endif // PLANODECONTAS_H
