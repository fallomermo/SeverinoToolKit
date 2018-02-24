#include "controledao.h"

ControleDAO::ControleDAO(QObject *parent) : QObject(parent) {}

void ControleDAO::enviarCadastroDeEmpresas()
{
    db = new BancoDeDados(Q_NULLPTR);
    QMap<int, CadastroEmpresa*> __tempMap = db->getEmpresas();
    emit retornarCadastroDeEmpresas(__tempMap);
}

void ControleDAO::enviarCadastroDeFiliais()
{
    db = new BancoDeDados(Q_NULLPTR);
    QMap<int, CadastroFilial *> __tempMap = db->getFiliais();
    emit retornarCadastroDeFiliais(__tempMap);
}

void ControleDAO::getMetaRetencao(QDate dInicio, QDate dFinal)
{
    obterMetaRetencao(dInicio, dFinal);
}

void ControleDAO::getCrednossoRetorno(QDate __competencia)
{
    this->obterCrednossoRetorno(__competencia);
}

void ControleDAO::abrirConexaoSenior()
{
    db = new BancoDeDados(Q_NULLPTR);
    bool ok = db->abrirConexao();
    if(ok)
        emit statusConexaoSenior(ok);
}

void ControleDAO::retornaPlanoContas(QString __idEmpresa, QString __idFilial, QString __inicioComp, QString __finalComp, int __tipoCalculo)
{
    db = new BancoDeDados(Q_NULLPTR);
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
    db = new BancoDeDados(Q_NULLPTR);
    QMap<int, Eventos *> __tempMap = db->getGuiaINSS(__idEmpresa, __idFilial, __inicioComp, __finalComp, __tipoCalculo);
    emit enviarGuiaINSS(__tempMap);
}

void ControleDAO::retornaRelacaoColaboradores(QString __ID_Empresa, QString __ID_Filial, QDate __dataReferencia)
{
    db = new BancoDeDados(Q_NULLPTR);
    QMap<int, CadastroColaborador*> __tempMap = db->getColaboradoresAtivos(__ID_Empresa, __ID_Filial, __dataReferencia);
    emit enviarRelacaoColaboradores(__tempMap);
}

void ControleDAO::retornaUsuarios()
{
    sqlitedb = new SQliteDatabase(Q_NULLPTR);
    emit enviarUsuarios(sqlitedb->obterUsuarios());
    this->deleteLater();
}

void ControleDAO::mensagemRetornoUsuario(QString t, QString m)
{
    emit mensagemRetorno(t, m);
}

void ControleDAO::statusProgressoAtual(int i, int t)
{
    emit statusProgresso(i,t);
}

void ControleDAO::obterMetaRetencao(QDate dInicio, QDate dFinal)
{
    db = new BancoDeDados(Q_NULLPTR);
    QMap<int, ObjetoRetencao *> __tempMap = db->getMetaRetencao(dInicio, dFinal);
    emit enviarMetaRetencao(__tempMap);
}

void ControleDAO::obterCrednossoRetorno(QDate __competencia)
{
    db = new BancoDeDados(Q_NULLPTR);
    QMap<int, ObjetoCrednossoRetorno *> __tempMap = db->getCrednossoRetorno(__competencia);
    emit enviarCrednossoRetorno(__tempMap);
}

void ControleDAO::obterUpdateDadosCadastroColaborador(int __empresa, int __matricula, int __linha, int __coluna, bool ok)
{
    db = new BancoDeDados(Q_NULLPTR);
    QMap<int, UpdateDataTableColumm *> __tempMap = db->getUpdateCadastroArquivo(__empresa, __matricula, __linha, __coluna, ok);
    emit enviarUpdateDadosCadastroColaborador(__tempMap);
}

void ControleDAO::obterUpdateDadosCadastroColaborador(UpdateDataTableColumm *p)
{
    db = new BancoDeDados(Q_NULLPTR);
    UpdateDataTableColumm *__p = db->getUpdateUnicoCadastroArquivo(p->getCodigoEmpresa(), p->getMatricula(), p->getLinha(), p->getColuna(), p->getUltimoRegistro());
    emit enviarUpdateDadosCadastroColaborador(__p);
}

void ControleDAO::obterEventosImportados(QString w, int t)
{
    db = new BancoDeDados(Q_NULLPTR);
    QList<EventosImportadosOBJ*>__l = db->getEventosImportados(w, t);
    emit retornaEventosImportados(__l);
}

void ControleDAO::mensagemControlador(QString titulo, QString mensagem, int t)
{
    emit mensagemControle(titulo, mensagem, t);
}

void ControleDAO::exitClass()
{
    exit(0);
    this->deleteLater();
}

void ControleDAO::inserirUsuario()
{
    db = new BancoDeDados(Q_NULLPTR);
    connect(db, SIGNAL(usuarioBanco(bool)), this, SLOT(checagemStmUsuarioStatus(bool)));
}

void ControleDAO::removerUsuario()
{
    db = new BancoDeDados(Q_NULLPTR);
    connect(db, SIGNAL(usuarioBanco(bool)), this, SLOT(checagemStmUsuarioStatus(bool)));
}

void ControleDAO::alterarUsuario()
{
    db = new BancoDeDados(Q_NULLPTR);
    connect(db, SIGNAL(usuarioBanco(bool)), this, SLOT(checagemStmUsuarioStatus(bool)));
}

void ControleDAO::checagemStmUsuarioStatus(bool ok)
{
    emit checagemStmUsuario(ok);
}
