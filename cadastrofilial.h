#ifndef CADASTROFILIAL_H
#define CADASTROFILIAL_H

#include <QObject>


// Inlcuindo clases manuais do projeto


class CadastroFilial : public QObject
{
    Q_OBJECT
public:
    explicit CadastroFilial(QObject *parent = 0);

    QString getID_Empresa() const;
    void setID_Empresa(const QString &value);

    QString getID_Filial() const;
    void setID_Filial(const QString &value);

    QString getFilial() const;
    void setFilial(const QString &value);

    QString getCnpj() const;
    void setCnpj(const QString &value);

signals:

public slots:

private:
    QString ID_Empresa;
    QString ID_Filial;
    QString Filial;
    QString Cnpj;
};

#endif // CADASTROFILIAL_H
