#include "guiainssfolha.h"
#include "ui_guiainssfolha.h"

GuiaINSSFolha::GuiaINSSFolha(QWidget *parent) : QWidget(parent), ui(new Ui::GuiaINSSFolha)
{
    ui->setupUi(this);
    ui->periodoInicial->setDate(QDateTime::currentDateTime().date());
    ui->periodoFinal->setDate(QDateTime::currentDateTime().date());
    connect(ui->periodoInicial, SIGNAL(editingFinished()), this, SLOT(focusPeriodoInicial()));
    connect(ui->periodoFinal, SIGNAL(editingFinished()), this, SLOT(focusPeriodoFinal()));
    ui->campoPesquisarObjetosTabela->addAction(QIcon(":/images/search.png"), QLineEdit::TrailingPosition);
    ui->campoPesquisarObjetosTabela->setPlaceholderText(QString("Pesquisar Itens na Tabela"));
    QAction *_act_emp = ui->campoID_Empresa->addAction(QIcon(":/images/search.png"), QLineEdit::TrailingPosition);
    connect(_act_emp, SIGNAL(triggered(bool)), this, SLOT(pesquisarEmpresa()));
    QAction *_act_fil = ui->campoID_Filial->addAction(QIcon(":/images/search.png"), QLineEdit::TrailingPosition);
    connect(_act_fil, SIGNAL(triggered(bool)), this, SLOT(pesquisarFilial()));
    connect(ui->campoPesquisarObjetosTabela, SIGNAL(textChanged(QString)), this, SLOT(filtroItemTabela(QString)));
    connect(ui->campoID_Empresa, SIGNAL(returnPressed()), this, SLOT(retornaCadastroEmpresa()));
    connect(ui->campoID_Filial, SIGNAL(returnPressed()), this, SLOT(retornaCadastroFilial(QString)));
    connect(ui->botaoProcessar, SIGNAL(clicked(bool)), this, SLOT(getDatatable()));
    connect(ui->botaoExportar, SIGNAL(clicked(bool)), this, SLOT(exportarParaExcel()));

    QStringList labels = QStringList() << "ID Empresa"
                                       << "Empresa"
                                       << "ID Filial"
                                       << "Filial"
                                       << "CNPJ"
                                       << "Cidade Região"
                                       << "Cálculo"
                                       << "Competência"
                                       << "Setor"
                                       << "Código do Evento"
                                       << "Descrição do Evento"
                                       << "Tipo Evento"
                                       << "Valor";
    ui->tableWidget->setColumnCount(labels.count());
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    ui->tableWidget->resizeColumnsToContents();
}

GuiaINSSFolha::GuiaINSSFolha(QWidget *parent, QMap<int, CadastroEmpresa *> ce, QMap<int, CadastroFilial *> cf) :
    QWidget(parent),
    ui(new Ui::GuiaINSSFolha)
{
    ui->setupUi(this);
    this->setMapEmpresas(ce);
    this->setMapFiliais(cf);
    ui->periodoInicial->setDate(QDateTime::currentDateTime().date());
    ui->periodoFinal->setDate(QDateTime::currentDateTime().date());
    connect(ui->periodoInicial, SIGNAL(editingFinished()), this, SLOT(focusPeriodoInicial()));
    connect(ui->periodoFinal, SIGNAL(editingFinished()), this, SLOT(focusPeriodoFinal()));
    ui->campoPesquisarObjetosTabela->addAction(QIcon(":/images/search.png"), QLineEdit::TrailingPosition);
    ui->campoPesquisarObjetosTabela->setPlaceholderText(QString("Pesquisar Itens na Tabela"));
    QAction *_act_emp = ui->campoID_Empresa->addAction(QIcon(":/images/search.png"), QLineEdit::TrailingPosition);
    connect(_act_emp, SIGNAL(triggered(bool)), this, SLOT(pesquisarEmpresa()));
    QAction *_act_fil = ui->campoID_Filial->addAction(QIcon(":/images/search.png"), QLineEdit::TrailingPosition);
    connect(_act_fil, SIGNAL(triggered(bool)), this, SLOT(pesquisarFilial()));
    connect(ui->campoPesquisarObjetosTabela, SIGNAL(textChanged(QString)), this, SLOT(filtroItemTabela(QString)));
    connect(ui->campoID_Empresa, SIGNAL(returnPressed()), this, SLOT(pesquisarEmpresa()));
    connect(ui->campoID_Filial, SIGNAL(returnPressed()), this, SLOT(pesquisarFilial()));
    connect(ui->botaoProcessar, SIGNAL(clicked(bool)), this, SLOT(getDatatable()));
    connect(ui->botaoExportar, SIGNAL(clicked(bool)), this, SLOT(exportarParaExcel()));

    QStringList labels = QStringList() << "ID Empresa"
                                       << "Empresa"
                                       << "ID Filial"
                                       << "Filial"
                                       << "CNPJ"
                                       << "Cidade Região"
                                       << "Cálculo"
                                       << "Competência"
                                       << "Setor"
                                       << "Código do Evento"
                                       << "Descrição do Evento"
                                       << "Tipo Evento"
                                       << "Valor";
    ui->tableWidget->setColumnCount(labels.count());
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    ui->tableWidget->resizeColumnsToContents();
}

GuiaINSSFolha::~GuiaINSSFolha()
{
    delete ui;
}

void GuiaINSSFolha::filtroItemTabela(QString filter)
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

void GuiaINSSFolha::focusPeriodoInicial()
{
    ui->periodoFinal->setFocus();
}

void GuiaINSSFolha::focusPeriodoFinal()
{
    ui->campoID_Empresa->setFocus();
}

void GuiaINSSFolha::pesquisarEmpresa()
{
    Pesquisar * p = new Pesquisar(this, getMapEmpresas(), getMapFiliais(), "", 1);
    p->setWindowModality(Qt::ApplicationModal);
    p->setWindowFlags(Qt::Window);
    p->setWindowTitle("Selecionar Empresa");
    connect(p, SIGNAL(getEmpresa(QString)), this, SLOT(setEmpresa(QString)));
    p->show();
}

void GuiaINSSFolha::setEmpresa(QString e)
{
    ui->campoID_Empresa->setText(e);
    retornaCadastroEmpresa();
}

void GuiaINSSFolha::pesquisarFilial()
{
    Pesquisar * p = new Pesquisar(this, getMapEmpresas(), getMapFiliais(), ui->campoID_Empresa->text(), 2);
    p->setWindowModality(Qt::ApplicationModal);
    p->setWindowFlags(Qt::Window);
    p->setWindowTitle("Selecionar Filial");
    connect(p, SIGNAL(getFilial(QString)), this, SLOT(setFilial(QString)));
    p->show();
}

void GuiaINSSFolha::setFilial(QString f)
{
    ui->campoID_Filial->setText(f);
    retornaCadastroFilial(f);
}

void GuiaINSSFolha::retornaCadastroEmpresa()
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

void GuiaINSSFolha::retornaCadastroFilial()
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
            ui->botaoProcessar->setFocus();
        }
    }
}

void GuiaINSSFolha::retornaCadastroFilial(QString _ID_Filial)
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
            ui->botaoProcessar->setFocus();
        }
    }
}

void GuiaINSSFolha::getDatatable()
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
    QDate _tempDateIni = ui->periodoInicial->date();
    QDate _tempDateFim = ui->periodoFinal->date();
    int _anoComIni = _tempDateIni.year();
    int _mesComIni = _tempDateIni.month();
    int _diaComIni = 1;
    QDate __dataIni( _anoComIni, _mesComIni, _diaComIni );

    int _anoComFim = _tempDateFim.year();
    int _mesComFim = _tempDateFim.month();
    int _diaComFim = 1;
    QDate __dataFim( _anoComFim, _mesComFim, _diaComFim );

    QMap<int, Eventos*> __tempMap = controle->getGuiaINSS(
                ui->campoID_Empresa->text(),
                ui->campoID_Filial->text(),
                __dataIni.toString(Qt::ISODate),
                __dataFim.toString(Qt::ISODate));
    progresso.setLabelText("Processando dados...");

    if(__tempMap.isEmpty()) {
        QMessageBox::information(this, tr("Eventos GUIA INSS"), QString("Nenhuma informação encontrada!"), QMessageBox::Ok);
        return;
    }

    QMapIterator<int, Eventos*> __mapIterator(__tempMap);
    progresso.setMaximum(__tempMap.count());

    ui->tableWidget->setRowCount(__tempMap.count());
    int linha = 0;
    while (__mapIterator.hasNext()) {
        __mapIterator.next();
        progresso.setValue(__mapIterator.key());
        Eventos *evento = __mapIterator.value();
        inserirLinhaTabela(linha, ui->tableWidget->columnCount(), evento);
        linha++;
    }
    ui->tableWidget->resizeColumnsToContents();
}

void GuiaINSSFolha::inserirItemTabela(int r, int c, QString sValue)
{
    ui->tableWidget->setItem(r,c,new QTableWidgetItem(sValue));
}

void GuiaINSSFolha::inserirItemTabela(int r, int c, QDate dtValue)
{
    ui->tableWidget->setItem(r,c,new QTableWidgetItem(QDate(dtValue).toString("dd/MM/yyyy")));
}

void GuiaINSSFolha::inserirItemTabela(int r, int c, double dValue)
{
    QTableWidgetItem *item = new QTableWidgetItem(QString("%L1").arg(dValue, 12, 'f', 4));
    item->setTextAlignment(Qt::AlignRight);
    ui->tableWidget->setItem(r,c,item);
}

void GuiaINSSFolha::inserirItemTabela(int r, int c, int iValue)
{
    QTableWidgetItem *item = new QTableWidgetItem(QString::number(iValue));
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(r,c,item);
}

void GuiaINSSFolha::inserirLinhaTabela(int linha, int nrColunas, Eventos *evento)
{
    for (int coluna = 0; coluna < nrColunas; ++coluna) {
        if(coluna == 0)
            inserirItemTabela(linha, coluna, evento->getID_Empresa());
        if(coluna == 1)
            inserirItemTabela(linha, coluna, evento->getEmpresa());
        if(coluna == 2)
            inserirItemTabela(linha, coluna, evento->getID_Filial());
        if(coluna == 3)
            inserirItemTabela(linha, coluna, evento->getFilial());
        if(coluna == 4)
            inserirItemTabela(linha, coluna, evento->getCnpj());
        if(coluna == 5)
            inserirItemTabela(linha, coluna, evento->getCidadeRegiao());
        if(coluna == 6)
            inserirItemTabela(linha, coluna, evento->getCalculo());
        if(coluna == 7)
            inserirItemTabela(linha, coluna, evento->getCompetencia().toString("dd/MM/yyyy"));
        if(coluna == 8)
            inserirItemTabela(linha, coluna, evento->getSetor());
        if(coluna == 9)
            inserirItemTabela(linha, coluna, evento->getCodigoEvento());
        if(coluna == 10)
            inserirItemTabela(linha, coluna, evento->getEvento());
        if(coluna == 11)
            inserirItemTabela(linha, coluna, evento->getTipoEvento());
        if(coluna == 12)
            inserirItemTabela(linha, coluna, evento->getValorEvento());
    }
}

void GuiaINSSFolha::exportarParaExcel()
{
        QString nomeArquivoTitulo = "";
        if(ui->campoDescricaoEmpresa->text().isEmpty() || ui->campoDescricaoFilial->text().isEmpty())
            nomeArquivoTitulo = "FullData";
        else
            nomeArquivoTitulo = ui->campoDescricaoFilial->text().replace(' ','_');

        QString __nomeArquivo = "/"+nomeArquivoTitulo+"_"+ui->periodoInicial->text().replace('/','-')+"_"+ui->periodoFinal->text().replace('/','-');
        ExportarArquivo *exp = new ExportarArquivo(this, ui->tableWidget);
        connect(exp, SIGNAL(mensagemRetorno(QString)), this, SLOT(caixaMensagemUsuario(QString)));
        exp->exportar(__nomeArquivo);
}

void GuiaINSSFolha::caixaMensagemUsuario(QString msg)
{
    QMessageBox::information(this, tr("Exportação de Dados"), QString(msg), QMessageBox::Ok);
}

QMap<int, CadastroFilial *> GuiaINSSFolha::getMapFiliais() const
{
    return mapFiliais;
}

void GuiaINSSFolha::setMapFiliais(const QMap<int, CadastroFilial *> &value)
{
    mapFiliais = value;
}

QMap<int, CadastroEmpresa *> GuiaINSSFolha::getMapEmpresas() const
{
    return mapEmpresas;
}

void GuiaINSSFolha::setMapEmpresas(const QMap<int, CadastroEmpresa *> &value)
{
    mapEmpresas = value;
}

