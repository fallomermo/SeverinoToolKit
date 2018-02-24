#ifndef SQLITEDATABASE_H
#define SQLITEDATABASE_H

#include <QDir>
#include <QObject>
#include <QSqlQuery>
#include <QFileInfo>
#include <QSqlError>
#include <QVariant>
#include <QSqlDatabase>


#include "parametros.h"
#include "usuarios.h"

class SQliteDatabase : public QObject
{
    Q_OBJECT
public:
    explicit SQliteDatabase(QObject *parent = nullptr);
    QList<Usuarios *> obterUsuarios();

signals:
    void mensagemRetorno(QString, QString);

public slots:
private slots:
    bool abrirConexao();
    Parametros* obterParametros();

private:
    QSqlDatabase db;



};

#endif // SQLITEDATABASE_H
