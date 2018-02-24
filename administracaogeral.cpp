#include "administracaogeral.h"
#include "ui_administracaogeral.h"

AdministracaoGeral::AdministracaoGeral(QWidget *parent, QList<Usuarios *> usr) : QWidget(parent), usuariosCadastrados(usr), ui(new Ui::AdministracaoGeral)
{
    ui->setupUi(this);

    controle = new ControleDAO(Q_NULLPTR);
    thread_DB = new QThread(Q_NULLPTR);
    controle->moveToThread(thread_DB);
    thread_DB->start(QThread::NormalPriority);

    connect(ui->botaoNovoUsuario, SIGNAL(clicked(bool)), this, SLOT(novoUsuario()));
    connect(ui->botaoAlterarUsuario, SIGNAL(clicked(bool)), this, SLOT(alterarUsuario()));
    connect(ui->botaoExcluirUsuario, SIGNAL(clicked(bool)), this, SLOT(excluirUsuario()));
    connect(ui->botaoSalvar, SIGNAL(clicked(bool)), this, SLOT(salvarUsuario()));
    connect(ui->botaoCancelar, SIGNAL(clicked(bool)), this, SLOT(cancelar()));

    connect(this, SIGNAL(inserirUsuario(Usuarios*)), controle, SLOT(inserirUsuario(Usuarios*)));
    connect(this, SIGNAL(removerUsuario(Usuarios*)), controle, SLOT(removerUsuario(Usuarios*)));
    connect(this, SIGNAL(alterarUsuario(Usuarios*)), controle, SLOT(alterarUsuario(Usuarios*)));
    connect(ui->tableWidget, SIGNAL(cellActivated(int,int)), this, SLOT(usuarioSelecionado(int,int)));
    connect(ui->tableWidget, SIGNAL(cellPressed(int,int)), this, SLOT(usuarioSelecionado(int,int)));
    connect(ui->tableWidget, SIGNAL(cellEntered(int,int)), this, SLOT(usuarioSelecionado(int,int)));

    QStringList labels = QStringList() << "ID" << "Usuário" << "Descrição" << "Perfil" << "Data de Criação" << "Data de Alteração";
    ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->tableWidget->setColumnCount(labels.count());
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    ui->tableWidget->resizeColumnsToContents();

    this->listarUsuarios(usuariosCadastrados);
}

AdministracaoGeral::~AdministracaoGeral()
{
    delete ui;
}

void AdministracaoGeral::novoUsuario()
{
    ui->campoUsuario->setEnabled(true);
    ui->campoSenha->setEnabled(true);
    ui->campoNome->setEnabled(true);
}

void AdministracaoGeral::alterarUsuario()
{
    ui->botaoSalvar->setEnabled(false);
    ui->botaoCancelar->setEnabled(false);

    Usuarios* u = new Usuarios(nullptr);
    u->setUsuario(ui->campoUsuario->text());
    u->setUsuario(ui->campoUsuario->text());
}

void AdministracaoGeral::excluirUsuario()
{

}

void AdministracaoGeral::salvarUsuario()
{
    this->alterarUsuario();
}

void AdministracaoGeral::cancelar()
{

}

void AdministracaoGeral::statusBotoesAtivos()
{
    ui->botaoSalvar->setEnabled(true);
    ui->botaoCancelar->setEnabled(true);
}

void AdministracaoGeral::statusBotoesInativos()
{
    ui->botaoSalvar->setEnabled(false);
    ui->botaoCancelar->setEnabled(false);
}

void AdministracaoGeral::listarUsuarios(QList<Usuarios *> usr)
{
    int linha = 0;
    ui->tableWidget->setRowCount(usr.count());
    foreach (Usuarios* u, usr) {
        Usuarios* __u = u;
        QTableWidgetItem *i_id      = new QTableWidgetItem(__u->getId());
        QTableWidgetItem *i_usuario = new QTableWidgetItem(__u->getUsuario());
        QTableWidgetItem *i_descric = new QTableWidgetItem(__u->getDescricao());
        QTableWidgetItem *i_perfil  = new QTableWidgetItem(__u->getPerfil());
        QTableWidgetItem *i_datacri = new QTableWidgetItem(__u->getDataCriacao().toString("dd/MM/yyyy"));
        QTableWidgetItem *i_dataalt = new QTableWidgetItem(__u->getDataAlteracao().toString("dd/MM/yyyy"));
        ui->tableWidget->setItem(linha, 0, i_id);
        ui->tableWidget->setItem(linha, 1, i_usuario);
        ui->tableWidget->setItem(linha, 2, i_descric);
        ui->tableWidget->setItem(linha, 3, i_perfil);
        ui->tableWidget->setItem(linha, 4, i_datacri);
        ui->tableWidget->setItem(linha, 5, i_dataalt);
        ui->tableWidget->resizeColumnsToContents();
        linha++;
    }
}

void AdministracaoGeral::usuarioSelecionado(QModelIndex idx)
{
    QTableWidgetItem *i_lg = ui->tableWidget->item(idx.row(), 1);
    QTableWidgetItem *i_nm = ui->tableWidget->item(idx.row(), 2);
    QTableWidgetItem *i_dc = ui->tableWidget->item(idx.row(), 3);
    QTableWidgetItem *i_da = ui->tableWidget->item(idx.row(), 4);

    ui->campoUsuario->setText(i_lg->text());
    ui->campoNome->setText(i_nm->text());

    QString d_c = i_dc->text().replace('/', '-');
    QString d_a = i_da->text().replace('/', '-');
    ui->campoDataCriacao->setDate(QDate::fromString(d_c, QString("dd-MM-yyyy")));
    ui->campoDataAlteracao->setDate(QDate::fromString(d_a, QString("dd-MM-yyyy")));

    QStringList p = QStringList() << "Administrador";
    ui->comboBoxPerfil->clear();
    ui->comboBoxPerfil->addItems(p);
    ui->campoNome->setFocus();
}

void AdministracaoGeral::usuarioSelecionado(int linha, int coluna)
{
    coluna = 0;
    QTableWidgetItem *i_lg = ui->tableWidget->item(linha, coluna+1);
    QTableWidgetItem *i_nm = ui->tableWidget->item(linha, coluna+2);
    QTableWidgetItem *i_dc = ui->tableWidget->item(linha, coluna+3);
    QTableWidgetItem *i_da = ui->tableWidget->item(linha, coluna+4);

    ui->campoUsuario->setText(i_lg->text());
    ui->campoNome->setText(i_nm->text());

    QString d_c = i_dc->text().replace('/', '-');
    QString d_a = i_da->text().replace('/', '-');
    ui->campoDataCriacao->setDate(QDate::fromString(d_c, QString("dd-MM-yyyy")));
    ui->campoDataAlteracao->setDate(QDate::fromString(d_a, QString("dd-MM-yyyy")));

    QStringList p = QStringList() << "Administrador";
    ui->comboBoxPerfil->clear();
    ui->comboBoxPerfil->addItems(p);
    ui->campoNome->setFocus();
}
QList<Usuarios *> AdministracaoGeral::getUsuariosCadastrados() const
{
    return usuariosCadastrados;
}

void AdministracaoGeral::setUsuariosCadastrados(const QList<Usuarios *> &value)
{
    usuariosCadastrados = value;
}

