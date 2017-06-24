#include "ecoclinicrepasses.h"

EcoclinicRepasses::EcoclinicRepasses(QObject *parent) : QObject(parent)
{}

QString EcoclinicRepasses::getID_Empresa() const
{
    return ID_Empresa;
}

void EcoclinicRepasses::setID_Empresa(const QString &value)
{
    ID_Empresa = value;
}

QString EcoclinicRepasses::getEmpresa() const
{
    return Empresa;
}

void EcoclinicRepasses::setEmpresa(const QString &value)
{
    Empresa = value;
}

QString EcoclinicRepasses::getID_Filial() const
{
    return ID_Filial;
}

void EcoclinicRepasses::setID_Filial(const QString &value)
{
    ID_Filial = value;
}

QString EcoclinicRepasses::getFilial() const
{
    return Filial;
}

void EcoclinicRepasses::setFilial(const QString &value)
{
    Filial = value;
}

QString EcoclinicRepasses::getMatricula() const
{
    return Matricula;
}

void EcoclinicRepasses::setMatricula(const QString &value)
{
    Matricula = value;
}

QString EcoclinicRepasses::getCPF() const
{
    return CPF;
}

void EcoclinicRepasses::setCPF(const QString &value)
{
    CPF = value;
}

QString EcoclinicRepasses::getNome() const
{
    return Nome;
}

void EcoclinicRepasses::setNome(const QString &value)
{
    Nome = value;
}

QString EcoclinicRepasses::getCargo() const
{
    return Cargo;
}

void EcoclinicRepasses::setCargo(const QString &value)
{
    Cargo = value;
}

QString EcoclinicRepasses::getID_Sindicato() const
{
    return ID_Sindicato;
}

void EcoclinicRepasses::setID_Sindicato(const QString &value)
{
    ID_Sindicato = value;
}

QString EcoclinicRepasses::getSetor() const
{
    return Setor;
}

void EcoclinicRepasses::setSetor(const QString &value)
{
    Setor = value;
}

QString EcoclinicRepasses::getSexo() const
{
    return Sexo;
}

void EcoclinicRepasses::setSexo(const QString &value)
{
    Sexo = value;
}

QString EcoclinicRepasses::getDdd1() const
{
    return Ddd1;
}

void EcoclinicRepasses::setDdd1(const QString &value)
{
    Ddd1 = value;
}

QString EcoclinicRepasses::getNumero1() const
{
    return Numero1;
}

void EcoclinicRepasses::setNumero1(const QString &value)
{
    Numero1 = value;
}

QString EcoclinicRepasses::getDdd2() const
{
    return Ddd2;
}

void EcoclinicRepasses::setDdd2(const QString &value)
{
    Ddd2 = value;
}

QString EcoclinicRepasses::getNumero2() const
{
    return Numero2;
}

void EcoclinicRepasses::setNumero2(const QString &value)
{
    Numero2 = value;
}

QString EcoclinicRepasses::getRegistroGeralIdentidade() const
{
    return RegistroGeralIdentidade;
}

void EcoclinicRepasses::setRegistroGeralIdentidade(const QString &value)
{
    RegistroGeralIdentidade = value;
}

QString EcoclinicRepasses::getOrgaoEmissor() const
{
    return OrgaoEmissor;
}

void EcoclinicRepasses::setOrgaoEmissor(const QString &value)
{
    OrgaoEmissor = value;
}

QString EcoclinicRepasses::getUF_Emissor() const
{
    return UF_Emissor;
}

void EcoclinicRepasses::setUF_Emissor(const QString &value)
{
    UF_Emissor = value;
}

QDate EcoclinicRepasses::getDataEmissao() const
{
    return DataEmissao;
}

void EcoclinicRepasses::setDataEmissao(const QDate &value)
{
    DataEmissao = value;
}

QDate EcoclinicRepasses::getDataAdmissao() const
{
    return DataAdmissao;
}

void EcoclinicRepasses::setDataAdmissao(const QDate &value)
{
    DataAdmissao = value;
}

QDate EcoclinicRepasses::getCompetencia() const
{
    return Competencia;
}

void EcoclinicRepasses::setCompetencia(const QDate &value)
{
    Competencia = value;
}

int EcoclinicRepasses::getID_Evento() const
{
    return ID_Evento;
}

void EcoclinicRepasses::setID_Evento(int value)
{
    ID_Evento = value;
}

QString EcoclinicRepasses::getEvento() const
{
    return Evento;
}

void EcoclinicRepasses::setEvento(const QString &value)
{
    Evento = value;
}

double EcoclinicRepasses::getValor() const
{
    return Valor;
}

void EcoclinicRepasses::setValor(double value)
{
    Valor = value;
}
