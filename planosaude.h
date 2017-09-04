#ifndef PLANOSAUDE_H
#define PLANOSAUDE_H

#include <QWidget>
#include <QFile>
#include <QFileDialog>
#include <QProgressDialog>
#include <QDebug>
#include <QDate>
#include <QMessageBox>


// Inlcuindo clases manuais do projeto
#include "ecoclinicrepasses.h"
#include "controledao.h"
#include "exportararquivo.h"

class CaixaMensagemProgresso;

namespace Ui {
class PlanoSaude;
}

class PlanoSaude : public QWidget
{
    Q_OBJECT

public:
    explicit PlanoSaude(QWidget *parent = 0);
    ~PlanoSaude();

    QMap<int, EcoclinicRepasses *> getMapPlanoSaude() const;
    void setMapPlanoSaude(const QMap<int, EcoclinicRepasses *> &value);

signals:
    void obterPlanoSaude(QString, QString, int);
    void minimumProgressValue(int);
    void maximumProgressValue(int);
    void progressValue(int);
    void fecharMensagemProgresso();
    void finishThread();

private slots:
    void getDatatable();
    void exportarParaExcel();
    void inserirLinhaTabela(int, int, EcoclinicRepasses*);
    void inserirItemTabela(int, int, int);
    void inserirItemTabela(int, int, double);
    void inserirItemTabela(int, int, QString);
    void inserirItemTabela(int, int, QDate);
    void filtroItemTabela(QString);
    void caixaMensagemUsuario(QString);
    void setPlanoSaude(QMap<int, EcoclinicRepasses*>);
    void preencherTabela(QMap<int, EcoclinicRepasses*>);

private:
    Ui::PlanoSaude *ui;
    ControleDAO *controle;
    CaixaMensagemProgresso *caixaDeMensagem;
    QMap<int, EcoclinicRepasses*> mapPlanoSaude;
};

#endif // PLANOSAUDE_H
