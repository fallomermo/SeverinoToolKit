#include "sqlitedatabase.h"

SQliteDatabase::SQliteDatabase(QObject *parent) : QObject(parent)
{
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    this->abrirConexao();
}

bool SQliteDatabase::abrirConexao()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    QString localDatabse = QDir::currentPath()+"/database/svr.sqlite";
    QFileInfo f(localDatabse);
    bool ok = false;

    if(f.isFile()) {
        db.setDatabaseName(f.absoluteFilePath());
        if(db.open())
            ok = true;
    }
    return ok;
}

Parametros *SQliteDatabase::obterParametros()
{
    Parametros *p = new Parametros(Q_NULLPTR);
    QString c = QString("SELECT ultuse, avatar, savpas FROM Parametros");
    QSqlQuery q(c);
    q.setForwardOnly(true);
    if(!q.exec()) { emit mensagemRetorno(QString("Erro de Conexão"), q.lastError().text()); return p;}
    else {
        while (q.next()) {
            Parametros *p = new Parametros(Q_NULLPTR);
            p->setUltimoUsuario(q.value("ultuse").toString());
            p->setAvatar(q.value("avatar").toString());
            p->setSenha(q.value("savpas").toString());
        }
    }
    return p;
}

QList<Usuarios *> SQliteDatabase::obterUsuarios()
{
    QList<Usuarios *> l;
    const QString c = QString("SELECT u.loguse, u.pasuse, u.desuse, u.datcri, u.datalt, u.codper, p.desper "
                        "FROM Usuarios u "
                        "LEFT JOIN Perfil p "
                        "ON p.codper = u.codper");
    QSqlQuery q(c);
    q.setForwardOnly(true);
    if(!q.exec()) {
        emit mensagemRetorno(QString("Erro de Conexão"), q.lastError().text()); return l;
    }
    else {
        int pos = 0;
        while (q.next()) {
            pos++;
            Usuarios *u = new Usuarios(Q_NULLPTR);
            u->setUsuario(q.value("loguse").toString());
            u->setSenha(q.value("pasuse").toString());
            u->setDescricao(q.value("desuse").toString());
            u->setDataCriacao(q.value("datcri").toDate());
            u->setDataAlteracao(q.value("datalt").toDate());
            u->setCodidoPerfil(q.value("codper").toInt(Q_NULLPTR));
            u->setPerfil(q.value("desper").toString());
            l.insert(pos, u);
        }
    }
    return l;
}
