#include "principal.h"
#include "ui_principal.h"

Principal::Principal(QWidget *parent, QString u) : QWidget(parent), usuarioSessao(u), ui(new Ui::Principal)
{
    ui->setupUi(this);

    this->setTimeSession(QTime::currentTime());
    this->aplicarDefinicoesGerais();
    this->setStatusConexaoSenior(true);

    QThread *srv = new QThread(Q_NULLPTR);
    ControleDAO *controle = new ControleDAO(Q_NULLPTR);

    connect(this, SIGNAL(obterConexaoBanco()), controle, SLOT(abrirConexaoSenior()));
    connect(this, SIGNAL(obterCadastroDeEmpresas()), controle, SLOT(enviarCadastroDeEmpresas()));
    connect(this, SIGNAL(obterCadastroDeFiliais()), controle, SLOT(enviarCadastroDeFiliais()));
    connect(controle, SIGNAL(enviarStatusConexaoSenior(bool)), this, SLOT(statusConexao(bool)));
    connect(controle, SIGNAL(retornarCadastroDeEmpresas(QMap<int,CadastroEmpresa*>)), this, SLOT(definirCadastrosDeEmpresas(QMap<int,CadastroEmpresa*>)));
    connect(controle, SIGNAL(retornarCadastroDeFiliais(QMap<int,CadastroFilial*>)), this, SLOT(definirCadastrosDeFiliais(QMap<int,CadastroFilial*>)));

    connect(srv, SIGNAL(finished()), controle, SLOT(exitClass()));
    connect(this, SIGNAL(finishThread()), srv, SLOT(quit()), Qt::DirectConnection);

    controle->moveToThread(srv);
    srv->start(QThread::NormalPriority);
    emit obterConexaoBanco();
}

Principal::~Principal()
{
    delete ui;
}

void Principal::aplicarDefinicoesGerais()
{
    ui->botaoAvatar->setText(this->getUsuarioSessao());
    QAction *act_HomeInicio = new QAction(QIcon(":/images/home.png"), "&Home", ui->toolButtonIntegracaoFinanceira);
    QAction *act_PlanoContas = new QAction(QIcon(":/images/tax.png"), "&Plano de Contas", ui->toolButtonIntegracaoFinanceira);
    QAction *act_PlanoSaude = new QAction(QIcon(":/images/heart.png"), "&Plano de Saúde", ui->toolButtonIntegracaoFinanceira);
    QAction *act_LiquidoFolha = new QAction(QIcon(":/images/water-drop.png"), "&Líquido Folha", ui->toolButtonIntegracaoFinanceira);
    QAction *act_GuiaINSS = new QAction(QIcon(":/images/piggy-bank.png"), "&Guia INSS Rateio", ui->toolButtonRecursosHumanos);
    QAction *act_EventosFolha = new QAction(QIcon(":/images/"),"&Eventos Folha [No]", ui->toolButtonRecursosHumanos);
    QAction *act_MetaRetencaoNormal = new QAction(QIcon(":/images/stocks-graphic-with-a-magnifier-tool.png"),"&Meta Retenção", ui->toolButtonRecursosHumanos);
    QAction *act_MetaRetencaoEstruturada= new QAction(QIcon(":/images/male-job-search-symbol.png"),"&Meta Retenção Estruturada", ui->toolButtonRecursosHumanos);
    QAction *act_OcorrenciasApuracao = new QAction(QIcon(":/images/wall-clock.png"),"&Ocorrências de Apuração", ui->toolButtonRecursosHumanos);
    QAction *act_RelacaoColaboradores = new QAction(QIcon(":/images/team.png"), "&Relação de Colaboradores", ui->toolButtonRecursosHumanos);
    QAction *act_HigienizacaoCrednosso = new QAction(QIcon(":/images/groupcrednosso.png"), "&Higienizacao Crednosso", ui->toolButtonCartaoCrednosso);
    QAction *act_BlissConvertFiles = new QAction(QIcon(":/images/groupcrednosso.png"), "&Processar Arquivos Bliss", ui->toolButtonBeneficios);
    QAction *act_GerenciamentoREP = new QAction(QIcon(":/images/clock-circular-outline.png"), "&Gerenciamento REP", ui->toolButtonControlePonto);
    QAction *act_Ecoclinic = new QAction(QIcon(":/images/clock-circular-outline.png"), "&Ecoclinic Repasses", ui->toolButtonBeneficios);
    QAction *act_ProcessarArquivo = new QAction(QIcon(":/images/groupcrednosso.png"), "&Processar Arquivo", ui->toolButtonFerramentas);
    QAction *act_TruncarArquivo = new QAction(QIcon(":/images/tools.png"), "&Truncar Arquivo", ui->toolButtonFerramentas);
    QAction *act_SobreSistema = new QAction(QIcon(":/images/info.png"), "&Sobre o Severino", ui->toolButtonFerramentas);
    QAction *act_ControleDePonto = new QAction(QIcon(":/images/info.png"), "&Controle de Ponto", ui->toolButtonFerramentas);
    QAction *act_AdministracaoSistema = new QAction(QIcon(":/images/tools.png"), "&Administração do Sistema", ui->toolButtonFerramentas);
    QAction *act_ConversaoDeArquivos = new QAction(QIcon(":/images/file_analict.png"), "&Conversão de Arquivos", ui->toolButtonFerramentas);

    QAction *act_AlterarUsuario = new QAction(QIcon(":/images/user_change.png"), "&Alterar Usuario", ui->botaoAvatar);
    QAction *act_AlterarSenha = new QAction(QIcon(":/images/password.png"), "&Alterar Senha", ui->botaoAvatar);

    //Criand menus para os botoes
    QMenu *menuFinanceiro = new QMenu(ui->toolButtonIntegracaoFinanceira);
    QMenu *menuCrednosso = new QMenu(ui->toolButtonCartaoCrednosso);
    QMenu *menuRH = new QMenu(ui->toolButtonRecursosHumanos);
    QMenu *menuBeneficios = new QMenu(ui->toolButtonBeneficios);
    QMenu *menuControlePonto = new QMenu(ui->toolButtonControlePonto);
    QMenu *menuFerramentas = new QMenu(ui->toolButtonFerramentas);
    QMenu *menuPropriedadesUsuario = new QMenu(ui->botaoAvatar);

    //Criando conexoes para acoes ligando com os slots
    connect(act_HomeInicio, SIGNAL(triggered(bool)), this, SLOT(homeInicio()));
    connect(act_PlanoContas, SIGNAL(triggered(bool)), this, SLOT(planoContas()));
    connect(act_PlanoSaude, SIGNAL(triggered(bool)), this, SLOT(planoSaude()));
    connect(act_LiquidoFolha, SIGNAL(triggered(bool)), this, SLOT(liquidoFolha()));
    connect(act_GuiaINSS, SIGNAL(triggered(bool)), this, SLOT(guiaInssFolha()));
    connect(act_MetaRetencaoNormal, SIGNAL(triggered(bool)), this, SLOT(metaRetencaoNormal()));
    connect(act_MetaRetencaoEstruturada, SIGNAL(triggered(bool)), this, SLOT(metaRetencaoEstruturada()));
    connect(act_OcorrenciasApuracao, SIGNAL(triggered(bool)), this, SLOT(ocorrenciasApuracao()));
    connect(act_RelacaoColaboradores, SIGNAL(triggered(bool)), this, SLOT(relacaoColaborador()));
    connect(act_HigienizacaoCrednosso, SIGNAL(triggered(bool)), this, SLOT(higienizacaoCrednosso()));
    connect(act_BlissConvertFiles, SIGNAL(triggered(bool)), this, SLOT(processarArquivosBliss()));
    connect(act_GerenciamentoREP, SIGNAL(triggered(bool)), this, SLOT(gerenciamentoREPs()));
    connect(act_Ecoclinic, SIGNAL(triggered(bool)), this, SLOT(ecoclinicRepasses()));
    connect(act_ProcessarArquivo, SIGNAL(triggered(bool)), this, SLOT(processarArquivos()));
    connect(act_TruncarArquivo, SIGNAL(triggered(bool)), this, SLOT(truncarArquivos()));
    connect(act_AlterarUsuario, SIGNAL(triggered(bool)), this, SLOT(alterarUsuario()));
    connect(act_SobreSistema, SIGNAL(triggered(bool)), this, SLOT(infoSobreSistema()));
    connect(act_ControleDePonto,SIGNAL(triggered(bool)), this, SLOT(controleDePonto()));
    connect(act_AdministracaoSistema,SIGNAL(triggered(bool)), this, SLOT(administracaoGeral()));
    connect(act_ConversaoDeArquivos,SIGNAL(triggered(bool)), this, SLOT(conversorDeArquivo()));

    //Adicionado acoes no menu de grupo financeiro
    menuFinanceiro->addAction(act_HomeInicio);
    menuFinanceiro->addAction(act_PlanoContas);
    menuFinanceiro->addAction(act_PlanoSaude);
    menuFinanceiro->addAction(act_LiquidoFolha);
    ui->toolButtonIntegracaoFinanceira->setMenu(menuFinanceiro);

    //Adicionado acoes no menu de grupo crednosso
    menuCrednosso->addAction(act_HigienizacaoCrednosso);
    menuCrednosso->addAction(act_ProcessarArquivo);
    ui->toolButtonCartaoCrednosso->setMenu(menuCrednosso);

    //Adicionado acoes no menu de grupo Recursos Humanos
    menuRH->addAction(act_HomeInicio);
    menuRH->addAction(act_GuiaINSS);
    menuRH->addAction(act_EventosFolha);
    menuRH->addAction(act_MetaRetencaoNormal);
    menuRH->addAction(act_MetaRetencaoEstruturada);
    menuRH->addAction(act_OcorrenciasApuracao);
    menuRH->addAction(act_RelacaoColaboradores);
    ui->toolButtonRecursosHumanos->setMenu(menuRH);

    //Adicionado acoes no menu de grupo Bliss Beneficios
    menuBeneficios->addAction(act_BlissConvertFiles);
    menuBeneficios->addAction(act_Ecoclinic);
    menuBeneficios->addAction(act_ControleDePonto);
    ui->toolButtonBeneficios->setMenu(menuBeneficios);

    //Adicionado acoes no menu de Controle de Ponto
    menuControlePonto->addAction(act_GerenciamentoREP);
    menuControlePonto->addAction(act_ControleDePonto);
    ui->toolButtonControlePonto->setMenu(menuControlePonto);

    //Adicionado acoes no menu de grupo Crednosso
    menuFerramentas->addAction(act_TruncarArquivo);
    menuFerramentas->addAction(act_SobreSistema);
    menuFerramentas->addAction(act_AdministracaoSistema);
    menuFerramentas->addAction(act_ConversaoDeArquivos);
    ui->toolButtonFerramentas->setMenu(menuFerramentas);

    //Adicionando acoes no menu de grupo Usuarios
    menuPropriedadesUsuario->addAction(act_AlterarSenha);
    menuPropriedadesUsuario->addAction(act_AlterarUsuario);
    ui->botaoAvatar->setMenu(menuPropriedadesUsuario);

    ui->toolButtonIntegracaoFinanceira->setEnabled(false);
    ui->toolButtonCartaoCrednosso->setEnabled(false);
    ui->toolButtonRecursosHumanos->setEnabled(false);
    ui->toolButtonBeneficios->setEnabled(false);
    ui->toolButtonControlePonto->setEnabled(false);
    ui->toolButtonFerramentas->setEnabled(false);;


    if(this->getUsuarioSessao().contains("admin") || this->getUsuarioSessao().contains("analistas")) {
        ui->toolButtonIntegracaoFinanceira->setEnabled(true);
        ui->toolButtonCartaoCrednosso->setEnabled(true);
        ui->toolButtonRecursosHumanos->setEnabled(true);
        ui->toolButtonBeneficios->setEnabled(true);
        ui->toolButtonControlePonto->setEnabled(true);
        ui->toolButtonFerramentas->setEnabled(true);
    }

    if(this->getUsuarioSessao().contains("coordenador")) {
        ui->toolButtonIntegracaoFinanceira->setEnabled(true);
        ui->toolButtonCartaoCrednosso->setEnabled(true);
        ui->toolButtonRecursosHumanos->setEnabled(true);
        ui->toolButtonBeneficios->setEnabled(true);
        ui->toolButtonControlePonto->setEnabled(true);
        ui->toolButtonFerramentas->setEnabled(true);
    }

    if(this->getUsuarioSessao().contains("recrutador")) {
        ui->toolButtonRecursosHumanos->setEnabled(true);
    }

    _flagHomeInicio   = false;
    _flagPlanoContas  = false;
    _flagPlanoSaude   = false;
    _flagLiquidoFolha = false;
    _flagGuiaINSS     = false;
    _flagEventosFolha = false;
    _flagMetaRetencaoNormal = false;
    _flagMetaRetencaoEstruturada = false;
    _flagOcorrenciasApuracao = false;
    _flagRelacaoColaborador = false;
    _flagHigienizacaoCrednosso = false;
    _flagDownloadAhgoraAFD = false;
    _flagGerenciamentoREP = false;
    _flagEcoclinicRepasses = false;
    _flagProcessarBlissBeneficios = false;
    _flagProcessarArquivos = false;
    _flagTruncarArquivos = false;
    _flagInfoSobreSistema = false;
    _flagControleDePonto = false;
    _flagConversorDeArquivo = false;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(1000);

    qRegisterMetaType<QMap<int,CadastroEmpresa*>>("__tempCadastroEmpresas");
    qRegisterMetaType<QMap<int,CadastroFilial*>>("__tempCadastroFiliais");

    controleFluxo = new ControleDAO(nullptr);
    QThread* threadDAO = new QThread(nullptr);

    connect(controleFluxo, SIGNAL(mensagemRetorno(QString,QString)), this, SLOT(mensagemRetornoUsuario(QString,QString)));
    connect(this, SIGNAL(obterCadastroDeEmpresas()), controleFluxo, SLOT(enviarCadastroDeEmpresas()));
    connect(this, SIGNAL(obterCadastroDeFiliais()), controleFluxo, SLOT(enviarCadastroDeFiliais()));
    connect(controleFluxo, SIGNAL(retornarCadastroDeEmpresas(QMap<int,CadastroEmpresa*>)), this, SLOT(definirCadastrosDeEmpresas(QMap<int,CadastroEmpresa*>)));
    connect(controleFluxo, SIGNAL(retornarCadastroDeFiliais(QMap<int,CadastroFilial*>)), this, SLOT(definirCadastrosDeFiliais(QMap<int,CadastroFilial*>)));
    connect(this, SIGNAL(finishThread()), threadDAO, SLOT(terminate()));
    connect(threadDAO, SIGNAL(finished()), controleFluxo, SLOT(deleteLater()));
    controleFluxo->moveToThread(threadDAO);
    threadDAO->start(QThread::NormalPriority);

    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(ui->botaoSair, SIGNAL(clicked(bool)), this, SLOT(sairDoSistema()));
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
    // lol -- Implementar novamente
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
        ui->tabWidget->addTab(__metaRetencaoEstruturada, QIcon(":/images/male-job-search-symbol.png"), QString("Meta de Retenção"));
        ui->tabWidget->setCurrentWidget(__metaRetencaoEstruturada);
        _flagMetaRetencaoEstruturada = true;
        _indexMetaRetencaoEstruturada = ui->tabWidget->indexOf(__metaRetencaoEstruturada);
    }
}

void Principal::ocorrenciasApuracao()
{
    if(_flagOcorrenciasApuracao) {
        ui->tabWidget->setCurrentIndex(_indexOcorrenciasApuracao);
    } else {
        __ocorrenciasApuracao = new CPOcorrenciasApuracao(this);
        __ocorrenciasApuracao->setObjectName("ocorrenciasApuracao");
        ui->tabWidget->addTab(__ocorrenciasApuracao, QIcon(":/images/wall-clock.png"), QString("Ocorrências na Apuração"));
        ui->tabWidget->setCurrentWidget(__ocorrenciasApuracao);
        _flagOcorrenciasApuracao = true;
        _indexOcorrenciasApuracao = ui->tabWidget->indexOf(__ocorrenciasApuracao);
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

void Principal::liquidoFolha()
{
    if(_flagLiquidoFolha) {
        ui->tabWidget->setCurrentIndex(_indexLiquidoFolha);
    } else {
        __liquidoFolha = new LiquidoFolha(this);
        __liquidoFolha->setObjectName("liquidoFolha");
        ui->tabWidget->addTab(__liquidoFolha, QIcon(":/images/team.png"), QString("Líquido da Folha"));
        ui->tabWidget->setCurrentWidget(__liquidoFolha);
        _flagLiquidoFolha = true;
        _indexLiquidoFolha = ui->tabWidget->indexOf(__liquidoFolha);
    }
}

void Principal::ecoclinicRepasses()
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
        __downloadAhgoraAFD = new DownloadAFD(this);
        __downloadAhgoraAFD->setObjectName("downloadAhgoraAFD");
        ui->tabWidget->addTab(__downloadAhgoraAFD, QString("Download Ahgora AFD"));
        ui->tabWidget->setCurrentWidget(__downloadAhgoraAFD);
        _flagDownloadAhgoraAFD = true;
        _indexDownloadAhgoraAFD = ui->tabWidget->indexOf(__downloadAhgoraAFD);
    }
}

void Principal::gerenciamentoREPs()
{
    if(_flagGerenciamentoREP) {
        ui->tabWidget->setCurrentIndex(_indexGerenciamentoREP);
    } else {
        __gerenciamentoREP = new GerenciamentoREP(this);
        __gerenciamentoREP->setObjectName("gerenciamentoREP");
        ui->tabWidget->addTab(__gerenciamentoREP, QIcon(":/images/vt.png"), QString("Gerenciamento REP"));
        ui->tabWidget->setCurrentWidget(__gerenciamentoREP);
        _flagGerenciamentoREP = true;
        _indexGerenciamentoREP = ui->tabWidget->indexOf(__gerenciamentoREP);
    }
}

void Principal::controleDePonto()
{
    if(_flagControleDePonto) {
        ui->tabWidget->setCurrentIndex(_indexControleDePonto);
    } else {
        __controleDePonto = new ControleDePonto(this);
        ui->tabWidget->addTab(__controleDePonto, QIcon(":/images/clock.png"), "Controle de Ponto");
        ui->tabWidget->setCurrentWidget(__controleDePonto);
        _flagControleDePonto = true;
        _indexControleDePonto = ui->tabWidget->indexOf(__controleDePonto);
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

void Principal::administracaoGeral()
{
    if(_flagAdministracaoGeral) {
        ui->tabWidget->setCurrentIndex(_indexAdministracaoGeral);
    } else {
        __administracaoGeral = new AdministracaoGeral(this, QList<Usuarios*>());
        __administracaoGeral->setObjectName("administracaoGeral");
        ui->tabWidget->addTab(__administracaoGeral, QIcon(":/images/tools.png"), QString("&Administração do Sistema"));
        ui->tabWidget->setCurrentWidget(__administracaoGeral);
        _flagAdministracaoGeral = true;
        _indexAdministracaoGeral = ui->tabWidget->indexOf(__administracaoGeral);
    }
}

void Principal::infoSobreSistema()
{
    if(_flagInfoSobreSistema) {
        ui->tabWidget->setCurrentIndex(_indexInfoSobreSistema);
    } else {
        __infoSobreSistema = new Sobre(this);
        __infoSobreSistema->setObjectName("infoSobreSeverino");
        ui->tabWidget->addTab(__infoSobreSistema, QIcon(":/images/info.png"), QString("Sobre o Severino"));
        ui->tabWidget->setCurrentWidget(__infoSobreSistema);
        _flagInfoSobreSistema = true;
        _indexInfoSobreSistema = ui->tabWidget->indexOf(__infoSobreSistema);
    }
}

void Principal::conversorDeArquivo()
{
    if(_flagConversorDeArquivo) {
        ui->tabWidget->setCurrentIndex(_indexConversorDeArquivo);
    } else {
        __conversorDeArquivo = new ConversorDeArquivo(this);
        __conversorDeArquivo->setObjectName("conversorDeArquivo");
        ui->tabWidget->addTab(__conversorDeArquivo, QIcon(":/images/file_analict.png"), QString("Conversor De Arquivo"));
        ui->tabWidget->setCurrentWidget(__conversorDeArquivo);
        _flagConversorDeArquivo = true;
        _indexConversorDeArquivo = ui->tabWidget->indexOf(__conversorDeArquivo);
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
    if(wgt->objectName().contains("ecoclinicRepasses"))
        _flagEcoclinicRepasses = false;
    if(wgt->objectName().contains("gerenciamentoAFD"))
        _flagGerenciamentoREP = false;
    if(wgt->objectName().contains("processarArquivos"))
        _flagProcessarArquivos = false;
    if(wgt->objectName().contains("truncarArquivos"))
        _flagTruncarArquivos = false;
    if(wgt->objectName().contains("infoSobreSeverino"))
        _flagProcessarArquivos = false;
    if(wgt->objectName().contains("administracaoGeral"))
        _flagAdministracaoGeral = false;
    if(wgt->objectName().contains("conversorDeArquivo"))
        _flagConversorDeArquivo = false;


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

void Principal::mensagemRetornoUsuario(const QString t, const QString msg)
{
    QMessageBox::critical(this, QString(t), QString("Nenhuma conexão com o Banco de Dados[!]\n").append(msg), QMessageBox::Ok);
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

void Principal::alterarUsuario()
{
    emit trocarUsuario();
    this->close();
}

void Principal::usuarioAutenticado(QString u)
{
    this->setUsuarioSessao(u);
}

void Principal::updateStatusConnection()
{
    if(!this->getStatusConexaoSenior()) {
        if(movieStatusSenior->state() == QMovie::NotRunning) {
            ui->statusAplicacao->setToolTip(QString("Obtendo informações da conexão com a base de dados Senior!"));
            ui->statusAplicacao->setMovie(movieStatusSenior);
            movieStatusSenior->start();
        }
    } else {
        ui->statusAplicacao->setToolTip(QString("Conectado!"));
        timerStatusSenior->stop();
        movieStatusSenior->stop();

        this->statusConexao(true);
        emit obterCadastroDeEmpresas();
    }
}

void Principal::encerrarSessao()
{
    this->close();
    this->deleteLater();
}

void Principal::sairDoSistema()
{
    emit fecharSistema();
}

void Principal::finalizarThread()
{
    if(!this->getMapFiliais().isEmpty() && !this->getMapEmpresas().isEmpty())
        emit finishThread();
}

bool Principal::getStatusConexaoSenior() const
{
    return statusConexaoSenior;
}

void Principal::setStatusConexaoSenior(bool value)
{
    statusConexaoSenior = value;
}

QString Principal::getUsuarioSessao() const
{
    return usuarioSessao;
}

void Principal::setUsuarioSessao(const QString &value)
{
    usuarioSessao = value;
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
        ui->statusAplicacao->setPixmap(QPixmap(":/images/database_on.png"));
        emit obterCadastroDeEmpresas();
        emit obterCadastroDeFiliais();
    } else {
        ui->statusAplicacao->setPixmap(QPixmap(":/images/database_alert.png"));
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
        ui->toolButtonCartaoCrednosso->setFocus();
        ui->toolButtonCartaoCrednosso->click();
        return;
    }
    if(event->key() == Qt::Key_F4) {
        ui->toolButtonRecursosHumanos->setFocus();
        ui->toolButtonRecursosHumanos->click();
        return;
    }
    if(event->key() == Qt::Key_F5) {
        ui->toolButtonBeneficios->setFocus();
        ui->toolButtonBeneficios->click();
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
