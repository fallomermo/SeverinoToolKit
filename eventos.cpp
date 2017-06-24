#include "eventos.h"

Eventos::Eventos(QObject *parent) : QObject(parent)
{}

QString Eventos::getID_Empresa() const
{
    return ID_Empresa;
}

void Eventos::setID_Empresa(const QString &value)
{
    ID_Empresa = value;
}

QString Eventos::getEmpresa() const
{
    return Empresa;
}

void Eventos::setEmpresa(const QString &value)
{
    Empresa = value;
}

QString Eventos::getID_Filial() const
{
    return ID_Filial;
}

void Eventos::setID_Filial(const QString &value)
{
    ID_Filial = value;
}

QString Eventos::getFilial() const
{
    return Filial;
}

void Eventos::setFilial(const QString &value)
{
    Filial = value;
}

QString Eventos::getCidadeRegiao() const
{
    return CidadeRegiao;
}

void Eventos::setCidadeRegiao(const QString &value)
{
    CidadeRegiao = value;
}

int Eventos::getCalculo() const
{
    return Calculo;
}

void Eventos::setCalculo(int value)
{
    Calculo = value;
}

QDate Eventos::getCompetencia() const
{
    return Competencia;
}

void Eventos::setCompetencia(const QDate &value)
{
    Competencia = value;
}

QString Eventos::getSetor() const
{
    return Setor;
}

void Eventos::setSetor(const QString &value)
{
    Setor = value;
}

int Eventos::getCodigoEvento() const
{
    return CodigoEvento;
}

void Eventos::setCodigoEvento(int value)
{
    CodigoEvento = value;
}

QString Eventos::getEvento() const
{
    return Evento;
}

void Eventos::setEvento(const QString &value)
{
    Evento = value;
}

int Eventos::getTipoEvento() const
{
    return TipoEvento;
}

void Eventos::setTipoEvento(int value)
{
    TipoEvento = value;
}

double Eventos::getValorEvento() const
{
    return ValorEvento;
}

void Eventos::setValorEvento(double value)
{
    ValorEvento = value;
}

double Eventos::getValorTerceiro() const
{
    return ValorTerceiro;
}

void Eventos::setValorTerceiro(double value)
{
    ValorTerceiro = value;
}

double Eventos::getValorGps() const
{
    return ValorGps;
}

void Eventos::setValorGps(double value)
{
    ValorGps = value;
}

double Eventos::getValorDeducao() const
{
    return ValorDeducao;
}

void Eventos::setValorDeducao(double value)
{
    ValorDeducao = value;
}

QString Eventos::getCnpj() const
{
    return Cnpj;
}

void Eventos::setCnpj(const QString &value)
{
    Cnpj = value;
}

int Eventos::getNumeroRegistros() const
{
    return NumeroRegistros;
}

void Eventos::setNumeroRegistros(int value)
{
    NumeroRegistros = value;
}
