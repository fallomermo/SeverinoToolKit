#ifndef BANCODEDADOS_H
#define BANCODEDADOS_H

#include <QMap>
#include <QDate>
#include <QDebug>
#include <QThread>
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
#include "objetoretencao.h"
#include "objetocrednossoretorno.h"
#include "objetocadastroupdatefile.h"
#include "updatedatatablecolumm.h"

class BancoDeDados : public QObject
{
    Q_OBJECT
public:
    explicit BancoDeDados(QObject *parent = 0);
    bool abrirConexao();
    QMap<int, CadastroEmpresa*> getEmpresas();
    QMap<int, CadastroFilial*> getFiliais();
    QMap<int, Eventos*> getPlanoContas(QString, QString, QString, QString, int);
    QMap<int, EcoclinicRepasses*> getPlanoSaude(QString, QString, int);
    QMap<int, Eventos*> getGuiaINSS(QString, QString, QString, QString, int);
    QMap<int, Eventos*> atualizarSaldosRateioGuia(QMap<int, Eventos*>, double);
    QMap<int, Eventos*> processarSaldosRegistros(QMap<QString, QMap<int, Eventos*>>);
    QMap<int, ObjetoRetencao*> getMetaRetencao(QDate, QDate);
    QMap<int, CadastroColaborador*> getColaboradoresAtivos(QString, QString, QDate);
    QMap<int, ObjetoCrednossoRetorno *> getCrednossoRetorno(QDate);
    QMap<int, UpdateDataTableColumm*> getUpdateCadastroArquivo(int, int, int, int, bool);

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
