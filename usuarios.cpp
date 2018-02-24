#include "usuarios.h"

Usuarios::Usuarios(QObject *parent) : QObject(parent)
{

}

QString Usuarios::getUsuario() const
{
    return usuario;
}

void Usuarios::setUsuario(const QString &value)
{
    usuario = value;
}

QString Usuarios::getSenha() const
{
    return senha;
}

void Usuarios::setSenha(const QString &value)
{
    senha = value;
}

QString Usuarios::getDescricao() const
{
    return descricao;
}

void Usuarios::setDescricao(const QString &value)
{
    descricao = value;
}

int Usuarios::getCodidoPerfil() const
{
    return codidoPerfil;
}

void Usuarios::setCodidoPerfil(int value)
{
    codidoPerfil = value;
}

QString Usuarios::getPerfil() const
{
    return perfil;
}

void Usuarios::setPerfil(const QString &value)
{
    perfil = value;
}

QDate Usuarios::getDataCriacao() const
{
    return dataCriacao;
}

void Usuarios::setDataCriacao(const QDate &value)
{
    dataCriacao = value;
}

QDate Usuarios::getDataAlteracao() const
{
    return dataAlteracao;
}

void Usuarios::setDataAlteracao(const QDate &value)
{
    dataAlteracao = value;
}

QString Usuarios::getId() const
{
    return id;
}

void Usuarios::setId(const QString &value)
{
    id = value;
}
