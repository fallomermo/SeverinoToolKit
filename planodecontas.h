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
#include "caixamensagemprogresso.h"


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

    QString getCodigoEmpresa() const;
    void setCodigoEmpresa(const QString &value);

    QString getCodigoFilial() const;
    void setCodigoFilial(const QString &value);

signals:
    void finishThread();
    void statusProgresso(bool);
    void progressValue(int);
    void minimumProgressValue(int);
    void maximumProgressValue(int);
    void fecharCaixaMensagem();
    void retornaPlanoDeContas(QString,QString,QString,QString,int);

private slots:
    void atualizarTabela();
    void empresaSelecionada();
    void filialSelecionada();
    void filialSelecionada(const QString);
    void competenciaSelecionada();
    void focusInicioPeriodo();
    void focusFinalPeriodo();
    void focusProcessar();
    void progressoRequisicao(bool);
    void caixaMensagemUsuario(const QString);

    QString pesquisarItem();
    void pesquisarEmpresa();
    void pesquisarFilial();
    void setEmpresa(const QString);
    void setFilial(const QString);
    void getDatatable();
    void inserirItemTabela(int, int, QString);
    void inserirItemTabela(int, int, QDate);
    void inserirItemTabela(int, int, double);
    void inserirItemTabela(int, int, int);
    void inserirLinhaTabela(int, int, Eventos *);
    void exportarParaExcel();
    void atualizarResultados(QModelIndex);
    void mensagemInfoUsuario(const QString, const QString);
    void filtroItemTabela(const QString);
    void preencherTabela(QMap<int, Eventos *>);

private:
    Ui::PlanoDeContas *ui;
    QMap<int, CadastroEmpresa*> mapEmpresas;
    QMap<int, CadastroFilial*> mapFiliais;
    CadastroEmpresa *_cemp;
    CadastroFilial *_cfil;
    ControleDAO *controle;
    bool _statusProgresso;

    QString codigoEmpresa;
    QString codigoFilial;

    enum {
        ID_EMPRESA, EMPRESA, ID_FILIAL, FILIAL, CNPJ, CIDADE_REGIAO, CALCULO, COMPETENCIA, TIPO_CALCULO, SETOR, CODEVENTO, EVENTO, TIPOEVENTO, VALOR
    };
};

#endif // PLANODECONTAS_H
