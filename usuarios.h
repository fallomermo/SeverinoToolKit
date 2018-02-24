#ifndef USUARIOS_H
#define USUARIOS_H

#include <QObject>
#include <QDate>

class Usuarios : public QObject
{
    Q_OBJECT
public:
    explicit Usuarios(QObject *parent = nullptr);

    QString getUsuario() const;
    void setUsuario(const QString &value);

    QString getSenha() const;
    void setSenha(const QString &value);

    QString getDescricao() const;
    void setDescricao(const QString &value);

    int getCodidoPerfil() const;
    void setCodidoPerfil(int value);

    QString getPerfil() const;
    void setPerfil(const QString &value);

    QDate getDataCriacao() const;
    void setDataCriacao(const QDate &value);

    QDate getDataAlteracao() const;
    void setDataAlteracao(const QDate &value);

    QString getId() const;
    void setId(const QString &value);

private:
    QString id;
    QString usuario;
    QString senha;
    QString descricao;
    int codidoPerfil;
    QString perfil;
    QDate dataCriacao;
    QDate dataAlteracao;
};

#endif // USUARIOS_H
