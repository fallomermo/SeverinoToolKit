#ifndef RELACAOCOLABORADOR_H
#define RELACAOCOLABORADOR_H

#include <QMap>
#include <QFile>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include <QMapIterator>
#include <QModelIndex>


// Inlcuindo clases manuais do projeto
#include "pesquisar.h"
#include "controledao.h"
#include "cadastroempresa.h"
#include "cadastrofilial.h"
#include "cadastrocolaborador.h"
#include "exportararquivo.h"


namespace Ui {
class RelacaoColaborador;
}

class RelacaoColaborador : public QWidget
{
    Q_OBJECT

public:
    explicit RelacaoColaborador(QWidget *parent = 0);
    RelacaoColaborador(QWidget *parent, QMap<int, CadastroEmpresa *> ce, QMap<int, CadastroFilial *> cf);
    ~RelacaoColaborador();

    QMap<int, CadastroEmpresa *> getMapEmpresas() const;
    void setMapEmpresas(const QMap<int, CadastroEmpresa *> &value);

    QMap<int, CadastroFilial *> getMapFiliais() const;
    void setMapFiliais(const QMap<int, CadastroFilial *> &value);

private slots:
    void filtroItemTabela(QString);
    void pesquisarEmpresa();
    void setEmpresa(QString);
    void pesquisarFilial();
    void setFilial(QString);
    void retornaCadastroEmpresa();
    void retornaCadastroFilial();
    void retornaCadastroFilial(QString);
    void focusDataReferencia();
    void getDatatable();
    void inserirItemTabela(int,int,QString);
    void inserirItemTabela(int,int,QDate);
    void inserirItemTabela(int, int, double);
    void inserirItemTabela(int,int,int);
    void inserirLinhaTabela(int, int, CadastroColaborador *);
    void exportarParaExcel();
    void exibirNumeroRegistros(QModelIndex);
    void exibirDataCompleta();
    void exibirDataCompleta(QDate);
    void caixaMensagemUsuario(QString);

private:
    Ui::RelacaoColaborador *ui;
    QMap<int, CadastroEmpresa *> mapEmpresas;
    QMap<int, CadastroFilial *> mapFiliais;
    ControleDAO *controle;
};

#endif // RELACAOCOLABORADOR_H
