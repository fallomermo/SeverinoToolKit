#include "controledao.h"
#include "bancodedados.h"

ControleDAO::ControleDAO(QObject *parent) : QThread(parent)
{
    db = new BancoDeDados(nullptr);
    connect(db, SIGNAL(messagemRetorno(QString)), this, SLOT(mensagemRetornoUsuario(QString)));
    connect(db, SIGNAL(statusProgresso(int,int)), this, SLOT(statusProgressoAtual(int,int)));
}

void ControleDAO::abrirConexao()
{
    db = new BancoDeDados;
    emit enviarStatusConexao( db->abrirConexao() );
}

void ControleDAO::enviarCadastroDeEmpresas()
{
    db = new BancoDeDados;
    QMap<int, CadastroEmpresa*> __tempMap = db->getEmpresas();
    emit retornarCadastroDeEmpresas(__tempMap);
}

void ControleDAO::enviarCadastroDeFiliais()
{
    db = new BancoDeDados;
    QMap<int, CadastroFilial *> __tempMap = db->getFiliais();
    emit retornarCadastroDeFiliais(__tempMap);
}

void ControleDAO::getPlanoSaude(QString dInicio, QString dFinal, int r)
{
    obterPlanoSaude(dInicio, dFinal, r);
}

void ControleDAO::getMetaRetencao(QDate dInicio, QDate dFinal)
{
    obterMetaRetencao(dInicio, dFinal);
}

void ControleDAO::getCrednossoRetorno(QDate __competencia)
{
    this->obterCrednossoRetorno(__competencia);
}

void ControleDAO::retornaPlanoContas(QString __idEmpresa, QString __idFilial, QString __inicioComp, QString __finalComp, int __tipoCalculo)
{
    db = new BancoDeDados;
    QMap<int, Eventos *> __tempMapBase = db->getPlanoContas(__idEmpresa, __idFilial, __inicioComp, __finalComp, __tipoCalculo);
    QMapIterator<int, Eventos *> __tempIterator(__tempMapBase);
    QMap<int, Eventos *> __tempMap;
    int registroKey = 0;
    while (__tempIterator.hasNext()) {
        __tempIterator.next();
        registroKey++;
        Eventos * __novoEvento = __tempIterator.value();
        __tempMap.insert(registroKey, __novoEvento);
    }

    __tempMapBase.clear();
    __tempMapBase = db->getGuiaINSS(__idEmpresa, __idFilial, __inicioComp, __finalComp, __tipoCalculo);
    __tempIterator = QMapIterator<int, Eventos *>(__tempMapBase);
    while (__tempIterator.hasNext()) {
        __tempIterator.next();
        registroKey++;
        Eventos * __novoEvento = __tempIterator.value();
        __tempMap.insert(registroKey, __novoEvento);
    }
    emit enviarPlanoContas(__tempMap);
}

void ControleDAO::retornaGuiaINSS(QString __idEmpresa, QString __idFilial, QString __inicioComp, QString __finalComp, int __tipoCalculo)
{
    db = new BancoDeDados;
    QMap<int, Eventos *> __tempMap = db->getGuiaINSS(__idEmpresa, __idFilial, __inicioComp, __finalComp, __tipoCalculo);
    emit enviarGuiaINSS(__tempMap);
}

void ControleDAO::retornaRelacaoColaboradores(QString __ID_Empresa, QString __ID_Filial, QDate __dataReferencia)
{
    db = new BancoDeDados;
    QMap<int, CadastroColaborador*> __tempMap = db->getColaboradoresAtivos(__ID_Empresa, __ID_Filial, __dataReferencia);
    emit enviarRelacaoColaboradores(__tempMap);
}

void ControleDAO::mensagemRetornoUsuario(QString msg)
{
    emit mensagemRetorno(msg);
}

void ControleDAO::statusProgressoAtual(int i, int t)
{
    emit statusProgresso(i,t);
}

void ControleDAO::obterMetaRetencao(QDate dInicio, QDate dFinal)
{
    db = new BancoDeDados;
    QMap<int, ObjetoRetencao *> __tempMap = db->getMetaRetencao(dInicio, dFinal);
    emit enviarMetaRetencao(__tempMap);
}

void ControleDAO::obterPlanoSaude(QString dInicio, QString dFinal, int r)
{
    db = new BancoDeDados;
    QMap<int, EcoclinicRepasses*> __tempMap = db->getPlanoSaude(dInicio, dFinal, r);
    emit enviarPlanoSaude(__tempMap);
}

void ControleDAO::obterCrednossoRetorno(QDate __competencia)
{
    db = new BancoDeDados;
    QMap<int, ObjetoCrednossoRetorno *> __tempMap = db->getCrednossoRetorno(__competencia);
    emit enviarCrednossoRetorno(__tempMap);
}

void ControleDAO::obterUpdateDadosCadastroColaborador(int __empresa, int __matricula, int __linha, int __coluna, bool ok)
{
    db = new BancoDeDados;
    QMap<int, UpdateDataTableColumm *> __tempMap = db->getUpdateCadastroArquivo(__empresa, __matricula, __linha, __coluna, ok);
    emit enviarUpdateDadosCadastroColaborador(__tempMap);
}

void ControleDAO::mensagemControlador(QString titulo, QString mensagem, int t)
{
    emit mensagemControle(titulo, mensagem, t);
}

void ControleDAO::exitClass()
{
    exit(0);
}
