#include "eventosimportadosobj.h"

EventosImportadosOBJ::EventosImportadosOBJ(QObject *parent) : QObject(parent)
{

}

QString EventosImportadosOBJ::getEmpresa() const
{
    return empresa;
}

void EventosImportadosOBJ::setEmpresa(const QString &value)
{
    empresa = value;
}

QString EventosImportadosOBJ::getFilial() const
{
    return filial;
}

void EventosImportadosOBJ::setFilial(const QString &value)
{
    filial = value;
}

QString EventosImportadosOBJ::getMatricula() const
{
    return matricula;
}

void EventosImportadosOBJ::setMatricula(const QString &value)
{
    matricula = value;
}

QString EventosImportadosOBJ::getNome() const
{
    return nome;
}

void EventosImportadosOBJ::setNome(const QString &value)
{
    nome = value;
}

QString EventosImportadosOBJ::getCpf() const
{
    return cpf;
}

void EventosImportadosOBJ::setCpf(const QString &value)
{
    cpf = value;
}

QString EventosImportadosOBJ::getSindicato() const
{
    return sindicato;
}

void EventosImportadosOBJ::setSindicato(const QString &value)
{
    sindicato = value;
}

QString EventosImportadosOBJ::getEvento() const
{
    return evento;
}

void EventosImportadosOBJ::setEvento(const QString &value)
{
    evento = value;
}

QString EventosImportadosOBJ::getReferencia() const
{
    return referencia;
}

void EventosImportadosOBJ::setReferencia(const QString &value)
{
    referencia = value;
}

QString EventosImportadosOBJ::getValor() const
{
    return valor;
}

void EventosImportadosOBJ::setValor(const QString &value)
{
    valor = value;
}

QString EventosImportadosOBJ::getOrigem() const
{
    return origem;
}

void EventosImportadosOBJ::setOrigem(const QString &value)
{
    origem = value;
}
