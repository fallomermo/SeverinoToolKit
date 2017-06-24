#include "controledao.h"

ControleDAO::ControleDAO(QObject *parent) : QObject(parent)
{
    db = new BancoDeDados(this);
    connect(db,SIGNAL(messagemRetorno(QString)), this, SLOT(mensagemRetornoUsuario(QString)));
    connect(db, SIGNAL(statusProgresso(int,int)),this, SLOT(statusProgressoAtual(int,int)));
}

bool ControleDAO::abrirConexao()
{
    db = new BancoDeDados;
    return db->abrirConexao();
}

QMap<int, CadastroEmpresa *> ControleDAO::getEmpresas()
{
    db = new BancoDeDados;
    return db->getEmpresas();
}

QMap<int, CadastroFilial *> ControleDAO::getFiliais()
{
    db = new BancoDeDados;
    return db->getFiliais();
}

QMap<int, Eventos *> ControleDAO::getPlanoContas(QString __idEmpresa, QString __idFilial, QString __InicioComp, QString __FinalComp)
{
    db = new BancoDeDados;
    return db->getPlanoContas(__idEmpresa,__idFilial,__InicioComp, __FinalComp);
}

QMap<int, Eventos *> ControleDAO::getGuiaINSS(QString __idEmpresa, QString __idFilial, QString __InicioComp, QString __FinalComp)
{
    db = new BancoDeDados;
    return db->getGuiaINSS(__idEmpresa,__idFilial,__InicioComp, __FinalComp);
}

QMap<int, EcoclinicRepasses *> ControleDAO::getPlanoSaude(QString dInicio, QString dFinal, int r)
{
    db = new BancoDeDados;
    return db->getPlanoSaude(dInicio, dFinal, r);
}

QMap<int, CadastroColaborador *> ControleDAO::getColaboradoresAtivos(QString __ID_Empresa, QString __ID_Filial, QDate __dataReferencia)
{
    db = new BancoDeDados;
    return db->getColaboradoresAtivos(__ID_Empresa, __ID_Filial, __dataReferencia);
}

void ControleDAO::mensagemRetornoUsuario(QString msg)
{
    emit mensagemRetorno(msg);
}

void ControleDAO::statusProgressoAtual(int i, int t)
{
    emit statusProgresso(i,t);
}
