#ifndef ECOCLINIC_H
#define ECOCLINIC_H

#include <QWidget>

#include "controledao.h"
#include "ecoclinicrepasses.h"
#include "caixamensagemprogresso.h"

namespace Ui {
class Ecoclinic;
}

class Ecoclinic : public QWidget
{
    Q_OBJECT

public:
    explicit Ecoclinic(QWidget *parent = 0);
    ~Ecoclinic();

signals:
    void finishThread();
    void cancelarProcesso();
    void obterPlanoSaude(QString, QString, int);
    void setMinimumValue(int);
    void setMaximumValue(int);
    void setProgressValue(int);
    void fecharCaixaDeMensagem();

private slots:
    void processarRequisicao();
    void processarRepasses(QMap<int, EcoclinicRepasses*>);
    void cancelarOperacao();
    void inserirLinhaTabela(int, int, EcoclinicRepasses*);
    void inserirItemTabela(int, int, QString);
    void inserirItemTabela(int, int, QDate);
    void inserirItemTabela(int, int, double);
    void inserirItemTabela(int, int, int);
    void exportarParaExcel();

private:
    Ui::Ecoclinic *ui;
    ControleDAO *controle;
    QThread *threadProcesso;
    EcoclinicRepasses *repasse;
};

#endif // ECOCLINIC_H
