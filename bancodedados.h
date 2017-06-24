#ifndef BANCODEDADOS_H
#define BANCODEDADOS_H

#include <QMap>
#include <QDebug>
#include <QObject>
#include <QVariant>
#include <QMultiMap>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlResult>
#include <QSqlDatabase>

// Inlcuindo clases manuais do projeto
#include "cadastroempresa.h"
#include "cadastrofilial.h"
#include "ecoclinicrepasses.h"
#include "eventos.h"
#include "cadastrocolaborador.h"

class BancoDeDados : public QObject
{
    Q_OBJECT
public:
    explicit BancoDeDados(QObject *parent = 0);
    bool abrirConexao();
    QMap<int, CadastroEmpresa*> getEmpresas();
    QMap<int, CadastroFilial*> getFiliais();
    QMap<int, Eventos*> getPlanoContas(QString, QString, QString, QString);
    QMap<int, EcoclinicRepasses*> getPlanoSaude(QString, QString, int);
    QMap<int, Eventos*> getGuiaINSS(QString, QString, QString, QString);
    QMap<int, Eventos*> atualizarSaldosRateioGuia(QMap<int, Eventos*>, double);
    QMap<int, Eventos*> processarSaldosRegistros(QMap<QString, QMap<int, Eventos*>>);
    QMap<int, CadastroColaborador*> getColaboradoresAtivos(QString, QString, QDate);

    double getSaldoRateioNegativo() const;
    void setSaldoRateioNegativo(double value);

    int getDivisorSaldo() const;
    void setDivisorSaldo(int value);

signals:
    void messagemRetorno(QString);
    int statusProgresso(int,int);

private slots:
    void statusProgressoAtual(int,int);

private:
    Eventos *evt;
    CadastroEmpresa *_cemp;
    CadastroFilial *_cfil;
    double saldoRateioNegativo;
    int divisorSaldo;
};

#endif // BANCODEDADOS_H
