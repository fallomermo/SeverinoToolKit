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
#include "cadastrocolaborador.h"
#include "objetoretencao.h"
#include "eventos.h"
#include "objetocrednossoretorno.h"
#include "objetocadastroupdatefile.h"
#include "updatedatatablecolumm.h"
#include "bancodedados.h"
#include "sqlitedatabase.h"
#include "eventosimportadosobj.h"
#include "usuarios.h"

class ControleDAO : public QObject
{
    Q_OBJECT
public:
    explicit ControleDAO(QObject *parent = 0);
    void getPlanoSaude(QString, QString, int);
    void getMetaRetencao(QDate, QDate);
    void getCrednossoRetorno(QDate);

signals:
    void mensagemRetorno(QString, QString);
    void statusProgresso(int,int);
    void enviarStatusConexaoSenior(bool);
    void enviarStatusConexaoLocal(bool);
    void enviarUsuarios(QList<Usuarios*>);
    void retornarCadastroDeEmpresas(QMap<int, CadastroEmpresa*>);
    void retornarCadastroDeFiliais(QMap<int, CadastroFilial*>);
    void retornaEventosImportados(QList<EventosImportadosOBJ*>);
    void enviarPlanoContas(QMap<int, Eventos *>);
    void enviarGuiaINSS(QMap<int, Eventos *>);
    void enviarMetaRetencao(QMap<int, ObjetoRetencao *>);
    void enviarCrednossoRetorno(QMap<int, ObjetoCrednossoRetorno*>);
    void enviarRelacaoColaboradores(QMap<int, CadastroColaborador*>);
    void enviarUpdateDadosCadastroColaborador(UpdateDataTableColumm *);
    void enviarUpdateDadosCadastroColaborador(QMap<int, UpdateDataTableColumm *>);
    void mensagemControle(QString, QString, int);
    void checagemStmUsuario(bool);
    void statusConexaoSenior(bool);

private slots:
    void abrirConexaoSenior();
    void enviarCadastroDeEmpresas();
    void enviarCadastroDeFiliais();
    void retornaPlanoContas(QString,QString,QString,QString,int);
    void retornaGuiaINSS(QString,QString,QString,QString,int);
    void retornaRelacaoColaboradores(QString, QString, QDate);
    void retornaUsuarios();
    void mensagemRetornoUsuario(QString, QString);
    void statusProgressoAtual(int,int);
    void obterMetaRetencao(QDate, QDate);
    void obterCrednossoRetorno(QDate);
    void obterUpdateDadosCadastroColaborador(int, int, int, int, bool);
    void obterUpdateDadosCadastroColaborador(UpdateDataTableColumm*);
    void obterEventosImportados(QString, int);
    void mensagemControlador(QString, QString, int);
    void exitClass();

    // slots para gerenciamento dos usuários
    void inserirUsuario();
    void removerUsuario();
    void alterarUsuario();
    void checagemStmUsuarioStatus(bool);

private:
    BancoDeDados *db;
    SQliteDatabase *sqlitedb;


};

#endif // CONTROLEDAO_H
