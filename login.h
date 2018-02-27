#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QFile>
#include <QTimer>
#include <QMovie>
#include <QInputDialog>
#include <QPaintEvent>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QLocale>
#include <QTextStream>
#include <QVersionNumber>
#include <QGraphicsOpacityEffect>
#include <QCryptographicHash>

#include "controledao.h"
#include "principal.h"
#include "usuarios.h"
#include "administracaogeral.h"

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

    bool getStatusSistema() const;
    void setStatusSistema(bool value);

    bool getStatusConexaoBancoSenior() const;
    void setStatusConexaoBancoSenior(bool value);

    bool getStatusConexaoBancoLocal() const;
    void setStatusConexaoBancoLocal(bool value);

    QList<Usuarios *> getListaDeUsuarios() const;
    void setListaDeUsuarios(const QList<Usuarios *> &value);

    QMap<int, Usuarios *> getMapDeUsuarios() const;
    void setMapDeUsuarios(const QMap<int, Usuarios *> &value);

    QString getUsuario() const;
    void setUsuario(const QString &value);

    QString getSenha() const;
    void setSenha(const QString &value);

    QString getPerfil() const;
    void setPerfil(const QString &value);

    bool getParamentrosAutomaticos() const;
    void setParamentrosAutomaticos(bool value);

signals:
    void obterConexaoBancoSenior();
    void inverterConexaoParaLocal();
    void inverterConexaoParaSenior();
    void obterStatusConexaoParaLocal(bool);
    void obterStatusConexaoParaSenior(bool);

    void usuarioAutenticado(QString);
    void fecharSessao();
    void finishThread();

    void finishThreadPri();
    void finishThreadAux();

    void stopLoadingImage();

private slots:
    //Implementado navegação entre os campos QLinEdit
    void focusCampoUsuario();
    void focusCampoSenha();
    void focusBotaoEntrar();
    bool autenticarUsuario();
    void invertendoConexaoParaSenior();

    void atualizarStatusConexaoBaseLocal(bool);
    void atualizarStatusConexaoBaseSenior(bool);

    void carregarListaDeUsuariosCadastrados();
    void atualizarUsuariosCadastrados(const QList<Usuarios*>);

    void abrirSistema();
    void abrirAdministracaoGeral();
    void trocarUsuario();
    void mensagemRetornoUsuario(const QString, const QString);

    void fecharSistema();
    void abortar();

    void animarLogotipo();
    void stopAnimacaoLogotipo();
    void updateStatusConnections();

private:
    Ui::Login *ui;
    Principal *sistema;
    AdministracaoGeral *admSis;

    QLocale local;
    bool statusSistema;
    bool statusConexaoBancoSenior;
    bool statusConexaoBancoLocal;

    // variaveis de usuário corrente
    QString usuario;
    QString senha;
    QString perfil;
    QMap<int, Usuarios *> mapDeUsuarios;
    QList<Usuarios *> listaDeUsuarios;

    QThread *threadPri;
    QThread *threadAux;
    QTimer *timerConn;
    QMovie *loadingImage;

    bool paramentrosAutomaticos;


protected:
    void closeEvent(QCloseEvent *e);
    void keyPressEvent(QKeyEvent *e);

};

#endif // LOGIN_H
