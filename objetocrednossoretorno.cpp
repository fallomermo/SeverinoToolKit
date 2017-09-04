#include "objetocrednossoretorno.h"

ObjetoCrednossoRetorno::ObjetoCrednossoRetorno(QObject *parent) : QObject(parent)
{

}

int ObjetoCrednossoRetorno::getCodigoDaEmpresa() const
{
    return CodigoDaEmpresa;
}

void ObjetoCrednossoRetorno::setCodigoDaEmpresa(int value)
{
    CodigoDaEmpresa = value;
}

int ObjetoCrednossoRetorno::getCPF() const
{
    return CPF;
}

void ObjetoCrednossoRetorno::setCPF(int value)
{
    CPF = value;
}

QString ObjetoCrednossoRetorno::getEmpresa() const
{
    return Empresa;
}

void ObjetoCrednossoRetorno::setEmpresa(const QString &value)
{
    Empresa = value;
}

int ObjetoCrednossoRetorno::getCodigoDaFilial() const
{
    return CodigoDaFilial;
}

void ObjetoCrednossoRetorno::setCodigoDaFilial(int value)
{
    CodigoDaFilial = value;
}

QString ObjetoCrednossoRetorno::getFilial() const
{
    return Filial;
}

void ObjetoCrednossoRetorno::setFilial(const QString &value)
{
    Filial = value;
}

QString ObjetoCrednossoRetorno::getCNPJ() const
{
    return CNPJ;
}

void ObjetoCrednossoRetorno::setCNPJ(const QString &value)
{
    CNPJ = value;
}

int ObjetoCrednossoRetorno::getMatricula() const
{
    return Matricula;
}

void ObjetoCrednossoRetorno::setMatricula(int value)
{
    Matricula = value;
}

QString ObjetoCrednossoRetorno::getNome() const
{
    return Nome;
}

void ObjetoCrednossoRetorno::setNome(const QString &value)
{
    Nome = value;
}

int ObjetoCrednossoRetorno::getCodigoDoEvento() const
{
    return CodigoDoEvento;
}

void ObjetoCrednossoRetorno::setCodigoDoEvento(int value)
{
    CodigoDoEvento = value;
}

QString ObjetoCrednossoRetorno::getEvento() const
{
    return Evento;
}

void ObjetoCrednossoRetorno::setEvento(const QString &value)
{
    Evento = value;
}

double ObjetoCrednossoRetorno::getValor() const
{
    return Valor;
}

void ObjetoCrednossoRetorno::setValor(double value)
{
    Valor = value;
}
