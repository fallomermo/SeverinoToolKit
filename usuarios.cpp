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
