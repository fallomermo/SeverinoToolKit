#include "login.h"
#include "ui_login.h"

QString Login::getUsuario() const
{
    return usuario;
}

void Login::setUsuario(const QString &value)
{
    usuario = value;
}

QString Login::getSenha() const
{
    return senha;
}

void Login::setSenha(const QString &value)
{
    senha = value;
}

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    this->atualizarTema();
    connect(ui->botaoEntrar, SIGNAL(clicked(bool)), this, SLOT(focusBotaoEntrar()));
    connect(ui->campoUsuario, SIGNAL(returnPressed()), this, SLOT(focusCampoUsuario()));
    connect(ui->campoSenha, SIGNAL(returnPressed()), this, SLOT(focusCampoSenha()));
    this->setUsuario(Q_NULLPTR);
    this->setSenha(Q_NULLPTR);
    this->setStatusSistema(false);

}

Login::~Login()
{
    delete ui;
}

void Login::atualizarTema()
{
    QFile file(QString(":/images/tema.css"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString tema;
    while (!in.atEnd()) {
        QString line = in.readLine();
        tema.append(line);
    }

    this->setStyleSheet(tema);
    this->update();
}

void Login::focusCampoUsuario()
{
    this->setUsuario(ui->campoUsuario->text().trimmed());
    if(this->getUsuario() == "admin")
        ui->campoUsuario->setText("Administrador");
    ui->campoUsuario->setEnabled(false);
    ui->campoSenha->setFocus();
}

void Login::focusCampoSenha()
{
    this->setSenha(ui->campoSenha->text());
    ui->campoSenha->setEnabled(false);
    ui->botaoEntrar->setFocus();
    this->abrirSistema();
}

void Login::focusBotaoEntrar()
{
    this->setUsuario(ui->campoUsuario->text().trimmed());
    if((this->getUsuario() == "admin") || (this->getUsuario() == "Administrador") || (this->getUsuario() == "administrador"))
        ui->campoUsuario->setText("Administrador");
    ui->campoUsuario->setEnabled(false);

    this->setSenha(ui->campoSenha->text());
    ui->campoSenha->setEnabled(false);
    this->abrirSistema();
}

bool Login::autenticarUsuario()
{
    QString __usuario = this->getUsuario();
    QString __senha = this->getSenha();
    if((__usuario == "admin") && (__senha == "admin"))
        return true;
    else
        return false;
}

void Login::abrirSistema()
{
    if(autenticarUsuario()) {
        Principal *sistema = new Principal(this);
        sistema->setUsuarioAutenticado(this->getUsuario());
        ui->groupBox_Login->hide();
        ui->horizontalLayout->addWidget(sistema);
        connect(sistema, SIGNAL(fecharSistema()), this, SLOT(fecharSistema()));
        connect(sistema, SIGNAL(trocarUsuario()), this, SLOT(trocarUsuario()));
        connect(this, SIGNAL(usuarioAutenticado(QString)), sistema, SLOT(atualizarInformacoesUsuario(QString)));
        sistema->setWindowModality(Qt::ApplicationModal);
        this->setStatusSistema(true);
        sistema->show();
        emit usuarioAutenticado(this->getUsuario());
    } else {
        this->setUsuario(Q_NULLPTR);
        this->setSenha(Q_NULLPTR);
        ui->campoUsuario->setEnabled(true);
        ui->campoSenha->setEnabled(true);
        ui->campoSenha->clear();
        QMessageBox::critical(this, tr("Autenticação Falhou!"), QString("Usuário ou Senha Inválida!"), QMessageBox::Ok);
        ui->campoUsuario->setFocus();
    }
}

void Login::trocarUsuario()
{
    this->setUsuario(Q_NULLPTR);
    this->setSenha(Q_NULLPTR);
    ui->groupBox_Login->show();
    ui->campoUsuario->setEnabled(true);
    ui->campoSenha->setEnabled(true);
    ui->campoSenha->clear();
    ui->campoUsuario->setFocus();
}

void Login::fecharSistema()
{
    qApp->exit(0);
}

bool Login::getStatusSistema() const
{
    return statusSistema;
}

void Login::setStatusSistema(bool value)
{
    statusSistema = value;
}
