#include "cadastrocolaborador.h"

CadastroColaborador::CadastroColaborador(QObject *parent) : QObject(parent)
{}

QString CadastroColaborador::getCodigoDaEmpresa() const
{
    return CodigoDaEmpresa;
}

void CadastroColaborador::setCodigoDaEmpresa(const QString &value)
{
    CodigoDaEmpresa = value;
}

QString CadastroColaborador::getEmpresa() const
{
    return Empresa;
}

void CadastroColaborador::setEmpresa(const QString &value)
{
    Empresa = value;
}

QString CadastroColaborador::getCodigoDaFilial() const
{
    return CodigoDaFilial;
}

void CadastroColaborador::setCodigoDaFilial(const QString &value)
{
    CodigoDaFilial = value;
}

QString CadastroColaborador::getFilial() const
{
    return Filial;
}

void CadastroColaborador::setFilial(const QString &value)
{
    Filial = value;
}

QString CadastroColaborador::getCidadeRegiao() const
{
    return CidadeRegiao;
}

void CadastroColaborador::setCidadeRegiao(const QString &value)
{
    CidadeRegiao = value;
}

QString CadastroColaborador::getCNPJ() const
{
    return CNPJ;
}

void CadastroColaborador::setCNPJ(const QString &value)
{
    CNPJ = value;
}

QString CadastroColaborador::getMatricula() const
{
    return Matricula;
}

void CadastroColaborador::setMatricula(const QString &value)
{
    Matricula = value;
}

QString CadastroColaborador::getCPF() const
{
    return CPF;
}

void CadastroColaborador::setCPF(const QString &value)
{
    CPF = value;
}

QString CadastroColaborador::getPIS() const
{
    return PIS;
}

void CadastroColaborador::setPIS(const QString &value)
{
    PIS = value;
}

QString CadastroColaborador::getNome() const
{
    return Nome;
}

void CadastroColaborador::setNome(const QString &value)
{
    Nome = value;
}

QDate CadastroColaborador::getDataDeAdmissao() const
{
    return DataDeAdmissao;
}

void CadastroColaborador::setDataDeAdmissao(const QDate &value)
{
    DataDeAdmissao = value;
}

QDate CadastroColaborador::getDataDeNascimento() const
{
    return DataDeNascimento;
}

void CadastroColaborador::setDataDeNascimento(const QDate &value)
{
    DataDeNascimento = value;
}

QString CadastroColaborador::getCodigoDeVinculo() const
{
    return CodigoDeVinculo;
}

void CadastroColaborador::setCodigoDeVinculo(const QString &value)
{
    CodigoDeVinculo = value;
}

QString CadastroColaborador::getTabelaDeOrganograma() const
{
    return TabelaDeOrganograma;
}

void CadastroColaborador::setTabelaDeOrganograma(const QString &value)
{
    TabelaDeOrganograma = value;
}

QString CadastroColaborador::getNumeroDoLocal() const
{
    return NumeroDoLocal;
}

void CadastroColaborador::setNumeroDoLocal(const QString &value)
{
    NumeroDoLocal = value;
}

QString CadastroColaborador::getHierarquiaDeLocal() const
{
    return HierarquiaDeLocal;
}

void CadastroColaborador::setHierarquiaDeLocal(const QString &value)
{
    HierarquiaDeLocal = value;
}

QString CadastroColaborador::getSetor() const
{
    return Setor;
}

void CadastroColaborador::setSetor(const QString &value)
{
    Setor = value;
}

QString CadastroColaborador::getEstruturaDeCargos() const
{
    return EstruturaDeCargos;
}

void CadastroColaborador::setEstruturaDeCargos(const QString &value)
{
    EstruturaDeCargos = value;
}

QString CadastroColaborador::getCodigoDoCargo() const
{
    return CodigoDoCargo;
}

void CadastroColaborador::setCodigoDoCargo(const QString &value)
{
    CodigoDoCargo = value;
}

QString CadastroColaborador::getCargo() const
{
    return Cargo;
}

void CadastroColaborador::setCargo(const QString &value)
{
    Cargo = value;
}

int CadastroColaborador::getTipoDeSalario() const
{
    return TipoDeSalario;
}

void CadastroColaborador::setTipoDeSalario(int value)
{
    TipoDeSalario = value;
}

double CadastroColaborador::getSalario() const
{
    return Salario;
}

void CadastroColaborador::setSalario(double value)
{
    Salario = value;
}
