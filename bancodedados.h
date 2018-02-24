#ifndef BANCODEDADOS_H
#define BANCODEDADOS_H

#include <QMap>
#include <QDir>
#include <QDate>
#include <QDebug>
#include <QThread>
#include <QObject>
#include <QVariant>
#include <QFileInfo>
#include <QMultiMap>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlResult>
#include <QSqlDatabase>

// Inlcuindo clases manuais do projeto
#include "cadastroempresa.h"
#include "cadastrofilial.h"
#include "eventos.h"
#include "cadastrocolaborador.h"
#include "objetoretencao.h"
#include "objetocrednossoretorno.h"
#include "objetocadastroupdatefile.h"
#include "updatedatatablecolumm.h"
#include "eventosimportadosobj.h"


class BancoDeDados : public QObject
{
    Q_OBJECT
public:
    explicit BancoDeDados(QObject *parent = 0);
    bool abrirConexao();
    QMap<int, CadastroEmpresa*> getEmpresas();
    QMap<int, CadastroFilial*> getFiliais();
    QMap<int, Eventos*> getPlanoContas(QString, QString, QString, QString, int);
    QMap<int, Eventos*> getGuiaINSS(QString, QString, QString, QString, int);
    QMap<int, Eventos*> atualizarSaldosRateioGuia(QMap<int, Eventos*>, double);
    QMap<int, Eventos*> processarSaldosRegistros(QMap<QString, QMap<int, Eventos*>>);
    QMap<int, ObjetoRetencao*> getMetaRetencao(QDate, QDate);
    QMap<int, CadastroColaborador*> getColaboradoresAtivos(QString, QString, QDate);
    QMap<int, ObjetoCrednossoRetorno *> getCrednossoRetorno(QDate);
    QMap<int, UpdateDataTableColumm*> getUpdateCadastroArquivo(int, int, int, int, bool);
    UpdateDataTableColumm* getUpdateUnicoCadastroArquivo(int, int, int, int, bool);
    QList<EventosImportadosOBJ*> getEventosImportados(QString, int);

    double getSaldoRateioNegativo() const;
    void setSaldoRateioNegativo(double value);

    int getDivisorSaldo() const;
    void setDivisorSaldo(int value);

signals:
    void mensagemRetorno(QString, QString);
    int statusProgresso(int,int);
    void usuarioBanco(bool);
    void statusConexao(bool);

public slots:
    void statusProgressoAtual(int,int);

private:
    Eventos *evt;
    CadastroEmpresa *_cemp;
    CadastroFilial *_cfil;
    double saldoRateioNegativo;
    int divisorSaldo;

    QSqlDatabase db;
};

#endif // BANCODEDADOS_H
