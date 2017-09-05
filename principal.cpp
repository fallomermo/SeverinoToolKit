#include "principal.h"
#include "ui_principal.h"

Principal::Principal(QWidget *parent) : QWidget(parent), ui(new Ui::Principal)
{
    ui->setupUi(this);
    setTimeSession(QTime::currentTime());

    aplicarDefinicoesGerais();
}

Principal::~Principal()
{
    delete ui;
}

void Principal::aplicarDefinicoesGerais()
{
    ui->campoUsuario->setText(this->getUsuarioAutenticado());
    atualizarTema();

    ui->toolButtonIntegracaoFinanceira->setPopupMode(QToolButton::InstantPopup);
    ui->toolButtonIntegracaoRH->setPopupMode(QToolButton::InstantPopup);
    ui->toolButtonIntegracaoCrednosso->setPopupMode(QToolButton::InstantPopup);
    ui->toolButtonBlissBeneficios->setPopupMode(QToolButton::InstantPopup);
    ui->toolButtonControlePonto->setPopupMode(QToolButton::InstantPopup);
    ui->toolButtonFerramentas->setPopupMode(QToolButton::InstantPopup);

    QAction *actionHomeInicio = new QAction(QIcon(":/images/home.png"), "&Home",ui->toolButtonIntegracaoFinanceira);
    QAction *actionPlanoContas = new QAction(QIcon(":/images/tax.png"), "&Plano de Contas",ui->toolButtonIntegracaoFinanceira);
    QAction *actionPlanoSaude = new QAction(QIcon(":/images/heart.png"), "&Plano de Saúde",ui->toolButtonIntegracaoFinanceira);
    QAction *actionLiquidoFolha = new QAction(QIcon(":/images/water-drop.png"), "&Líquido Folha [No]",ui->toolButtonIntegracaoFinanceira);
    QAction *actionGuiaINSS = new QAction(QIcon(":/images/piggy-bank.png"), "&Guia INSS Rateio",ui->toolButtonIntegracaoRH);
    QAction *actionEventosFolha = new QAction(QIcon(":/images/"),"&Eventos Folha [No]",ui->toolButtonIntegracaoRH);
    QAction *actionMetaRetencaoNormal = new QAction(QIcon(":/images/stocks-graphic-with-a-magnifier-tool.png"),"&Meta Retenção",ui->toolButtonIntegracaoRH);
    QAction *actionMetaRetencaoEstruturada= new QAction(QIcon(":/images/handshake.png"),"&Meta Retenção Estruturada",ui->toolButtonIntegracaoRH);
    QAction *actionRelacaoColaboradores = new QAction(QIcon(":/images/team.png"), "&Relação de Colaboradores",ui->toolButtonIntegracaoRH);
    QAction *actionHigienizacaoCrednosso = new QAction(QIcon(":/images/groupcrednosso.png"), "&Higienizacao Crednosso",ui->toolButtonIntegracaoCrednosso);
    QAction *actionBlissConvertFiles = new QAction(QIcon(":/images/groupcrednosso.png"), "&Processar Arquivos Bliss",ui->toolButtonBlissBeneficios);
    QAction *actionDownloadAhgoraAFD = new QAction(QIcon(":/images/clock-circular-outline.png"), "&Download Ahgora AFD",ui->toolButtonControlePonto);
    QAction *actionProcessarArquivo = new QAction(QIcon(":/images/groupcrednosso.png"), "&Processar Arquivo",ui->toolButtonFerramentas);
    QAction *actionTruncarArquivo = new QAction(QIcon(":/images/tools.png"), "&Truncar Arquivo",ui->toolButtonFerramentas);
    QAction *actionTrocarUsuario = new QAction(QIcon(":/images/user.png"), "&Trocar Usuário",ui->toolButtonFerramentas);

    //Criand menus para os botoes
    QMenu *menuFinanceiro = new QMenu(ui->toolButtonIntegracaoFinanceira);
    QMenu *menuCrednosso = new QMenu(ui->toolButtonIntegracaoCrednosso);
    QMenu *menuRH = new QMenu(ui->toolButtonIntegracaoRH);
    QMenu *menuBlissBeneficios = new QMenu(ui->toolButtonBlissBeneficios);
    QMenu *menuControlePonto = new QMenu(ui->toolButtonControlePonto);
    QMenu *menuFerramentas = new QMenu(ui->toolButtonFerramentas);

    //Criando conexoes para acoes ligando com os slots
    connect(actionHomeInicio, SIGNAL(triggered(bool)), this, SLOT(homeInicio()));
    connect(actionPlanoContas, SIGNAL(triggered(bool)), this, SLOT(planoContas()));
    connect(actionPlanoSaude, SIGNAL(triggered(bool)), this, SLOT(planoSaude()));
    connect(actionGuiaINSS, SIGNAL(triggered(bool)), this, SLOT(guiaInssFolha()));
    connect(actionMetaRetencaoNormal, SIGNAL(triggered(bool)), this, SLOT(metaRetencaoNormal()));
    connect(actionMetaRetencaoEstruturada, SIGNAL(triggered(bool)), this, SLOT(metaRetencaoEstruturada()));
    connect(actionRelacaoColaboradores, SIGNAL(triggered(bool)), this, SLOT(relacaoColaborador()));
    connect(actionHigienizacaoCrednosso, SIGNAL(triggered(bool)), this, SLOT(higienizacaoCrednosso()));
    connect(actionBlissConvertFiles, SIGNAL(triggered(bool)), this, SLOT(processarArquivosBliss()));
    connect(actionDownloadAhgoraAFD, SIGNAL(triggered(bool)), this, SLOT(downloadAhgoraAFD()));
    connect(actionProcessarArquivo, SIGNAL(triggered(bool)), this, SLOT(processarArquivos()));
    connect(actionTruncarArquivo, SIGNAL(triggered(bool)), this, SLOT(truncarArquivos()));
    connect(actionTrocarUsuario, SIGNAL(triggered(bool)), this, SLOT(alterarUsuario()));

    //Adicionado acoes no menu de grupo financeiro
    menuFinanceiro->addAction(actionHomeInicio);
    menuFinanceiro->addAction(actionPlanoContas);
    menuFinanceiro->addAction(actionPlanoSaude);
    menuFinanceiro->addAction(actionLiquidoFolha);
    ui->toolButtonIntegracaoFinanceira->setMenu(menuFinanceiro);

    //Adicionado acoes no menu de grupo crednosso
    menuCrednosso->addAction(actionHigienizacaoCrednosso);
    ui->toolButtonIntegracaoCrednosso->setMenu(menuCrednosso);

    //Adicionado acoes no menu de grupo RH
    menuRH->addAction(actionHomeInicio);
    menuRH->addAction(actionGuiaINSS);
    menuRH->addAction(actionEventosFolha);
    menuRH->addAction(actionMetaRetencaoNormal);
    menuRH->addAction(actionMetaRetencaoEstruturada);
    menuRH->addAction(actionRelacaoColaboradores);
    ui->toolButtonIntegracaoRH->setMenu(menuRH);

    //Adicionado acoes no menu de grupo Bliss Beneficios
    menuBlissBeneficios->addAction(actionBlissConvertFiles);
    ui->toolButtonBlissBeneficios->setMenu(menuBlissBeneficios);

    //Adicionado acoes no menu de Controle de Ponto
    menuControlePonto->addAction(actionDownloadAhgoraAFD);
    ui->toolButtonControlePonto->setMenu(menuControlePonto);

    //Adicionado acoes no menu de grupo crednosso
    menuFerramentas->addAction(actionProcessarArquivo);
    menuFerramentas->addAction(actionTruncarArquivo);
    menuFerramentas->addAction(actionTrocarUsuario);
    ui->toolButtonFerramentas->setMenu(menuFerramentas);

    _flagHomeInicio   = false;
    _flagPlanoContas  = false;
    _flagPlanoSaude   = false;
    _flagLiquidoFolha = false;
    _flagGuiaINSS     = false;
    _flagEventosFolha = false;
    _flagMetaRetencaoNormal = false;
    _flagMetaRetencaoEstruturada = false;
    _flagRelacaoColaborador = false;
    _flagHigienizacaoCrednosso = false;
    _flagDownloadAhgoraAFD = false;
    _flagProcessarBlissBeneficios = false;
    _flagProcessarArquivos = false;
    _flagTruncarArquivos = false;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(1000);

    controleFluxo = new ControleDAO(nullptr);
    QThread* threadDAO = new QThread(nullptr);
    qRegisterMetaType<QMap<int,CadastroEmpresa*>>("__tempCadastroEmpresas");
    qRegisterMetaType<QMap<int,CadastroFilial*>>("__tempCadastroFiliais");

    connect(controleFluxo, SIGNAL(mensagemRetorno(QString)), this, SLOT(mensagemRetornoUsuario(QString)));
    connect(this, SIGNAL(obterConexaoBanco()), controleFluxo, SLOT(abrirConexao()));
    connect(controleFluxo, SIGNAL(enviarStatusConexao(bool)), this, SLOT(statusConexao(bool)));
    connect(this, SIGNAL(obterCadastroDeEmpresas()), controleFluxo, SLOT(enviarCadastroDeEmpresas()));
    connect(this, SIGNAL(obterCadastroDeFiliais()), controleFluxo, SLOT(enviarCadastroDeFiliais()));
    connect(controleFluxo, SIGNAL(retornarCadastroDeEmpresas(QMap<int,CadastroEmpresa*>)), this, SLOT(definirCadastrosDeEmpresas(QMap<int,CadastroEmpresa*>)));
    connect(controleFluxo, SIGNAL(retornarCadastroDeFiliais(QMap<int,CadastroFilial*>)), this, SLOT(definirCadastrosDeFiliais(QMap<int,CadastroFilial*>)));
    connect(this, SIGNAL(finishThread()), threadDAO, SLOT(terminate()));
    connect(threadDAO, SIGNAL(finished()), controleFluxo, SLOT(deleteLater()));
    controleFluxo->moveToThread(threadDAO);
    threadDAO->start(QThread::NormalPriority);

    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(ui->botaoSair, SIGNAL(clicked(bool)), this, SLOT(close()));
    emit obterConexaoBanco();
    ui->campoUsuario->setText("Convidado");
}

void Principal::homeInicio()
{
    if(_flagHomeInicio) {
        ui->tabWidget->setCurrentIndex(_indexHomeInicio);
    } else {
        __homeInicio = new HomeInicio(this);
        __homeInicio->setObjectName("homeInicio");
        ui->tabWidget->addTab(__homeInicio, QIcon(":/images/tax.png"), QString("Home :: Início"));
        ui->tabWidget->setCurrentWidget(__homeInicio);
        _flagHomeInicio = true;
        _indexHomeInicio = ui->tabWidget->indexOf(__homeInicio);
    }
}

void Principal::planoContas()
{
    if(_flagPlanoContas) {
        ui->tabWidget->setCurrentIndex(_indexPlanoContas);
    } else {
        QMap<int, CadastroEmpresa*> me = mapEmpresas;
        QMap<int, CadastroFilial*> mf = mapFiliais;
        __planoContas = new PlanoDeContas(this, me, mf);
        __planoContas->setObjectName("planoContas");
        ui->tabWidget->addTab(__planoContas, QIcon(":/images/tax.png"), QString("Plano de Contas"));
        ui->tabWidget->setCurrentWidget(__planoContas);
        _flagPlanoContas = true;
        _indexPlanoContas = ui->tabWidget->indexOf(__planoContas);
    }
}

void Principal::planoSaude()
{
    if(_flagPlanoSaude) {
        ui->tabWidget->setCurrentIndex(_indexPlanoSaude);
    } else {
        __planoSaude = new PlanoSaude(this);
        __planoSaude->setObjectName("planoSaude");
        ui->tabWidget->addTab(__planoSaude, QIcon(":/images/heart.png"), QString("Plano de Saúde"));
        ui->tabWidget->setCurrentWidget(__planoSaude);
        _flagPlanoSaude = true;
        _indexPlanoSaude = ui->tabWidget->indexOf(__planoSaude);
    }
}

void Principal::guiaInssFolha()
{
    if(_flagGuiaINSS) {
        ui->tabWidget->setCurrentIndex(_indexGuiaINSS);
    } else {
        __guiaInss = new GuiaINSSFolha(this, mapEmpresas, mapFiliais);
        __guiaInss->setObjectName("guiaINSSFolha");
        ui->tabWidget->addTab(__guiaInss, QIcon(":/images/piggy-bank.png"), QString("Guia INSS Folha"));
        ui->tabWidget->setCurrentWidget(__guiaInss);
        _flagGuiaINSS = true;
        _indexGuiaINSS = ui->tabWidget->indexOf(__guiaInss);
    }
}

void Principal::metaRetencaoNormal()
{
    if(_flagMetaRetencaoNormal) {
        ui->tabWidget->setCurrentIndex(_indexMetaRetencaoNormal);
    } else {
        __metaRetencao = new MetaRetencao(this);
        __metaRetencao->setObjectName("metaRetencaoNormal");
        ui->tabWidget->addTab(__metaRetencao, QIcon(":/images/piggy-bank.png"), QString("Meta de Retenção"));
        ui->tabWidget->setCurrentWidget(__metaRetencao);
        _flagMetaRetencaoNormal = true;
        _indexMetaRetencaoNormal = ui->tabWidget->indexOf(__metaRetencao);
    }
}

void Principal::metaRetencaoEstruturada()
{
    if(_flagMetaRetencaoEstruturada) {
        ui->tabWidget->setCurrentIndex(_indexMetaRetencaoEstruturada);
    } else {
        __metaRetencaoEstruturada = new MetaRetencaoEstruturada(this);
        __metaRetencaoEstruturada->setObjectName("metaRetencaoEstruturada");
        ui->tabWidget->addTab(__metaRetencaoEstruturada, QIcon(":/images/handshake.png"), QString("Meta de Retenção"));
        ui->tabWidget->setCurrentWidget(__metaRetencaoEstruturada);
        _flagMetaRetencaoEstruturada = true;
        _indexMetaRetencaoEstruturada = ui->tabWidget->indexOf(__metaRetencaoEstruturada);
    }
}

void Principal::relacaoColaborador()
{
    if(_flagRelacaoColaborador) {
        ui->tabWidget->setCurrentIndex(_indexRelacaoColaborador);
    } else {
        __relacaoColaboradores = new RelacaoColaborador(this, mapEmpresas, mapFiliais);
        __relacaoColaboradores->setObjectName("relacaoColaborador");
        ui->tabWidget->addTab(__relacaoColaboradores, QIcon(":/images/team.png"), QString("Relação de Colaboradores"));
        ui->tabWidget->setCurrentWidget(__relacaoColaboradores);
        _flagRelacaoColaborador = true;
        _indexRelacaoColaborador = ui->tabWidget->indexOf(__relacaoColaboradores);
    }
}

void Principal::higienizacaoCrednosso()
{
    if(_flagHigienizacaoCrednosso) {
        ui->tabWidget->setCurrentIndex(_indexHigienizacaoCrednosso);
    } else {
        __crednosso = new HigienizacaoCrednosso(this);
        __crednosso->setObjectName("higienizacaoCrednosso");
        ui->tabWidget->addTab(__crednosso, QIcon(":/images/team.png"), QString("Higienização Crednosso"));
        ui->tabWidget->setCurrentWidget(__crednosso);
        _flagHigienizacaoCrednosso = true;
        _indexHigienizacaoCrednosso = ui->tabWidget->indexOf(__crednosso);
    }
}

void Principal::downloadAhgoraAFD()
{
    if(_flagDownloadAhgoraAFD) {
        ui->tabWidget->setCurrentIndex(_indexDownloadAhgoraAFD);
    } else {
        __downloadAhgoraAFD = new DownloadAhgoraAFD(this);
        __downloadAhgoraAFD->setObjectName("downloadAhgoraAFD");
        ui->tabWidget->addTab(__downloadAhgoraAFD, QString("Download Ahgora AFD"));
        ui->tabWidget->setCornerWidget(__downloadAhgoraAFD);
        _flagDownloadAhgoraAFD = true;
        _indexDownloadAhgoraAFD = ui->tabWidget->indexOf(__downloadAhgoraAFD);
    }
}

void Principal::processarArquivosBliss()
{
    if(_flagProcessarBlissBeneficios) {
        ui->tabWidget->setCurrentIndex(_indexProcessarBlissBeneficios);
    } else {
        __blissBeneficios = new BlissBeneficios(this);
        __blissBeneficios->setObjectName("blissBeneficios");
        ui->tabWidget->addTab(__blissBeneficios, QIcon(":/images/vt.png"), QString("Bliss Beneficios"));
        ui->tabWidget->setCurrentWidget(__blissBeneficios);
        _flagProcessarBlissBeneficios = true;
        _indexProcessarBlissBeneficios = ui->tabWidget->indexOf(__blissBeneficios);
    }
}

void Principal::processarArquivos()
{
    if(_flagProcessarArquivos) {
        ui->tabWidget->setCurrentIndex(_indexProcessarArquivos);
    } else {
        __processarArquivos = new ProcessarArquivoCrednosso(this);
        __processarArquivos->setObjectName("processarArquivos");
        ui->tabWidget->addTab(__processarArquivos, QIcon(":/images/team.png"), QString("Procesar Arquivos"));
        ui->tabWidget->setCurrentWidget(__processarArquivos);
        _flagProcessarArquivos = true;
        _indexProcessarArquivos = ui->tabWidget->indexOf(__processarArquivos);
    }
}

void Principal::truncarArquivos()
{
    if(_flagTruncarArquivos) {
        ui->tabWidget->setCurrentIndex(_indexTruncarArquivos);
    } else {
        __truncarArquivos = new TruncarArquivosTexto(this);
        __truncarArquivos->setObjectName("truncarArquivos");
        ui->tabWidget->addTab(__truncarArquivos, QIcon(":/images/team.png"), QString("Truncar Arquivos Texto"));
        ui->tabWidget->setCurrentWidget(__truncarArquivos);
        _flagTruncarArquivos = true;
        _indexTruncarArquivos = ui->tabWidget->indexOf(__truncarArquivos);
    }
}

void Principal::closeTab(int i)
{
    QWidget *wgt = ui->tabWidget->widget(i);
    if(wgt->objectName().contains("homeInicio"))
        _flagHomeInicio = false;
    if(wgt->objectName().contains("planoContas"))
        _flagPlanoContas = false;
    if(wgt->objectName().contains("planoSaude"))
        _flagPlanoSaude = false;
    if(wgt->objectName().contains("guiaINSS"))
        _flagGuiaINSS = false;
    if(wgt->objectName().contains("liquidoFolha"))
        _flagLiquidoFolha = false;
    if(wgt->objectName().contains("metaRetencaoNormal"))
        _flagMetaRetencaoNormal = false;
    if(wgt->objectName().contains("metaRetencaoEstruturada"))
        _flagMetaRetencaoEstruturada = false;
    if(wgt->objectName().contains("relacaoColaborador"))
        _flagRelacaoColaborador = false;
    if(wgt->objectName().contains("higienizacaoCrednosso"))
        _flagHigienizacaoCrednosso = false;
    if(wgt->objectName().contains("blissBeneficios"))
        _flagProcessarBlissBeneficios = false;
    if(wgt->objectName().contains("downloadAhgoraAFD"))
        _flagDownloadAhgoraAFD = false;
    if(wgt->objectName().contains("processarArquivos"))
        _flagProcessarArquivos = false;
    if(wgt->objectName().contains("truncarArquivos"))
        _flagProcessarArquivos = false;

    ui->tabWidget->removeTab(i);
    wgt->deleteLater();
}

void Principal::atualizarTema()
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

void Principal::instalarTema()
{
    QFile file(QFileDialog::getOpenFileName(this, tr("Abrir Arquivo CSS"), QDir::currentPath(), "Todos os arquivos (*.*)"));
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    QString tema;

    while (!in.atEnd()) {
        QString line = in.readLine();
        tema.append(line);
    }
    this->setStyleSheet(tema);
}

void Principal::updateTime()
{
    ui->campoTime->setText(QString("%0 ms").arg(QTime::fromMSecsSinceStartOfDay(getTimeSession().elapsed()).toString("HH:mm:ss")));
}

void Principal::mensagemRetornoUsuario(QString msg)
{
    QMessageBox::critical(this, tr("Conexão de Banco"), QString("Nenhuma conexão com o Banco de Dados[!]\n").append(msg), QMessageBox::Ok);
}

void Principal::mensagemControlador(QMessageBox& mensagem)
{
    mensagem.exec();
}

void Principal::finishThreadBool(bool ok)
{
    if(ok) {
        if(this->getMapEmpresas().count() > 0 && this->getMapFiliais().count() > 0)
            emit finishThread();
        else
            emit obterCadastroDeEmpresas();
    }
}

void Principal::atualizarInformacoesUsuario(QString usr)
{
    ui->campoUsuario->setText(usr);
}

void Principal::alterarUsuario()
{
    emit trocarUsuario();
}

QString Principal::getUsuarioAutenticado() const
{
    return usuarioAutenticado;
}

void Principal::setUsuarioAutenticado(const QString &value)
{
    usuarioAutenticado = value;
}

void Principal::closeEvent(QCloseEvent *event)
{
    if(QMessageBox::question(this, this->windowTitle(), QString("Deseja realmente sair?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::No) {
        event->ignore();
    } else {
        emit fecharSistema();
        this->close();
    }
}

QMap<int, CadastroFilial *> Principal::getMapFiliais() const
{
    return mapFiliais;
}

void Principal::setMapFiliais(const QMap<int, CadastroFilial *> &value)
{
    mapFiliais = value;
}

QMap<int, CadastroEmpresa *> Principal::getMapEmpresas() const
{
    return mapEmpresas;
}

void Principal::setMapEmpresas(const QMap<int, CadastroEmpresa *> &value)
{
    mapEmpresas = value;
}

QTime Principal::getTimeSession() const
{
    return timeSession;
}

void Principal::setTimeSession(const QTime &value)
{
    timeSession = value;
}

void Principal::statusConexao(bool ok)
{
    if(ok){
        ui->statusAplicacao->setText("Conectado!!!");
        this->finishThreadBool(true);
    } else {
        ui->statusAplicacao->setText("Sem conexão [!]");
        this->finishThreadBool(true);
    }
}

void Principal::definirCadastrosDeEmpresas(const QMap<int, CadastroEmpresa *> __tempMap)
{
    if(__tempMap.count() > 0) {
        this->setMapEmpresas(__tempMap);
        emit obterCadastroDeFiliais();
    } else {
        emit finishThread();
    }
}

void Principal::definirCadastrosDeFiliais(const QMap<int, CadastroFilial *> __tempMap)
{
    if(__tempMap.count() > 0)
        this->setMapFiliais(__tempMap);
    emit finishThread();
}

void Principal::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F2) {
        ui->toolButtonIntegracaoFinanceira->setFocus();
        ui->toolButtonIntegracaoFinanceira->click();
        return;
    }
    if(event->key() == Qt::Key_F3) {
        ui->toolButtonIntegracaoCrednosso->setFocus();
        ui->toolButtonIntegracaoCrednosso->click();
        return;
    }
    if(event->key() == Qt::Key_F4) {
        ui->toolButtonIntegracaoRH->setFocus();
        ui->toolButtonIntegracaoRH->click();
        return;
    }
    if(event->key() == Qt::Key_F5) {
        ui->toolButtonBlissBeneficios->setFocus();
        ui->toolButtonBlissBeneficios->click();
        return;
    }
    if(event->key() == Qt::Key_F6) {
        ui->toolButtonControlePonto->setFocus();
        ui->toolButtonControlePonto->click();
        return;
    }
    if(event->key() == Qt::Key_F7) {
        ui->toolButtonFerramentas->setFocus();
        ui->toolButtonFerramentas->click();
        return;
    }
}
