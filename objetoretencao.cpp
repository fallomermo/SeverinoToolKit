#include "objetoretencao.h"

ObjetoRetencao::ObjetoRetencao(QObject *parent) : QObject(parent)
{

}

int ObjetoRetencao::getCadastro() const
{
    return cadastro;
}

void ObjetoRetencao::setCadastro(int value)
{
    cadastro = value;
}

QString ObjetoRetencao::getNome() const
{
    return nome;
}

void ObjetoRetencao::setNome(const QString &value)
{
    nome = value;
}

QDate ObjetoRetencao::getDemissao() const
{
    return demissao;
}

void ObjetoRetencao::setDemissao(const QDate &value)
{
    demissao = value;
}

QDate ObjetoRetencao::getAdmissao() const
{
    return admissao;
}

void ObjetoRetencao::setAdmissao(const QDate &value)
{
    admissao = value;
}

QString ObjetoRetencao::getCargo() const
{
    return cargo;
}

void ObjetoRetencao::setCargo(const QString &value)
{
    cargo = value;
}

QString ObjetoRetencao::getSetor() const
{
    return setor;
}

void ObjetoRetencao::setSetor(const QString &value)
{
    setor = value;
}

int ObjetoRetencao::getCodigoDaCausa() const
{
    return codigoDaCausa;
}

void ObjetoRetencao::setCodigoDaCausa(int value)
{
    codigoDaCausa = value;
}

QString ObjetoRetencao::getCausa() const
{
    return causa;
}

void ObjetoRetencao::setCausa(const QString &value)
{
    causa = value;
}

int ObjetoRetencao::getCodigoDaEmpresa() const
{
    return codigoDaEmpresa;
}

void ObjetoRetencao::setCodigoDaEmpresa(int value)
{
    codigoDaEmpresa = value;
}

int ObjetoRetencao::getCodigoDaFilial() const
{
    return codigoDaFilial;
}

void ObjetoRetencao::setCodigoDaFilial(int value)
{
    codigoDaFilial = value;
}

QString ObjetoRetencao::getFilial() const
{
    return filial;
}

void ObjetoRetencao::setFilial(const QString &value)
{
    filial = value;
}

QString ObjetoRetencao::getRecrutador() const
{
    return recrutador;
}

void ObjetoRetencao::setRecrutador(const QString &value)
{
    recrutador = value;
}

int ObjetoRetencao::getAdmitidos() const
{
    return admitidos;
}

void ObjetoRetencao::setAdmitidos(int value)
{
    admitidos = value;
}

int ObjetoRetencao::getDemitidos() const
{
    return demitidos;
}

void ObjetoRetencao::setDemitidos(int value)
{
    demitidos = value;
}
