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


namespace Ui {
class PlanoSaude;
}

class PlanoSaude : public QWidget
{
    Q_OBJECT

public:
    explicit PlanoSaude(QWidget *parent = 0);
    ~PlanoSaude();

private slots:
    void getDatatable();
    void exportarParaExcel();
    void inserirLinhaTabela(int, int, EcoclinicRepasses*);
    void inserirItemTabela(int, int, int);
    void inserirItemTabela(int, int, double);
    void inserirItemTabela(int, int, QString);
    void inserirItemTabela(int, int, QDate);
    void filtroItemTabela(QString);

private:
    Ui::PlanoSaude *ui;
    ControleDAO *controle;
};

#endif // PLANOSAUDE_H
