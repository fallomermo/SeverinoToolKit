#include "responsavelselecaoagregado.h"

ResponsavelSelecaoAgregado::ResponsavelSelecaoAgregado(QObject *parent) : QObject(parent)
{

}

QString ResponsavelSelecaoAgregado::getReposavel() const
{
    return reposavel;
}

void ResponsavelSelecaoAgregado::setReposavel(const QString &value)
{
    reposavel = value;
}

int ResponsavelSelecaoAgregado::getNumeroAdmitidos() const
{
    return numeroAdmitidos;
}

void ResponsavelSelecaoAgregado::setNumeroAdmitidos(int value)
{
    numeroAdmitidos = value;
}

int ResponsavelSelecaoAgregado::getNumeroDemitidos() const
{
    return numeroDemitidos;
}

void ResponsavelSelecaoAgregado::setNumeroDemitidos(int value)
{
    numeroDemitidos = value;
}
