#ifndef METARETENCAO_H
#define METARETENCAO_H

#include <QWidget>
#include <QMap>
#include <QDate>
#include <QThread>
#include <QMovie>
#include <QToolButton>
#include <QModelIndex>
#include <QMapIterator>
#include <QMessageBox>


#include "objetoretencao.h"
#include "exportararquivo.h"
#include "controledao.h"
#include "caixamensagemprogresso.h"

class CaixaMensagemProgresso;

namespace Ui {
class MetaRetencao;
}

class MetaRetencao : public QWidget
{
    Q_OBJECT

public:
    explicit MetaRetencao(QWidget *parent = 0);
    ~MetaRetencao();

    QMap<int, ObjetoRetencao *> getMapRetencao() const;
    void setMapRetencao(const QMap<int, ObjetoRetencao *> &value);

signals:
    void progressValue(int);
    void minimumProgressValue(int);
    void maximumProgressValue(int);
    void fecharCaixaDeMensagem();
    void finishThread();
    void obterMetaRetencao(QDate, QDate);

private slots:
    void filtroItemTabela(QString);
    void focusPeriodoInicial();
    void focusPeriodoFinal();
    void getDatatable();
    void inserirItemTabela(int,int,QString);
    void inserirItemTabela(int,int,QDate);
    void inserirItemTabela(int, int, double);
    void inserirItemTabela(int,int,int);
    void inserirItemTabela(int,int,QWidget*);
    void inserirLinhaTabela(int, int, ObjetoRetencao *);
    void exportarParaExcel();
    void caixaMensagemUsuario(QString);
    void atualizarResultados();
    void setRetencao(const QMap<int, ObjetoRetencao *> &value);
    void preencherTabela(QMap<int, ObjetoRetencao *>);
    void removerItemTabela();
    void cancelarOperacao();

private:
    Ui::MetaRetencao *ui;
    ControleDAO *controle;
    CaixaMensagemProgresso *cxMensagem;
    QMap<int, ObjetoRetencao*> mapRetencao;
};

#endif // METARETENCAO_H
