#ifndef PARAMETROS_H
#define PARAMETROS_H

#include <QObject>

class Parametros : public QObject
{
    Q_OBJECT
public:
    explicit Parametros(QObject *parent = nullptr);

    QString getUltimoUsuario() const;
    void setUltimoUsuario(const QString &value);

    QString getAvatar() const;
    void setAvatar(const QString &value);

    QString getSenha() const;
    void setSenha(const QString &value);

private:
    QString ultimoUsuario;
    QString avatar;
    QString senha;
};

#endif // PARAMETROS_H
