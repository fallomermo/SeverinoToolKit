#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Severino Tools :: Versão 2.02 "));
    local.setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));
    loadingImage = new QMovie(":/images/logotipo_animado.gif");
    connect(this, SIGNAL(stopLoadingImage()), this, SLOT(stopAnimacaoLogotipo()));

    this->atualizarTema();
    this->animarLogotipo();
    this->setStatusSistema(false);

    ui->groupBox_LoginPanel->setAttribute(Qt::WA_OpaquePaintEvent);
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(ui->groupBox_LoginPanel);
    effect->setOpacity(0.8);
    ui->groupBox_LoginPanel->setGraphicsEffect(effect);

    connect(ui->campoUsuario, SIGNAL(returnPressed()), this, SLOT(focusCampoUsuario()));
    connect(ui->campoSenha, SIGNAL(returnPressed()), this, SLOT(focusCampoSenha()));
    connect(ui->botaoEntrar, SIGNAL(clicked(bool)), this, SLOT(focusBotaoEntrar()));
    connect(ui->botaoAbortar, SIGNAL(clicked(bool)), this, SLOT(close()));

    this->setStatusConexaoBancoLocal(true);
    this->atualizarUsuariosCadastrados(listaDeUsuarios);
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
    file.close();
    this->setStyleSheet(tema);
    this->update();
}

void Login::focusCampoUsuario()
{
    ui->campoSenha->setFocus();
}

void Login::focusCampoSenha()
{
    ui->botaoEntrar->setFocus();
}

void Login::focusBotaoEntrar()
{
    //SLOT
    this->setUsuario(ui->campoUsuario->text().trimmed());
    this->setSenha(ui->campoSenha->text().trimmed());
    this->abrirSistema();
}

bool Login::autenticarUsuario()
{
    bool ok = false;
    if(this->getStatusConexaoBancoLocal()) {
        QList<Usuarios*> l = this->getListaDeUsuarios();
        QString usr = this->getUsuario();
        QString pws = this->getSenha();

        for (int i = 0; i < l.count(); ++i) {
            Usuarios * u = l.at(i);
            if((u->getUsuario() == usr) && (u->getSenha() == pws)) {
                ok = true;
                i = l.count();
                this->setUsuario(u->getUsuario());
                this->setPerfil(u->getPerfil());
            } else {
                ok = false;
            };
        }
    } else {
        ok = false;
        QMessageBox::critical(this, tr("Sem conexão"), QString("Não foi possível estabelecer conexão com o base de dados!"), QMessageBox::Ok);
    }
    return ok;
}

void Login::invertendoConexaoParaSenior()
{
    ControleDAO *controleFluxo = new ControleDAO(Q_NULLPTR);
    QThread *thread = new QThread(Q_NULLPTR);
    connect(this, SIGNAL(finishThreadPri()), thread, SLOT(quit()), Qt::DirectConnection);
    connect(controleFluxo, SIGNAL(mensagemRetorno(QString,QString)), this, SLOT(mensagemRetornoUsuario(QString,QString)));
    connect(this, SIGNAL(obterConexaoBancoSenior()), controleFluxo, SLOT(abrirConexaoSenior()));
    connect(controleFluxo, SIGNAL(statusConexaoSenior(bool)), this, SLOT(atualizarStatusConexaoBaseSenior(bool)));
    connect(thread, SIGNAL(finished()), controleFluxo, SLOT(exitClass()));

    controleFluxo->moveToThread(thread);
    thread->start(QThread::NormalPriority);
    emit obterConexaoBancoSenior();
}

void Login::atualizarStatusConexaoBaseLocal(bool ok)
{
    this->setStatusConexaoBancoLocal(ok);
    if(ok) {
        ui->labelStatus->setPixmap(QPixmap(":/images/database_on.png"));
        this->carregarListaDeUsuariosCadastrados();
    } else
        ui->labelStatus->setPixmap(QPixmap(":/images/database_error.png"));

}

void Login::atualizarStatusConexaoBaseSenior(bool ok)
{
    this->setStatusConexaoBancoSenior(ok);
    emit obterStatusConexaoParaSenior(this->getStatusConexaoBancoSenior());
    emit finishThreadPri();
}

void Login::carregarListaDeUsuariosCadastrados()
{
    if(this->getStatusConexaoBancoLocal()) {
        this->atualizarUsuariosCadastrados(QList<Usuarios*>());
    } else {
        this->mensagemRetornoUsuario(QString("Conexão Banco"), QString("Sem conexão com a base local!"));
    }
}

void Login::atualizarUsuariosCadastrados(const QList<Usuarios *> u)
{
    //SLOT
    QList<Usuarios*> tmp_User = u;
    tmp_User.clear();
    Usuarios *u1 = new Usuarios(Q_NULLPTR);
    Usuarios *u2 = new Usuarios(Q_NULLPTR);
    Usuarios *u3 = new Usuarios(Q_NULLPTR);
    Usuarios *u4 = new Usuarios(Q_NULLPTR);

    u1->setUsuario("admin");
    u2->setUsuario("recrutador");
    u3->setUsuario("coordenador");
    u4->setUsuario("analista");

    u1->setSenha("admin");
    u2->setSenha("recrutador");
    u3->setSenha("coordenador");
    u4->setSenha("analista");

    tmp_User.insert(0, u1);
    tmp_User.insert(1, u2);
    tmp_User.insert(2, u3);
    tmp_User.insert(3, u4);

    this->setListaDeUsuarios(tmp_User);
}

void Login::abrirSistema()
{
    // Verifica a autenticidade do usuário
    if(this->getStatusSistema())
        return;

    if(autenticarUsuario()) {
        if(!this->getStatusSistema()) {
            QString __USR = QString("%0(%1)").arg(this->getUsuario()).arg(this->getPerfil());
            Principal *sistema = new Principal(this, __USR);
            ui->groupBox_LoginPanel->close();
            ui->horizontalLayout->addWidget(sistema);
            connect(sistema, SIGNAL(fecharSistema()), this, SLOT(fecharSistema()));
            connect(sistema, SIGNAL(trocarUsuario()), this, SLOT(trocarUsuario()));
            connect(this, SIGNAL(fecharSessao()), sistema, SLOT(encerrarSessao()), Qt::DirectConnection);
            connect(this, SIGNAL(usuarioAutenticado(QString)), sistema, SLOT(usuarioAutenticado(QString)));
            connect(this, SIGNAL(obterStatusConexaoParaSenior(bool)), sistema, SLOT(statusConexao(bool)));
            sistema->setWindowModality(Qt::ApplicationModal);
            this->setStatusSistema(true);
            sistema->show();
            emit usuarioAutenticado(__USR);
            emit stopLoadingImage();
        }
    } else {
        this->mensagemRetornoUsuario(QString("Erro de Autenticação"), QString("Usuário ou Senha Inválidos!"));
        ui->campoUsuario->setFocus();
    }
}

void Login::abrirAdministracaoGeral()
{

}

void Login::trocarUsuario()
{

}

void Login::mensagemRetornoUsuario(const QString t, const QString m)
{
    QMessageBox mensagem(this);
    mensagem.setText(m);
    mensagem.setDetailedText(t);
    mensagem.setToolTipDuration(200);
    mensagem.setEnabled(true);
    mensagem.setLocale(this->locale());
    mensagem.exec();
}

void Login::fecharSistema()
{
    this->close();
}

void Login::abortar()
{
    this->close();
}

void Login::animarLogotipo()
{
    ui->label_logo->setAttribute( Qt::WA_NoSystemBackground );
    ui->label_logo->setMovie( loadingImage );
    this->loadingImage->start();
}

void Login::stopAnimacaoLogotipo()
{
    this->loadingImage->stop();
    this->loadingImage->deleteLater();
}

void Login::updateStatusConnections()
{
    if(!this->getStatusConexaoBancoLocal())
    {
        if(loadingImage->state() == QMovie::NotRunning)
        {
            ui->labelStatus->setMovie(loadingImage);
            loadingImage->start();
        }
    } else {
        loadingImage = new QMovie(":/images/database_on.png");
    }
}

bool Login::getParamentrosAutomaticos() const
{
    return paramentrosAutomaticos;
}

void Login::setParamentrosAutomaticos(bool value)
{
    paramentrosAutomaticos = value;
}

void Login::closeEvent(QCloseEvent *e)
{
    if(QMessageBox::question(this, this->windowTitle(),
                             QString("Deseja realmente sair?"),
                             QMessageBox::Yes, QMessageBox::No) == QMessageBox::No) {
        e->ignore();
    } else {
        qApp->exit(0);
    }
}

void Login::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape)
        this->close();
}

QString Login::getPerfil() const
{
    return perfil;
}

void Login::setPerfil(const QString &value)
{
    perfil = value;
}

QString Login::getSenha() const
{
    return senha;
}

void Login::setSenha(const QString &value)
{
    senha = value;
}

QString Login::getUsuario() const
{
    return usuario;
}

void Login::setUsuario(const QString &value)
{
    usuario = value;
}

QMap<int, Usuarios *> Login::getMapDeUsuarios() const
{
    return mapDeUsuarios;
}

void Login::setMapDeUsuarios(const QMap<int, Usuarios *> &value)
{
    mapDeUsuarios = value;
}

QList<Usuarios *> Login::getListaDeUsuarios() const
{
    return listaDeUsuarios;
}

void Login::setListaDeUsuarios(const QList<Usuarios *> &value)
{
    listaDeUsuarios = value;
}

bool Login::getStatusConexaoBancoLocal() const
{
    return statusConexaoBancoLocal;
}

void Login::setStatusConexaoBancoLocal(bool value)
{
    statusConexaoBancoLocal = value;
}

bool Login::getStatusConexaoBancoSenior() const
{
    return statusConexaoBancoSenior;
}

void Login::setStatusConexaoBancoSenior(bool value)
{
    statusConexaoBancoSenior = value;
}

bool Login::getStatusSistema() const
{
    return statusSistema;
}

void Login::setStatusSistema(bool value)
{
    statusSistema = value;
}
