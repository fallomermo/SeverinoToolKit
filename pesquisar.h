#ifndef PESQUISAR_H
#define PESQUISAR_H

#include <QWidget>
#include <QMap>
#include <QDebug>
#include <QMapIterator>
#include <QMessageBox>


// Inlcuindo clases manuais do projeto
#include "controledao.h"
#include "cadastroempresa.h"
#include "cadastrofilial.h"


namespace Ui {
class Pesquisar;
}

class Pesquisar : public QWidget
{
    Q_OBJECT

public:
    explicit Pesquisar(QWidget *parent = 0);
    Pesquisar(QWidget *parent, QString, int);
    ~Pesquisar();

    QMap<int, CadastroEmpresa *> getMapEmpresas() const;

    QMap<int, CadastroFilial *> getMapFiliais() const;

    QString getEmpresaSelecionada() const;

    int getTipoPesquisa() const;

signals:
    void getData(QString);
    void getEmpresa(QString);
    void getFilial(QString);

    void obterEmpresas();
    void obterFiliais();

    void finishThread();

private slots:
    void preencherTabelaComEmpresas(QMap<int, CadastroEmpresa*>);
    void preencherTabelaComFiliais(QMap<int, CadastroFilial*>);
    void setData(int, int);
    void setData();
    void setEmpresa(int,int);
    void setEmpresa();
    void setFilial(int,int);
    void setFilial();
    void fecharPesquisa();

private:
    Ui::Pesquisar *ui;
    ControleDAO *controle;
    BancoDeDados *db;
    QMap<int, CadastroEmpresa *> mapEmpresas;
    QMap<int, CadastroFilial *> mapFiliais;
    QString empresaSelecionada;
    int tipoPesquisa;
};

#endif // PESQUISAR_H
