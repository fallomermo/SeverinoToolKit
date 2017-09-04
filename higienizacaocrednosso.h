#ifndef HIGIENIZACAOCREDNOSSO_H
#define HIGIENIZACAOCREDNOSSO_H

#include <QWidget>
#include <QDate>
#include <QDebug>
#include <QMapIterator>
#include <QMap>

#include "objetocrednossobeneficios.h"
#include "objetocrednossoretorno.h"
#include "exportararquivo.h"
#include "caixamensagemprogresso.h"

class ControleDAO;
class CaixaMensagemProgresso;

namespace Ui {
class HigienizacaoCrednosso;
}

class HigienizacaoCrednosso : public QWidget
{
    Q_OBJECT

public:
    explicit HigienizacaoCrednosso(QWidget *parent = 0);
    ~HigienizacaoCrednosso();

signals:
    void finishThread();
    void obterCrednossoRetorno(QDate);
    void fecharCaixaMensagem();

private slots:
    void getDatatable();
    void inserirItemTabela(int,int,QString);
    void inserirItemTabela(int,int,QDate);
    void inserirItemTabela(int, int, double);
    void inserirItemTabela(int,int,int);
    void inserirLinhaTabela(int, int, ObjetoCrednossoRetorno *);
    void exportarParaExcel();
    void atualizarResultados(QModelIndex);
    void mensagemInfoUsuario(QString);
    void filtroItemTabela(QString);
    void preencherTabela(QMap<int, ObjetoCrednossoRetorno*>);
    void caixaMensagemUsuario(QString);

private:
    Ui::HigienizacaoCrednosso *ui;
    ControleDAO *controle;
    CaixaMensagemProgresso *progresso;
};

#endif // HIGIENIZACAOCREDNOSSO_H
