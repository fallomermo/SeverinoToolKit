#include "principal.h"
#include "ui_principal.h"

Principal::Principal(QWidget *parent) : QWidget(parent), ui(new Ui::Principal)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("Severino Tools | Build 1.1.0 ").append(QT_VERSION_MINOR));
    local.setDefault(QLocale(QLocale::Portuguese, QLocale::Brazil));
    setTimeSession(QTime::currentTime());

    aplicarDefinicoesGerais();
}

Principal::~Principal()
{
    delete ui;
}

void Principal::aplicarDefinicoesGerais()
{
    qApp->setWindowIcon(QIcon(":/images/iconeSeverino.png"));
    atualizarTema();
    ui->toolButton->setPopupMode(QToolButton::InstantPopup);
    QAction *actionPlanoContas = new QAction(QIcon(":/images/tax.png"), "&Plano de Contas",ui->toolButton);
    QAction *actionPlanoSaude = new QAction(QIcon(":/images/heart.png"), "&Plano de Saúde",ui->toolButton);
    QAction *actionLiquidoFolha = new QAction(QIcon(":/images/water-drop.png"), "&Líquido Folha [No]",ui->toolButton);
    QAction *actionGuiaINSS = new QAction(QIcon(":/images/piggy-bank.png"), "&Guia INSS Rateio",ui->toolButton);
    QAction *actionEventosFolha = new QAction(QIcon(":/images/"),"&Eventos Folha [No]",ui->toolButton);
    QAction *actionMetaRetencao = new QAction(QIcon(":/images/stocks-graphic-with-a-magnifier-tool.png"),"&Meta Retenção [No]",ui->toolButton);
    QAction *actionRelacaoColaboradores = new QAction(QIcon(":/images/team.png"), "&Relação de Colaboradores",ui->toolButton);
    QMenu *menu = new QMenu(ui->toolButton);

    connect(actionPlanoContas, SIGNAL(triggered(bool)), this, SLOT(planoContas()));
    connect(actionPlanoSaude, SIGNAL(triggered(bool)), this, SLOT(planoSaude()));
    connect(actionGuiaINSS, SIGNAL(triggered(bool)), this, SLOT(guiaInssFolha()));
    connect(actionRelacaoColaboradores, SIGNAL(triggered(bool)), this, SLOT(relacaoColaborador()));

    menu->addAction(actionPlanoContas);
    menu->addAction(actionPlanoSaude);
    menu->addAction(actionLiquidoFolha);
    menu->addAction(actionGuiaINSS);
    menu->addAction(actionEventosFolha);
    menu->addAction(actionMetaRetencao);
    menu->addAction(actionRelacaoColaboradores);
    ui->toolButton->setMenu(menu);

    _flagPlanoContas  = false;
    _flagPlanoSaude   = false;
    _flagLiquidoFolha = false;
    _flagGuiaINSS     = false;
    _flagEventosFolha = false;
    _flagMetaRetencao = false;
    _flagRelacaoColaborador = false;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(1000);

    ctrl = new ControleDAO(this);
    connect(ctrl, SIGNAL(mensagemRetorno(QString)), this, SLOT(mensagemRetornoUsuario(QString)));
    if(ctrl->abrirConexao()){
        mapEmpresas = ctrl->getEmpresas();
        mapFiliais = ctrl->getFiliais();
    } else {
        QMessageBox::critical(this, tr("Conexão Banco de Dados"), QString("Não foi possível estabelecer conexão com o banco de dados!"), QMessageBox::Ok);
    }

    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(ui->botaoSair, SIGNAL(clicked(bool)), this, SLOT(close()));
    ui->campoUsuario->setText("Convidado");
}

void Principal::planoContas()
{
    if(_flagPlanoContas) {
        ui->tabWidget->setCurrentIndex(_indexPlanoContas);
    } else {
        QMap<int, CadastroEmpresa*> me = mapEmpresas;
        QMap<int, CadastroFilial*> mf = mapFiliais;
        pc = new PlanoDeContas(this, me, mf);
        pc->setObjectName("planoContas");
        ui->tabWidget->addTab(pc, QIcon(":/images/tax.png"), QString("Plano de Contas"));
        ui->tabWidget->setCurrentWidget(pc);
        _flagPlanoContas = true;
        _indexPlanoContas = ui->tabWidget->indexOf(pc);
    }
}

void Principal::planoSaude()
{
    if(_flagPlanoSaude) {
        ui->tabWidget->setCurrentIndex(_indexPlanoSaude);
    } else {
        ps = new PlanoSaude(this);
        ps->setObjectName("planoSaude");
        ui->tabWidget->addTab(ps, QIcon(":/images/heart.png"), QString("Plano de Saúde"));
        ui->tabWidget->setCurrentWidget(ps);
        _flagPlanoSaude = true;
        _indexPlanoSaude = ui->tabWidget->indexOf(ps);
    }
}

void Principal::guiaInssFolha()
{
    if(_flagGuiaINSS) {
        ui->tabWidget->setCurrentIndex(_indexGuiaINSS);
    } else {
        guiaInss = new GuiaINSSFolha(this, mapEmpresas, mapFiliais);
        guiaInss->setObjectName("guiaINSSFolha");
        ui->tabWidget->addTab(guiaInss, QIcon(":/images/piggy-bank.png"), QString("Guia INSS Folha"));
        ui->tabWidget->setCurrentWidget(guiaInss);
        _flagGuiaINSS = true;
        _indexGuiaINSS = ui->tabWidget->indexOf(guiaInss);
    }
}

void Principal::relacaoColaborador()
{
    if(_flagRelacaoColaborador) {
        ui->tabWidget->setCurrentIndex(_indexRelacaoColaborador);
    } else {
        relCol = new RelacaoColaborador(this, mapEmpresas, mapFiliais);
        relCol->setObjectName("relacaoColaborador");
        ui->tabWidget->addTab(relCol, QIcon(":/images/team.png"), QString("Relação de Colaboradores"));
        ui->tabWidget->setCurrentWidget(relCol);
        _flagRelacaoColaborador = true;
        _indexRelacaoColaborador = ui->tabWidget->indexOf(relCol);
    }
}

void Principal::closeTab(int i)
{
    QWidget *wgt = ui->tabWidget->widget(i);
    if(wgt->objectName().contains("planoContas"))
        _flagPlanoContas = false;
    if(wgt->objectName().contains("planoSaude"))
        _flagPlanoSaude = false;
    if(wgt->objectName().contains("guiaINSS"))
        _flagGuiaINSS = false;
    if(wgt->objectName().contains("liquidoFolha"))
        _flagLiquidoFolha = false;
    if(wgt->objectName().contains("relacaoColaborador"))
        _flagRelacaoColaborador = false;

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

QTime Principal::getTimeSession() const
{
    return timeSession;
}

void Principal::setTimeSession(const QTime &value)
{
    timeSession = value;
}

void Principal::closeEvent(QCloseEvent *event)
{
    if(QMessageBox::question(this, tr("Plano de Contas :: Sair"), QString("Deseja realmente sair?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
        event->ignore();
}

void Principal::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F2)
        planoContas();
    if(event->key() == Qt::Key_F3)
        planoSaude();
    if(event->key() == Qt::Key_F4)
        guiaInssFolha();
}
