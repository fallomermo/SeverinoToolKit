#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QFile>
#include <QLocale>
#include <QTextStream>

#include "principal.h"
#include "usuarios.h"

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT
    QString usuario;
    QString senha;

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    QString getUsuario() const;
    void setUsuario(const QString &value);

    QString getSenha() const;
    void setSenha(const QString &value);

    bool getStatusSistema() const;
    void setStatusSistema(bool value);

signals:
    void usuarioAutenticado(QString);

private slots:
    void atualizarTema();
    void focusCampoUsuario();
    void focusCampoSenha();
    void focusBotaoEntrar();
    bool autenticarUsuario();
    void abrirSistema();
    void trocarUsuario();
    void fecharSistema();

private:
    Ui::Login *ui;
    Principal *sistema;

    QLocale local;
    bool statusSistema;

    QMap<int, Usuarios*> listUsers;

};

#endif // LOGIN_H
