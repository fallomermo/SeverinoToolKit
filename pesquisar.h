#ifndef PESQUISAR_H
#define PESQUISAR_H

#include <QWidget>
#include <QMap>
#include <QDebug>
#include <QMapIterator>
#include <QMessageBox>


// Inlcuindo clases manuais do projeto
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
    Pesquisar(QWidget *parent, QMap<int, CadastroEmpresa*>, QMap<int, CadastroFilial *>, QString, int);
    ~Pesquisar();

    QMap<int, CadastroEmpresa *> getMapEmpresas() const;

    QMap<int, CadastroFilial *> getMapFiliais() const;

    QString getEmpresaSelecionada() const;

    int getFlagTipoPesquisa() const;

signals:
    void getData(QString);
    void getEmpresa(QString);
    void getFilial(QString);

private slots:
    void preencherTabela();
    void setData(int, int);
    void setData();
    void setEmpresa(int,int);
    void setEmpresa();
    void setFilial(int,int);
    void setFilial();
    void fecharPesquisa();

private:
    Ui::Pesquisar *ui;
    QMap<int, CadastroEmpresa *> mapEmpresas;
    QMap<int, CadastroFilial *> mapFiliais;
    QString empresaSelecionada;
    int flagTipoPesquisa;
};

#endif // PESQUISAR_H
