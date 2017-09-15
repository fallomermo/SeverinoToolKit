#ifndef USUARIOS_H
#define USUARIOS_H

#include <QObject>

class Usuarios : public QObject
{
    Q_OBJECT
public:
    explicit Usuarios(QObject *parent = nullptr);

    QString getUsuario() const;
    void setUsuario(const QString &value);

    QString getSenha() const;
    void setSenha(const QString &value);

signals:

public slots:

private:
    QString usuario;
    QString senha;
};

#endif // USUARIOS_H
