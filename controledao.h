#ifndef CONTROLEDAO_H
#define CONTROLEDAO_H

#include <QObject>
#include <QMessageBox>
#include <QApplication>
#include <QThread>
#include <QMap>
#include <QMapIterator>

// Inlcuindo clases manuais do projeto
#include "cadastroempresa.h"
#include "cadastrofilial.h"
#include "ecoclinicrepasses.h"
#include "cadastrocolaborador.h"
#include "objetoretencao.h"
#include "eventos.h"
#include "objetocrednossoretorno.h"
#include "objetocadastroupdatefile.h"
#include "updatedatatablecolumm.h"
#include "bancodedados.h"

class ControleDAO : public QObject
{
    Q_OBJECT
public:
    explicit ControleDAO(QObject *parent = 0);
    void getPlanoSaude(QString, QString, int);
    void getMetaRetencao(QDate, QDate);
    void getCrednossoRetorno(QDate);

signals:
    void mensagemRetorno(QString);
    void statusProgresso(int,int);
    void enviarStatusConexao(bool);
    void retornarCadastroDeEmpresas(QMap<int, CadastroEmpresa*>);
    void retornarCadastroDeFiliais(QMap<int, CadastroFilial*>);
    void enviarPlanoContas(QMap<int, Eventos *>);
    void enviarGuiaINSS(QMap<int, Eventos *>);
    void enviarMetaRetencao(QMap<int, ObjetoRetencao *>);
    void enviarPlanoSaude(QMap<int, EcoclinicRepasses*>);
    void enviarCrednossoRetorno(QMap<int, ObjetoCrednossoRetorno*>);
    void enviarRelacaoColaboradores(QMap<int, CadastroColaborador*>);
    void enviarUpdateDadosCadastroColaborador(QMap<int, UpdateDataTableColumm *>);
    void finishThread();
    void mensagemControle(QString, QString, int);

private slots:
    void abrirConexao();
    void enviarCadastroDeEmpresas();
    void enviarCadastroDeFiliais();
    void retornaPlanoContas(QString,QString,QString,QString,int);
    void retornaGuiaINSS(QString,QString,QString,QString,int);
    void retornaRelacaoColaboradores(QString, QString, QDate);
    void mensagemRetornoUsuario(QString);
    void statusProgressoAtual(int,int);
    void obterMetaRetencao(QDate, QDate);
    void obterPlanoSaude(QString, QString, int);
    void obterCrednossoRetorno(QDate);
    void obterUpdateDadosCadastroColaborador(int, int, int, int, bool);
    void obterUpdateDadosCadastroColaborador(UpdateDataTableColumm*);
    void mensagemControlador(QString, QString, int);
    void exitClass();

private:
    BancoDeDados *db;


};

#endif // CONTROLEDAO_H
