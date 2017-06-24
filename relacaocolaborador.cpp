#include "relacaocolaborador.h"
#include "ui_relacaocolaborador.h"

RelacaoColaborador::RelacaoColaborador(QWidget *parent) : QWidget(parent), ui(new Ui::RelacaoColaborador)
{
    ui->setupUi(this);
}

RelacaoColaborador::RelacaoColaborador(QWidget *parent, QMap<int, CadastroEmpresa *> ce, QMap<int, CadastroFilial *> cf) :
    QWidget(parent),
    ui(new Ui::RelacaoColaborador)
{
    ui->setupUi(this);
    this->setMapEmpresas(ce);
    this->setMapFiliais(cf);
    ui->campoID_Empresa->setPlaceholderText(QString("Código da Empresa..."));
    ui->campoID_Filial->setPlaceholderText(QString("Código da Filial..."));
    ui->dataReferencia->setDate(QDate::currentDate());
    QString __data = ui->dataReferencia->date().toString("d' de 'MMMM' de 'yyyy");
    ui->campoDescricaoData->setText(__data);
    connect(ui->campoID_Empresa, SIGNAL(returnPressed()), this, SLOT(retornaCadastroEmpresa()));
    connect(ui->campoID_Filial, SIGNAL(returnPressed()), this, SLOT(retornaCadastroFilial()));
    QAction *_act_emp = ui->campoID_Empresa->addAction(QIcon(":/images/search.png"), QLineEdit::TrailingPosition);
    QAction *_act_fil = ui->campoID_Filial->addAction(QIcon(":/images/search.png"), QLineEdit::TrailingPosition);
    ui->campoPesquisarObjetosTabela->addAction(QIcon(":/images/search.png"), QLineEdit::TrailingPosition);
    ui->campoPesquisarObjetosTabela->setPlaceholderText(QString("Pesquisar itens..."));
    connect(ui->campoPesquisarObjetosTabela, SIGNAL(textChanged(QString)), this, SLOT(filtroItemTabela(QString)));
    connect(_act_emp, SIGNAL(triggered(bool)), this, SLOT(pesquisarEmpresa()));
    connect(_act_fil, SIGNAL(triggered(bool)), this, SLOT(pesquisarFilial()));
    connect(ui->botaoProcessar, SIGNAL(clicked(bool)), this, SLOT(getDatatable()));
    connect(ui->botaoExportar, SIGNAL(clicked(bool)), this, SLOT(exportarParaExcel()));
    connect(ui->tableWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(exibirNumeroRegistros(QModelIndex)));
    connect(ui->tableWidget, SIGNAL(activated(QModelIndex)), this, SLOT(exibirNumeroRegistros(QModelIndex)));
    connect(ui->dataReferencia, SIGNAL(editingFinished()), this, SLOT(exibirDataCompleta()));
    connect(ui->dataReferencia, SIGNAL(dateChanged(QDate)), this, SLOT(exibirDataCompleta(QDate)));
    connect(ui->dataReferencia, SIGNAL(editingFinished()), this, SLOT(focusDataReferencia()));

    ui->campoID_Empresa->setToolTip(QString("Dica! \nPara que sejam exibidas as informações dos colaboradores \n"
                                            "nas empresas antes da reestruturação de primeiro \n"
                                            "de janeiro de 2013, é preciso deixar os campos de filtro \n"
                                            "de código da empresa e código da filial em branco e\n"
                                            "selecionar uma data anterior a esse dia!"));

    QStringList labels = QStringList() << "Codigo da Empresa"
                                       << "Empresa"
                                       << "Codigo da Filial"
                                       << "Filial"
                                       << "Cidade Região"
                                       << "CNPJ"
                                       << "Matricula"
                                       << "CPF"
                                       << "PIS"
                                       << "Nome"
                                       << "Data de Admissao"
                                       << "Data de Nascimento"
                                       << "Codigo de Vinculo"
                                       << "Tabela de Organograma"
                                       << "Numero do Local"
                                       << "Hierarquia de Local"
                                       << "Setor"
                                       << "Estrutura de Cargos"
                                       << "Codigo do Cargo"
                                       << "Cargo"
                                       << "Tipo de Salário"
                                       << "Salário";
    ui->tableWidget->setColumnCount(labels.count());
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    ui->tableWidget->resizeColumnsToContents();
}

RelacaoColaborador::~RelacaoColaborador()
{
    delete ui;
}

QMap<int, CadastroEmpresa *> RelacaoColaborador::getMapEmpresas() const
{
    return mapEmpresas;
}

void RelacaoColaborador::setMapEmpresas(const QMap<int, CadastroEmpresa *> &value)
{
    mapEmpresas = value;
}

QMap<int, CadastroFilial *> RelacaoColaborador::getMapFiliais() const
{
    return mapFiliais;
}

void RelacaoColaborador::setMapFiliais(const QMap<int, CadastroFilial *> &value)
{
    mapFiliais = value;
}

void RelacaoColaborador::filtroItemTabela(QString filter)
{
    for( int i = 0; i < ui->tableWidget->rowCount(); ++i ) {
        bool match = false;
        for( int j = 0; j < ui->tableWidget->columnCount(); ++j ) {
            QTableWidgetItem *item = ui->tableWidget->item( i, j );
            if( item->text().contains(filter) ) {
                match = true;
                break;
            }
        }
        ui->tableWidget->setRowHidden( i, !match );
    }
}

void RelacaoColaborador::pesquisarEmpresa()
{
    Pesquisar * p = new Pesquisar(this, getMapEmpresas(), getMapFiliais(), "", 1);
    p->setWindowModality(Qt::ApplicationModal);
    p->setWindowFlags(Qt::Window);
    p->setWindowTitle("Selecionar Empresa");
    connect(p, SIGNAL(getEmpresa(QString)), this, SLOT(setEmpresa(QString)));
    p->show();
}

void RelacaoColaborador::setEmpresa(QString e)
{
    ui->campoID_Empresa->setText(e);
    retornaCadastroEmpresa();
}

void RelacaoColaborador::pesquisarFilial()
{
    Pesquisar * p = new Pesquisar(this, getMapEmpresas(), getMapFiliais(), ui->campoID_Empresa->text(), 2);
    p->setWindowModality(Qt::ApplicationModal);
    p->setWindowFlags(Qt::Window);
    p->setWindowTitle("Selecionar Filial");
    connect(p, SIGNAL(getFilial(QString)), this, SLOT(setFilial(QString)));
    p->show();
}

void RelacaoColaborador::setFilial(QString f)
{
    ui->campoID_Filial->setText(f);
    retornaCadastroFilial(f);
}

void RelacaoColaborador::retornaCadastroEmpresa()
{
    QMapIterator<int, CadastroEmpresa*> mi(getMapEmpresas());
    QString _nomeEmpresa;

    while (mi.hasNext()) {
        mi.next();
        CadastroEmpresa *_cemp = new CadastroEmpresa;
        _cemp = mi.value();

        if(_cemp->getID_Empresa() == ui->campoID_Empresa->text()) {
            _nomeEmpresa = _cemp->getEmpresa();
        }
    }

    if(_nomeEmpresa.isEmpty()) {
        QMessageBox::critical(this, tr("Cadastro Empresa [!]"), QString("Empresa não encontrada [!]"), QMessageBox::Ok);
        ui->campoID_Empresa->setFocus();
    } else {
        ui->campoDescricaoEmpresa->setText(_nomeEmpresa);
        ui->campoID_Filial->setFocus();
    }
}

void RelacaoColaborador::retornaCadastroFilial()
{
    QString _ID_Filial = ui->campoID_Filial->text();
    if(ui->campoID_Empresa->text().isEmpty()) {
        QMessageBox::critical(this, tr("Seleção de Filtro"), QString("Nenhuma Empresa Selecionada"), QMessageBox::Ok);
        ui->campoID_Empresa->setFocus();
    } else {
        QMapIterator<int, CadastroFilial*> mi(getMapFiliais());
        QString _nomeFilial;

        while (mi.hasNext()) {
            mi.next();
            CadastroFilial *_cfil = new CadastroFilial;
            _cfil = mi.value();
            QString _ID_Empresa = ui->campoID_Empresa->text();
            if(_cfil->getID_Empresa() == _ID_Empresa && _cfil->getID_Filial() == _ID_Filial) {
                _nomeFilial = _cfil->getFilial();
            }
        }

        if(_nomeFilial.isEmpty()) {
            QMessageBox::critical(this, tr("Cadastro Filial [!]"), QString("Filial Inválida [!]"), QMessageBox::Ok);
            ui->campoID_Filial->setFocus();
        } else {
            ui->campoID_Filial->setText(_ID_Filial);
            ui->campoDescricaoFilial->setText(_nomeFilial);
            ui->dataReferencia->setFocus();
        }
    }
}

void RelacaoColaborador::retornaCadastroFilial(QString _ID_Filial)
{
    if(ui->campoID_Empresa->text().isEmpty()) {
        QMessageBox::critical(this, tr("Seleção de Filtro"), QString("Nenhuma Empresa Selecionada"), QMessageBox::Ok);
        ui->campoID_Empresa->setFocus();
    } else {
        QMapIterator<int, CadastroFilial*> mi(getMapFiliais());
        QString _nomeFilial;

        while (mi.hasNext()) {
            mi.next();
            CadastroFilial *_cfil = new CadastroFilial;
            _cfil = mi.value();
            QString _ID_Empresa = ui->campoID_Empresa->text();
            if(_cfil->getID_Empresa() == _ID_Empresa && _cfil->getID_Filial() == _ID_Filial) {
                _nomeFilial = _cfil->getFilial();
            }
        }

        if(_nomeFilial.isEmpty()) {
            QMessageBox::critical(this, tr("Cadastro Filial [!]"), QString("Filial Inválida [!]"), QMessageBox::Ok);
            ui->campoID_Filial->setFocus();
        } else {
            ui->campoID_Filial->setText(_ID_Filial);
            ui->campoDescricaoFilial->setText(_nomeFilial);
            ui->dataReferencia->setFocus();
        }
    }
}

void RelacaoColaborador::focusDataReferencia()
{
    ui->botaoProcessar->setFocus();
}

void RelacaoColaborador::getDatatable()
{
    QProgressDialog progresso("Trabalhando em sua requisição, aguarde...", "Cancelar", 0, 100, this, Qt::Dialog);
    progresso.setWindowModality(Qt::ApplicationModal);
    progresso.setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    progresso.setModal( true );
    progresso.setVisible( true );
    progresso.setAutoClose( true );

    qApp->processEvents(QEventLoop::DialogExec);
    progresso.setRange(0,1);
    progresso.setValue(0);
    progresso.setRange(0,0);

    controle = new ControleDAO(this);
    QMap<int, CadastroColaborador*> __tempMap = controle->getColaboradoresAtivos(
                ui->campoID_Empresa->text(),
                ui->campoID_Filial->text(),
                ui->dataReferencia->date());
    progresso.setLabelText("Processando dados...");

    if(__tempMap.isEmpty()) {
        QMessageBox::information(this, tr("Relação de Colaboradores"), QString("Nenhuma informação encontrada!"), QMessageBox::Ok);
        return;
    }

    QMapIterator<int, CadastroColaborador*> __mapIterator(__tempMap);
    progresso.setMaximum(__tempMap.count());

    ui->tableWidget->setRowCount(__tempMap.count());
    int linha = 0;
    while (__mapIterator.hasNext()) {
        __mapIterator.next();
        progresso.setValue(__mapIterator.key());
        CadastroColaborador *cadastro = __mapIterator.value();
        inserirLinhaTabela(linha, ui->tableWidget->columnCount(), cadastro);
        linha++;
    }
    ui->campoItemSelecionado->setText("Total de Registros");
    ui->campoTotalRegistros->setText(QString::number(__tempMap.count()));
    ui->tableWidget->resizeColumnsToContents();
}

void RelacaoColaborador::inserirItemTabela(int r, int c, QString sValue)
{
    ui->tableWidget->setItem(r,c,new QTableWidgetItem(sValue));
}

void RelacaoColaborador::inserirItemTabela(int r, int c, QDate dtValue)
{
    ui->tableWidget->setItem(r,c,new QTableWidgetItem(QDate(dtValue).toString("dd/MM/yyyy")));
}

void RelacaoColaborador::inserirItemTabela(int r, int c, double dValue)
{
    QTableWidgetItem *item = new QTableWidgetItem(QString("%L1").arg(dValue, 12, 'f', 4));
    item->setTextAlignment(Qt::AlignRight);
    ui->tableWidget->setItem(r,c,item);
}

void RelacaoColaborador::inserirItemTabela(int r, int c, int iValue)
{
    QTableWidgetItem *item = new QTableWidgetItem(QString::number(iValue));
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(r,c,item);
}

void RelacaoColaborador::inserirLinhaTabela(int linha, int nrColunas, CadastroColaborador *colaborador)
{
    for (int coluna = 0; coluna < nrColunas; ++coluna) {
        if(coluna == 0)
            inserirItemTabela(linha, coluna, colaborador->getCodigoDaEmpresa() );
        if(coluna == 1)
            inserirItemTabela(linha, coluna, colaborador->getEmpresa() );
        if(coluna == 2)
            inserirItemTabela(linha, coluna, colaborador->getCodigoDaFilial() );
        if(coluna == 3)
            inserirItemTabela(linha, coluna, colaborador->getFilial() );
        if(coluna == 4)
            inserirItemTabela(linha, coluna, colaborador->getCidadeRegiao() );
        if(coluna == 5)
            inserirItemTabela(linha, coluna, colaborador->getCNPJ() );
        if(coluna == 6)
            inserirItemTabela(linha, coluna, colaborador->getMatricula() );
        if(coluna == 7)
            inserirItemTabela(linha, coluna, colaborador->getCPF() );
        if(coluna == 8)
            inserirItemTabela(linha, coluna, colaborador->getPIS() );
        if(coluna == 9)
            inserirItemTabela(linha, coluna, colaborador->getNome() );
        if(coluna == 10)
            inserirItemTabela(linha, coluna, colaborador->getDataDeAdmissao() );
        if(coluna == 11)
            inserirItemTabela(linha, coluna, colaborador->getDataDeNascimento() );
        if(coluna == 12)
            inserirItemTabela(linha, coluna, colaborador->getCodigoDeVinculo() );
        if(coluna == 13)
            inserirItemTabela(linha, coluna, colaborador->getTabelaDeOrganograma() );
        if(coluna == 14)
            inserirItemTabela(linha, coluna, colaborador->getNumeroDoLocal() );
        if(coluna == 15)
            inserirItemTabela(linha, coluna, colaborador->getHierarquiaDeLocal() );
        if(coluna == 16)
            inserirItemTabela(linha, coluna, colaborador->getSetor() );
        if(coluna == 17)
            inserirItemTabela(linha, coluna, colaborador->getEstruturaDeCargos() );
        if(coluna == 18)
            inserirItemTabela(linha, coluna, colaborador->getCodigoDoCargo() );
        if(coluna == 19)
            inserirItemTabela(linha, coluna, colaborador->getCargo() );
        if(coluna == 20)
            inserirItemTabela(linha, coluna, colaborador->getTipoDeSalario() );
        if(coluna == 21)
            inserirItemTabela(linha, coluna, colaborador->getSalario() );
    }
}

void RelacaoColaborador::exportarParaExcel()
{
    QString tituloArquivo = "";
    if(ui->campoDescricaoEmpresa->text().isEmpty() || ui->campoDescricaoFilial->text().isEmpty())
        tituloArquivo = "FullData";
    else
        tituloArquivo = ui->campoDescricaoFilial->text().replace(' ','_');

    QString __nomeArquivo = "/"+tituloArquivo+"_"+ui->dataReferencia->text().replace('/','-');

    ExportarArquivo *exp = new ExportarArquivo(this, ui->tableWidget);
    connect(exp, SIGNAL(mensagemRetorno(QString)), this, SLOT(caixaMensagemUsuario(QString)));
    exp->exportar(__nomeArquivo);
}

void RelacaoColaborador::exibirNumeroRegistros(QModelIndex i)
{
    if(i.column() > 21)
        return;

    QTableWidgetItem *_item = ui->tableWidget->item(i.row(), i.column());
    ui->campoItemSelecionado->setText(_item->text());

    int iValue = 0;
    QString sValue = _item->text();
    for (int linha = 0; linha < ui->tableWidget->rowCount(); ++linha) {
        for (int coluna = 0; coluna <= i.column(); ++coluna) {
            if(sValue.contains(ui->tableWidget->item(linha, coluna)->text())) {
                iValue++;
            }
        }
    }
    ui->campoTotalRegistros->setText(QString::number(iValue));
}

void RelacaoColaborador::exibirDataCompleta()
{
    ui->campoDescricaoData->setText(ui->dataReferencia->date().toString("d' de 'MMMM' de 'yyyy"));
}

void RelacaoColaborador::exibirDataCompleta(QDate d)
{
    ui->campoDescricaoData->setText(d.toString("d' de 'MMMM' de 'yyyy"));
}

void RelacaoColaborador::caixaMensagemUsuario(QString msg)
{
    QMessageBox::information(this, tr("Exportação de Dados"), QString(msg), QMessageBox::Ok);
}
