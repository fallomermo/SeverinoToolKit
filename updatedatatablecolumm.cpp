#include "updatedatatablecolumm.h"

UpdateDataTableColumm::UpdateDataTableColumm(QObject *parent) : QObject(parent) {}

int UpdateDataTableColumm::getLinha() const
{
    return linha;
}

void UpdateDataTableColumm::setLinha(int value)
{
    linha = value;
}

int UpdateDataTableColumm::getColuna() const
{
    return coluna;
}

void UpdateDataTableColumm::setColuna(int value)
{
    coluna = value;
}

int UpdateDataTableColumm::getCodigoEmpresa() const
{
    return codigoEmpresa;
}

void UpdateDataTableColumm::setCodigoEmpresa(int value)
{
    codigoEmpresa = value;
}

void UpdateDataTableColumm::setMatricula(int value)
{
    matricula = value;
}

QString UpdateDataTableColumm::getNome() const
{
    return nome;
}

void UpdateDataTableColumm::setNome(const QString &value)
{
    nome = value;
}

int UpdateDataTableColumm::getCodigoFilial() const
{
    return codigoFilial;
}

void UpdateDataTableColumm::setCodigoFilial(int value)
{
    codigoFilial = value;
}

QString UpdateDataTableColumm::getFilial() const
{
    return filial;
}

void UpdateDataTableColumm::setFilial(const QString &value)
{
    filial = value;
}

bool UpdateDataTableColumm::getUltimoRegistro() const
{
    return ultimoRegistro;
}

void UpdateDataTableColumm::setUltimoRegistro(bool value)
{
    ultimoRegistro = value;
}

int UpdateDataTableColumm::getMatricula() const
{
    return matricula;
}

