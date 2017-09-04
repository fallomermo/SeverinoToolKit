#include "updatedatatablecolumm.h"

UpdateDataTableColumm::UpdateDataTableColumm(QObject *parent) : QObject(parent)
{}

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
