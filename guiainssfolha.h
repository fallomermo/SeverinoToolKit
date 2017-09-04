#ifndef GUIAINSSFOLHA_H
#define GUIAINSSFOLHA_H

#include <QWidget>
#include <QMap>
#include <QProgressDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QSharedPointer>
#include <QMovie>


// Inlcuindo clases manuais do projeto
#include "cadastroempresa.h"
#include "cadastrofilial.h"
#include "pesquisar.h"
#include "eventos.h"
#include "controledao.h"
#include "exportararquivo.h"
#include "caixamensagemprogresso.h"


namespace Ui {
class GuiaINSSFolha;
}

class GuiaINSSFolha : public QWidget
{
    Q_OBJECT

public:
    explicit GuiaINSSFolha(QWidget *parent = 0);
    GuiaINSSFolha(QWidget *parent, QMap<int, CadastroEmpresa *> ce, QMap<int, CadastroFilial *> cf);
    ~GuiaINSSFolha();

    QMap<int, CadastroEmpresa *> getMapEmpresas() const;
    void setMapEmpresas(const QMap<int, CadastroEmpresa *> &value);

    QMap<int, CadastroFilial *> getMapFiliais() const;
    void setMapFiliais(const QMap<int, CadastroFilial *> &value);

    QString getCodigoEmpresaAtivo() const;
    void setCodigoEmpresaAtivo(const QString &value);

    QString getCodigoFilialAtivo() const;
    void setCodigoFilialAtivo(const QString &value);

signals:
    void obterGuiaINSS(QString,QString,QString,QString,int);
    void finishThread();
    void setProgressValue(int);
    void minimumProgressValue(int);
    void maximumProgressValue(int);
    void fecharCaixaMensagem();

private slots:
    void filtroItemTabela(QString);
    void focusPeriodoInicial();
    void focusPeriodoFinal();
    void pesquisarEmpresa();
    void setEmpresa(QString);
    void pesquisarFilial();
    void setFilial(QString);
    void retornaCadastroEmpresa();
    void retornaCadastroFilial();
    void retornaCadastroFilial(QString);
    void getDatatable();
    void preencherTabela(QMap<int, Eventos*>);
    void inserirItemTabela(int,int,QString);
    void inserirItemTabela(int,int,QDate);
    void inserirItemTabela(int, int, double);
    void inserirItemTabela(int,int,int);
    void inserirLinhaTabela(int, int, Eventos *);
    void exportarParaExcel();
    void caixaMensagemUsuario(QString);

private:
    Ui::GuiaINSSFolha *ui;
    QMap<int, CadastroEmpresa *> mapEmpresas;
    QMap<int, CadastroFilial *> mapFiliais;
    ControleDAO *controle;

    QString CodigoEmpresaAtivo;
    QString CodigoFilialAtivo;
};

#endif // GUIAINSSFOLHA_H
