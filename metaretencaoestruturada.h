#ifndef METARETENCAOESTRUTURADA_H
#define METARETENCAOESTRUTURADA_H

#include <QMap>
#include <QDate>
#include <QDebug>
#include <QMovie>
#include <QWidget>
#include <QThread>
#include <QPixmap>
#include <QLocale>
#include <QPainter>
#include <QFileInfo>
#include <QChartView>
#include <QPieSeries>
#include <QModelIndex>
#include <QFileDialog>
#include <QMessageBox>
#include <QToolButton>
#include <QMapIterator>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsWidget>
#include <QGraphicsEffect>
#include <QGraphicsEllipseItem>
#include <QStyleOptionGraphicsItem>


#include "controledao.h"
#include "qcustomplot.h"
#include "objetoretencao.h"
#include "exportararquivo.h"
#include "detalhesretencao.h"
#include "responsavelselecaoagregado.h"
#include "donutbreakdownchart.h"

class QCustomPlot;

class QGraphicsRectWidget : public QGraphicsWidget
{
public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
               QWidget *) override
    {
        painter->fillRect(rect(), Qt::blue);
    }
};

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
    void caixaMensagemUsuario(QString);
    void atualizarResultados(QModelIndex);
    void setRetencao(const QMap<int, ObjetoRetencao *> &value);
    void removerItemTabela();
    void removerItemTabela(const QWidget *);
    void detalhesRetencao();
    void detalhesRetencao(QModelIndex);
    QMap<QString, ResponsavelSelecaoAgregado*> agregarValores(const QMap<int, ObjetoRetencao*>);
    void updateDadosGrafico();
    void girarEtiquetas(int);
    void imprimirPlotagemGrafico();
    void salvarImagemGrafico();

private:
    Ui::MetaRetencaoEstruturada *ui;
    DetalhesRetencao *detalhes;

    ControleDAO *controle;
    QMap<int, ObjetoRetencao*> mapRetencao;
    CaixaMensagemProgresso *caixaMensagem;

    enum {
        RECRUTA, ADMITIDOS, DEMITIDOS, RETENCAO, ACOES
    };

};

#endif // METARETENCAOESTRUTURADA_H
