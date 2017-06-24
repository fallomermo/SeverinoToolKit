#ifndef CADASTROEMPRESA_H
#define CADASTROEMPRESA_H

#include <QObject>

// Inlcuindo clases manuais do projeto


class CadastroEmpresa : public QObject
{
    Q_OBJECT
public:
    explicit CadastroEmpresa(QObject *parent = 0);

    QString getID_Empresa() const;
    void setID_Empresa(const QString &value);

    QString getEmpresa() const;
    void setEmpresa(const QString &value);

signals:

public slots:

private:
    QString ID_Empresa;
    QString Empresa;

};

#endif // CADASTROEMPRESA_H
