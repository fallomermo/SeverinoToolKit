#include "planodecontas.h"
#include "ui_planodecontas.h"

PlanoDeContas::PlanoDeContas(QWidget *parent) : QWidget(parent), ui(new Ui::PlanoDeContas)
{
    ui->setupUi(this);
    controle = new ControleDAO(this);
    connect(controle, SIGNAL(mensagemRetorno(QString)), this, SLOT(mensagemInfoUsuario(QString)));
    this->update();

    ui->campoIDEmpresa->setPlaceholderText(QString("ID Empresa"));
    ui->campoIDFilial->setPlaceholderText(QString("ID Filial"));
    atualizarTabela();
}

PlanoDeContas::PlanoDeContas(QWidget *parent, QMap<int, CadastroEmpresa *> ce, QMap<int, CadastroFilial *> cf) :
    QWidget(parent),
    ui(new Ui::PlanoDeContas),
    mapEmpresas(ce),
    mapFiliais(cf)
{
    ui->setupUi(this);
    controle = new ControleDAO(this);
    connect(controle, SIGNAL(mensagemRetorno(QString)), this, SLOT(mensagemInfoUsuario(QString)));
    ui->campoInicioPeriodo->setDate(QDateTime::currentDateTime().date());
    ui->campoFinalPeriodo->setDate(QDateTime::currentDateTime().date());
    atualizarTabela();
    connect(ui->campoIDEmpresa,SIGNAL(returnPressed()), this, SLOT(empresaSelecionada()));
    connect(ui->campoIDFilial,SIGNAL(returnPressed()), this, SLOT(filialSelecionada()));
    connect(ui->campoIDFilial,SIGNAL(returnPressed()), this, SLOT(focusInicioPeriodo()));
    connect(ui->campoInicioPeriodo, SIGNAL(editingFinished()), this, SLOT(focusFinalPeriodo()));
    connect(ui->campoFinalPeriodo, SIGNAL(editingFinished()), this, SLOT(focusProcessar()));
    connect(ui->botaoProcessar,SIGNAL(clicked(bool)), this, SLOT(getDatatable()));
    connect(ui->botaoExportar,SIGNAL(clicked(bool)), this, SLOT(exportarParaExcel()));
    connect(ui->tableWidget,SIGNAL(clicked(QModelIndex)), this, SLOT(atualizarResultados(QModelIndex)));
    connect(ui->tableWidget,SIGNAL(activated(QModelIndex)), this, SLOT(atualizarResultados(QModelIndex)));
    connect(this, SIGNAL(statusProgresso(bool)), this, SLOT(progressoRequisicao(bool)));

    ui->campoIDEmpresa->setPlaceholderText(QString("ID Empresa"));
    QAction *_act_emp = ui->campoIDEmpresa->addAction(QIcon(":/images/search.png"), QLineEdit::TrailingPosition);
    connect(_act_emp, SIGNAL(triggered(bool)), this, SLOT(pesquisarEmpresa()));

    ui->campoIDFilial->setPlaceholderText(QString("ID Filial"));
    QAction *_act_fil = ui->campoIDFilial->addAction(QIcon(":/images/search.png"), QLineEdit::TrailingPosition);
    connect(_act_fil, SIGNAL(triggered(bool)), this, SLOT(pesquisarFilial()));

    ui->campoPesquisarObjetosTabela->addAction(QIcon(":/images/search.png"), QLineEdit::TrailingPosition);
    ui->campoPesquisarObjetosTabela->setPlaceholderText(QString("Pesquisar Itens na Tabela..."));
    connect(ui->campoPesquisarObjetosTabela, SIGNAL(textChanged(QString)), this, SLOT(filtroItemTabela(QString)));

}

PlanoDeContas::~PlanoDeContas()
{
    delete ui;
}

void PlanoDeContas::atualizarTabela()
{
    ui->campoIDEmpresa->setToolTip("Código da Empresa.");
    ui->campoIDFilial->setToolTip("Código da Filial.");
//    ui->botaoPesquisaEmpresa->setToolTip("Lista de Empresas Disponíveis.");
//    ui->botaoPesquisaFilial->setToolTip("Lista de Filiais Disponíveis.");
    ui->campoInicioPeriodo->setToolTip("Início da Competência de Cálculo.");
    ui->campoFinalPeriodo->setToolTip("Fim da Competência de Cálculo.");
    ui->botaoProcessar->setToolTip("Clique para processar a requisição das Informações.");
    ui->botaoExportar->setToolTip("Exportar para arquivo .CSV!");
    ui->tableWidget->setToolTip("Tabela dos Eventos de Rateio para Plano e Contas.");
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

void PlanoDeContas::empresaSelecionada()
{
    QMapIterator<int, CadastroEmpresa*> mi(getMapEmpresas());
    QString _nomeEmpresa;

    while (mi.hasNext()) {
        mi.next();
        CadastroEmpresa *_cemp = new CadastroEmpresa(this);
        _cemp = mi.value();

        if(_cemp->getID_Empresa() == ui->campoIDEmpresa->text()) {
            _nomeEmpresa = _cemp->getEmpresa();
        }
    }

    if(_nomeEmpresa.isEmpty()) {
        QMessageBox::critical(this, tr("Cadastro Empresa [!]"), QString("Empresa não encontrada [!]"), QMessageBox::Ok);
        ui->campoIDEmpresa->setFocus();
    } else {
        ui->campoNomeEmpresa->setText(_nomeEmpresa);
        ui->campoIDFilial->setFocus();
    }
}

void PlanoDeContas::filialSelecionada()
{
    if(ui->campoNomeEmpresa->text().isEmpty()) {
        QMessageBox::critical(this, tr("Seleção de Filtro"), QString("Nenhuma Empresa Selecionada"), QMessageBox::Ok);
        ui->campoIDEmpresa->setFocus();
    } else {
        QMapIterator<int, CadastroFilial*> mi(getMapFiliais());
        QString _nomeFilial;

        while (mi.hasNext()) {
            mi.next();
            CadastroFilial *_cfil = new CadastroFilial(this);
            _cfil = mi.value();

            if(_cfil->getID_Empresa() == ui->campoIDEmpresa->text()) {
                if(_cfil->getID_Filial() == ui->campoIDFilial->text()) {
                    _nomeFilial = _cfil->getFilial();
                }
            }
        }

        if(_nomeFilial.isEmpty()) {
            QMessageBox::critical(this, tr("Cadastro Filial [!]"), QString("Filial não encontrada [!]"), QMessageBox::Ok);
            ui->campoIDFilial->setFocus();
        } else {
            ui->campoNomeFilial->setText(_nomeFilial);
            ui->botaoProcessar->setFocus();
        }
    }
}

void PlanoDeContas::filialSelecionada(QString _ID_Filial)
{
    if(ui->campoNomeEmpresa->text().isEmpty()) {
        QMessageBox::critical(this, tr("Seleção de Filtro"), QString("Nenhuma Empresa Selecionada"), QMessageBox::Ok);
        ui->campoIDEmpresa->setFocus();
    } else {
        QMapIterator<int, CadastroFilial*> mi(getMapFiliais());
        QString _nomeFilial;

        while (mi.hasNext()) {
            mi.next();
            CadastroFilial *_cfil = new CadastroFilial(this);
            _cfil = mi.value();
            QString _ID_Empresa = ui->campoIDEmpresa->text();
            if(_cfil->getID_Empresa() == _ID_Empresa && _cfil->getID_Filial() == _ID_Filial) {
                _nomeFilial = _cfil->getFilial();
            }
        }

        if(_nomeFilial.isEmpty()) {
            QMessageBox::critical(this, tr("Cadastro Filial [!]"), QString("Filial Inválida [!]"), QMessageBox::Ok);
            ui->campoIDFilial->setFocus();
        } else {
            ui->campoIDFilial->setText(_ID_Filial);
            ui->campoNomeFilial->setText(_nomeFilial);
            ui->campoInicioPeriodo->setFocus();
        }
    }
}

void PlanoDeContas::competenciaSelecionada()
{
    ui->campoInicioPeriodo->setFocus();
}

void PlanoDeContas::focusInicioPeriodo()
{
    ui->campoInicioPeriodo->setFocus();
}

void PlanoDeContas::focusFinalPeriodo()
{
    ui->campoFinalPeriodo->setFocus();
}

void PlanoDeContas::focusProcessar()
{
    ui->botaoProcessar->setFocus();
}

void PlanoDeContas::progressoRequisicao(bool)
{
    QProgressDialog _progresso(QString("Processando requisição..."), QString("Cancelar"), 0, 0);
    _progresso.setWindowModality(Qt::WindowModal);;
    _progresso.setMinimumDuration(0);
    _progresso.setRange(0, 1);
    _progresso.setValue(0);
    _progresso.setRange(0, 0);
    _progresso.show();
}

QString PlanoDeContas::pesquisarItem()
{
    return QString("");
}

void PlanoDeContas::pesquisarEmpresa()
{
    Pesquisar * p = new Pesquisar(this, getMapEmpresas(), getMapFiliais(), "", 1);
    p->setWindowModality(Qt::ApplicationModal);
    p->setWindowFlags(Qt::Window);
    p->setWindowTitle("Selecionar Empresa");
    connect(p, SIGNAL(getEmpresa(QString)), this, SLOT(setEmpresa(QString)));
    p->show();
}

void PlanoDeContas::pesquisarFilial()
{
    Pesquisar * p = new Pesquisar(this, getMapEmpresas(), getMapFiliais(), ui->campoIDEmpresa->text(), 2);
    p->setWindowModality(Qt::ApplicationModal);
    p->setWindowFlags(Qt::Window);
    p->setWindowTitle("Selecionar Filial");
    connect(p, SIGNAL(getFilial(QString)), this, SLOT(setFilial(QString)));
    p->show();
}

void PlanoDeContas::setEmpresa(QString e)
{
    ui->campoIDEmpresa->setText(e);
    empresaSelecionada();
}

void PlanoDeContas::setFilial(QString f)
{
    filialSelecionada(f);
}

void PlanoDeContas::getDatatable()
{
    QProgressDialog _progresso("Processando... \nAguardade!", "Cancelar", 70, 200, this);
    _progresso.setWindowModality(Qt::WindowModal);
    _progresso.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    _progresso.activateWindow();
    _progresso.setRange(0, 0);
    _progresso.setValue(0);
    _progresso.setAutoClose(true);
    _progresso.setVisible(true);
    _progresso.show();
    qApp->processEvents(QEventLoop::DialogExec);

    ui->tableWidget->clear();
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

    controle = new ControleDAO(this);
    QDate _tempDateIni = ui->campoInicioPeriodo->date();
    QDate _tempDateFim = ui->campoFinalPeriodo->date();
    int _anoComIni = _tempDateIni.year();
    int _mesComIni = _tempDateIni.month();
    int _diaComIni = 1;
    QDate __dataIni( _anoComIni, _mesComIni, _diaComIni );

    int _anoComFim = _tempDateFim.year();
    int _mesComFim = _tempDateFim.month();
    int _diaComFim = 1;
    QDate __dataFim( _anoComFim, _mesComFim, _diaComFim );

    _progresso.setLabelText(QString("Coletando Eventos de Plano de Contas..."));
    //QMap<int, Eventos*> pc = controle->getPlanoContas(ui->campoIDEmpresa->text(), ui->campoIDFilial->text(), __dataIni.toString(Qt::ISODate), __dataFim.toString(Qt::ISODate));
    QMap<int, Eventos*> pc = controle->getPlanoContas("", "", "1900-12-31", "1900-12-31");
    _progresso.setLabelText(QString("Coletando Eventos de Guia INSS..."));
    QMap<int, Eventos*> gp = controle->getGuiaINSS(ui->campoIDEmpresa->text(), ui->campoIDFilial->text(), __dataIni.toString(Qt::ISODate), __dataFim.toString(Qt::ISODate));

    if(pc.isEmpty() && gp.isEmpty()) {
        _progresso.setValue(100);
        _progresso.setLabelText(QString("Ops!..."));
        _progresso.close();
        QMessageBox::information(this, tr("Eventos Plano de Contas"), QString("Nenhuma informação Encontrada [!]"), QMessageBox::Ok);
        ui->campoInicioPeriodo->setFocus();
        return;
    }

    _progresso.setMaximum(pc.count() + gp.count());
    _progresso.setLabelText(QString("Organizando dados..."));

    ui->tableWidget->setRowCount(pc.count());
    QMapIterator<int, Eventos*> map1(pc);
    int linha = 0;
    _progresso.setValue(0);
    _progresso.setLabelText(QString("Atualizando Lista de Eventos Plano de Contas..."));

    double _valTotReg = 0.0;
    while (map1.hasNext()) {
        map1.next();
        _progresso.setValue(map1.key());
        Eventos *evento = new Eventos;
        evento = map1.value();
        inserirLinhaTabela(linha,ui->tableWidget->columnCount(),evento);
        _valTotReg += evento->getValorEvento();
        linha++;
    }

    QMapIterator<int, Eventos*> map2(gp);
    _progresso.setLabelText(QString("Atualizando Lista de Eventos GUIA INSS Ratiada..."));

    while (map2.hasNext()) {
        map2.next();
        _progresso.setValue(map2.key());
        ui->tableWidget->insertRow(linha);
        Eventos *evento = new Eventos;
        evento = map2.value();
        inserirLinhaTabela(linha,ui->tableWidget->columnCount(),evento);
        _valTotReg += evento->getValorEvento();
        linha++;
    }

    _progresso.setValue(pc.count() + gp.count());
    _progresso.setLabelText("Concluido!");
    ui->tableWidget->resizeColumnsToContents();

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->sortByColumn(7);
    ui->campoTotalRegistros->setText(QString::number(pc.count() + gp.count()));
    ui->campoValorTotalAgrupado->setText(QString("%L1").arg(_valTotReg,12, 'f',2,' '));
    ui->botaoExportar->setFocus();
}

void PlanoDeContas::inserirItemTabela(int r, int c, QString sValue)
{
    ui->tableWidget->setItem(r,c,new QTableWidgetItem(sValue));
}

void PlanoDeContas::inserirItemTabela(int r, int c, QDate v)
{
    ui->tableWidget->setItem(r,c,new QTableWidgetItem(QDate(v).toString("dd/MM/yyyy")));
}

void PlanoDeContas::inserirItemTabela(int r, int c, double dValue)
{
    QTableWidgetItem *item = new QTableWidgetItem(QString("%L1").arg(dValue, 12, 'f', 4));
    item->setTextAlignment(Qt::AlignRight);
    ui->tableWidget->setItem(r,c,item);
}

void PlanoDeContas::inserirItemTabela(int r, int c, int iValue)
{
    QTableWidgetItem *item = new QTableWidgetItem(QString::number(iValue));
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(r,c,item);
}

void PlanoDeContas::inserirLinhaTabela(int linha, int nrColunas, Eventos *evento)
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

void PlanoDeContas::exportarParaExcel()
{
    if(ui->tableWidget->rowCount() <= 0) {
        QMessageBox::critical(this, tr("Exportar Dados da Tabela"), QString("Não existe informação para exportar!!!"), QMessageBox::Ok);
        ui->campoIDEmpresa->setFocus();
    } else {
        QProgressDialog progresso("Salvando dados para CSV...\n", "Cancelar", 70, 200, this);
        progresso.setWindowModality(Qt::WindowModal);
        progresso.setWindowFlag(Qt::Window);
        progresso.setWindowFlag(Qt::FramelessWindowHint);
        progresso.setMinimumDuration(0);
        progresso.activateWindow();
        progresso.setRange(0, 1);
        progresso.setValue(0);
        progresso.setRange(0, 0);
        progresso.setVisible(true);
        progresso.show();
        QCoreApplication::processEvents();
        progresso.setRange(0, 1);
        progresso.setValue(0);
        progresso.setRange(0, 0);

        QString nomeArquivoTitulo = "";
        if(ui->campoNomeEmpresa->text().isEmpty() || ui->campoNomeFilial->text().isEmpty())
            nomeArquivoTitulo = "FullData";
        else
            nomeArquivoTitulo = ui->campoNomeFilial->text().replace(' ','_');

        QString _nomeArquivo = "/"+nomeArquivoTitulo+"_"+ui->campoInicioPeriodo->text().replace('/','-')+"_"+ui->campoFinalPeriodo->text().replace('/','-');

        QString filename = QFileDialog::getSaveFileName(this, tr("Exportação CSV"), QDir::homePath()+QString(_nomeArquivo), "CSV (*.csv)");
        if(filename.isEmpty())
            return;

        QFile f( filename );
        if (f.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream data( &f );
            QStringList strList;
            QTableWidget *nTable = ui->tableWidget;
            progresso.setMaximum(ui->tableWidget->rowCount());
            for( int r = 0; r < nTable->rowCount(); ++r )
            {
                progresso.setValue(r);
                strList.clear();
                for( int c = 0; c < nTable->columnCount(); ++c )
                {
                    strList << "\""+nTable->item( r, c )->text().trimmed()+"\"";
                }
                data << strList.join( ";" )+"\n";
            }
            f.close();
            QMessageBox::information(this, tr("Exportação para Arquivo CSV"), QString("Arquivo salvo com Sucesso!"), QMessageBox::Ok);
        }
    }
}

void PlanoDeContas::atualizarResultados(QModelIndex i)
{
    QLocale local = this->locale();

    if(i.column() > 10)
        return;

    QTableWidgetItem *_item = ui->tableWidget->item(i.row(), i.column());
    ui->campoEventoPlanoContas->setText(_item->text());

    double dValue = 0.0;
    QString sValue = _item->text();
    for (int linha = 0; linha < ui->tableWidget->rowCount(); ++linha) {
        for (int coluna = 0; coluna <= i.column(); ++coluna) {
            if(sValue.contains(ui->tableWidget->item(linha, coluna)->text())) {
                dValue+= local.toDouble(ui->tableWidget->item(linha, 12)->text());
            }
        }
    }
    ui->campoValorEventoAgrupado->setText(QString("%L1").arg(dValue,12,'f',4));
}

void PlanoDeContas::mensagemInfoUsuario(QString msg)
{
    QMessageBox::critical(this, tr("Erro Conexão com Banco de Dados"), QString("Não foi possível consultar a base de dado!\n").append(msg), QMessageBox::Ok);
}

void PlanoDeContas::filtroItemTabela(QString filtro)
{
    for( int i = 0; i < ui->tableWidget->rowCount(); ++i ) {
        bool match = false;
        for( int j = 0; j < ui->tableWidget->columnCount(); ++j ) {
            QTableWidgetItem *item = ui->tableWidget->item( i, j );
            if( item->text().contains(filtro) ) {
                match = true;
                break;
            }
        }
        ui->tableWidget->setRowHidden( i, !match );
    }
}

QMap<int, CadastroFilial *> PlanoDeContas::getMapFiliais() const
{
    return mapFiliais;
}

QMap<int, CadastroEmpresa *> PlanoDeContas::getMapEmpresas() const
{
    return mapEmpresas;
}
