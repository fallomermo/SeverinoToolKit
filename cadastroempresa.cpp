#include "cadastroempresa.h"

CadastroEmpresa::CadastroEmpresa(QObject *parent) : QObject(parent)
{}

QString CadastroEmpresa::getID_Empresa() const
{
    return ID_Empresa;
}

void CadastroEmpresa::setID_Empresa(const QString &value)
{
    ID_Empresa = value;
}

QString CadastroEmpresa::getEmpresa() const
{
    return Empresa;
}

void CadastroEmpresa::setEmpresa(const QString &value)
{
    Empresa = value;
}
