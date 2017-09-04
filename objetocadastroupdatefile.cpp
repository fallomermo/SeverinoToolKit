#include "objetocadastroupdatefile.h"

ObjetoCadastroUpdateFile::ObjetoCadastroUpdateFile(QObject *parent) : QObject(parent)
{

}

int ObjetoCadastroUpdateFile::getChaveKey() const
{
    return chaveKey;
}

void ObjetoCadastroUpdateFile::setChaveKey(int value)
{
    chaveKey = value;
}

int ObjetoCadastroUpdateFile::getCodigoDaEmpresa() const
{
    return codigoDaEmpresa;
}

void ObjetoCadastroUpdateFile::setCodigoDaEmpresa(int value)
{
    codigoDaEmpresa = value;
}

int ObjetoCadastroUpdateFile::getMatricula() const
{
    return matricula;
}

void ObjetoCadastroUpdateFile::setMatricula(int value)
{
    matricula = value;
}

int ObjetoCadastroUpdateFile::getCPF() const
{
    return CPF;
}

void ObjetoCadastroUpdateFile::setCPF(int value)
{
    CPF = value;
}

QString ObjetoCadastroUpdateFile::getNome() const
{
    return Nome;
}

void ObjetoCadastroUpdateFile::setNome(const QString &value)
{
    Nome = value;
}

int ObjetoCadastroUpdateFile::getCodigoDaFilial() const
{
    return codigoDaFilial;
}

void ObjetoCadastroUpdateFile::setCodigoDaFilial(int value)
{
    codigoDaFilial = value;
}

QString ObjetoCadastroUpdateFile::getFilial() const
{
    return filial;
}

void ObjetoCadastroUpdateFile::setFilial(const QString &value)
{
    filial = value;
}

int ObjetoCadastroUpdateFile::getCodigoDaSituacao() const
{
    return codigoDaSituacao;
}

void ObjetoCadastroUpdateFile::setCodigoDaSituacao(int value)
{
    codigoDaSituacao = value;
}

int ObjetoCadastroUpdateFile::getCodidoDaCausa() const
{
    return codidoDaCausa;
}

void ObjetoCadastroUpdateFile::setCodidoDaCausa(int value)
{
    codidoDaCausa = value;
}
