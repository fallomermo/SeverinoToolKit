#include "cadastrofilial.h"

CadastroFilial::CadastroFilial(QObject *parent) : QObject(parent)
{}

QString CadastroFilial::getID_Empresa() const
{
    return ID_Empresa;
}

void CadastroFilial::setID_Empresa(const QString &value)
{
    ID_Empresa = value;
}

QString CadastroFilial::getID_Filial() const
{
    return ID_Filial;
}

void CadastroFilial::setID_Filial(const QString &value)
{
    ID_Filial = value;
}

QString CadastroFilial::getFilial() const
{
    return Filial;
}

void CadastroFilial::setFilial(const QString &value)
{
    Filial = value;
}

QString CadastroFilial::getCnpj() const
{
    return Cnpj;
}

void CadastroFilial::setCnpj(const QString &value)
{
    Cnpj = value;
}
