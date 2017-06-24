#ifndef CONTROLEDAO_H
#define CONTROLEDAO_H

#include <QObject>
#include <QMap>


// Inlcuindo clases manuais do projeto
#include "cadastroempresa.h"
#include "cadastrofilial.h"
#include "bancodedados.h"
#include "ecoclinicrepasses.h"
#include "cadastrocolaborador.h"


class ControleDAO : public QObject
{
    Q_OBJECT
public:
    explicit ControleDAO(QObject *parent = 0);

    bool abrirConexao();
    QMap<int, CadastroEmpresa*> getEmpresas();
    QMap<int, CadastroFilial*> getFiliais();
    QMap<int, Eventos*> getPlanoContas(QString,QString,QString,QString);
    QMap<int, Eventos*> getGuiaINSS(QString, QString, QString, QString);
    QMap<int, EcoclinicRepasses*> getPlanoSaude(QString, QString, int);
    QMap<int, CadastroColaborador*> getColaboradoresAtivos(QString, QString, QDate);

signals:
    void mensagemRetorno(QString);
    void statusProgresso(int,int);

private slots:
    void mensagemRetornoUsuario(QString);
    void statusProgressoAtual(int,int);

private:
    BancoDeDados *db;


};

#endif // CONTROLEDAO_H
