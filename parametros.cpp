#include "parametros.h"

Parametros::Parametros(QObject *parent) : QObject(parent)
{

}

QString Parametros::getUltimoUsuario() const
{
    return ultimoUsuario;
}

void Parametros::setUltimoUsuario(const QString &value)
{
    ultimoUsuario = value;
}

QString Parametros::getAvatar() const
{
    return avatar;
}

void Parametros::setAvatar(const QString &value)
{
    avatar = value;
}

QString Parametros::getSenha() const
{
    return senha;
}

void Parametros::setSenha(const QString &value)
{
    senha = value;
}
