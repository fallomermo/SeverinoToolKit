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
    QStringList labels = QStringList() << "ID Empresa"
                                       << "Empresa"
                                       << "ID Filial"
                                       << "Filial"
                                       << "CNPJ"
                                       << "Cidade Região"
                                       << "Cálculo"
                                       << "Competência"
                                       << "Tipo de Cálculo"
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
    if(!ui->campoIDEmpresa->text().isEmpty()) {
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
    } else {
        ui->campoIDFilial->setFocus();
    }
}

void PlanoDeContas::filialSelecionada()
{
    if(ui->campoNomeEmpresa->text().isEmpty() && !ui->campoIDFilial->text().isEmpty()) {
        QMessageBox::critical(this, tr("Seleção de Filtro"), QString("Nenhuma Empresa Selecionada"),QMessageBox::Ok);
        ui->campoIDEmpresa->setFocus();
    } else {
        if(!ui->campoIDFilial->text().isEmpty()) {
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
                QMessageBox::critical(
                            this,
                            tr("Cadastro Filial [!]"),
                            QString("Filial não encontrada [!]"),
                            QMessageBox::Ok);
                ui->campoIDFilial->setFocus();
            } else {
                ui->campoNomeFilial->setText(_nomeFilial);
                ui->campoFolhaNormal->setFocus();
            }
        } else {
            ui->campoFolhaNormal->setFocus();
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

void PlanoDeContas::caixaMensagemUsuario(QString msg)
{
    QMessageBox::information(this, tr("Exportação de Dados"), QString(msg), QMessageBox::Ok);
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
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    CaixaMensagemProgresso *cxMensagem = new CaixaMensagemProgresso(this);
    cxMensagem->setWindowFlag(Qt::Window);
    cxMensagem->setWindowFlag(Qt::FramelessWindowHint);
    cxMensagem->setWindowModality(Qt::ApplicationModal);

    connect(this, SIGNAL(fecharCaixaMensagem()), cxMensagem, SLOT(fecharJanela()));
    connect(this, SIGNAL(minimumProgressValue(int)), cxMensagem, SLOT(setMinimumValue(int)));
    connect(this, SIGNAL(maximumProgressValue(int)), cxMensagem, SLOT(setMaximumValue(int)));

    QMovie *movie = new QMovie(":/images/splash.gif");
    cxMensagem->setMovie(movie);
    cxMensagem->show();
    qApp->processEvents();

    ui->tableWidget->clear();
    QStringList labels = QStringList() << "Código da Empresa"
                                       << "Empresa"
                                       << "Código da Filial"
                                       << "Filial"
                                       << "CNPJ"
                                       << "Cidade Região"
                                       << "Cálculo"
                                       << "Competência"
                                       << "Tipo de Cálculo"
                                       << "Setor"
                                       << "Código do Evento"
                                       << "Descrição do Evento"
                                       << "Tipo do Evento"
                                       << "Valor";
    ui->tableWidget->setColumnCount(labels.count());
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    ui->tableWidget->resizeColumnsToContents();

    int __tipoCalculo = 0;
    if(ui->campoFolhaNormal->isChecked()) __tipoCalculo = 11; else __tipoCalculo = 14;

    QThread *threadDAO = new QThread(nullptr);
    controle = new ControleDAO(nullptr);
    controle->moveToThread(threadDAO);

    qRegisterMetaType<QMap<int,Eventos*>>("__tempMapEventos");
    connect(this, SIGNAL(retornaPlanoDeContas(QString,QString,QString,QString,int)), controle, SLOT(retornaPlanoContas(QString,QString,QString,QString,int)));
    connect(threadDAO, SIGNAL(finished()), controle, SLOT(deleteLater()));
    connect(this, SIGNAL(finishThread()), threadDAO, SLOT(terminate()));
    connect(controle, SIGNAL(enviarPlanoContas(QMap<int,Eventos*>)), this, SLOT(preencherTabela(QMap<int,Eventos*>)));

    threadDAO->start(QThread::NormalPriority);

    QDate _tempDateIni = ui->campoInicioPeriodo->date();
    QDate _tempDateFim = ui->campoFinalPeriodo->date();
    QDate __dataIni( _tempDateIni.year(), _tempDateIni.month(), 1);
    QDate __dataFim( _tempDateFim.year(), _tempDateFim.month(), _tempDateFim.daysInMonth());
    QString __idEmp = ui->campoIDEmpresa->text().trimmed();
    QString __idFil = ui->campoIDFilial->text().trimmed();
    QString __datIni = __dataIni.toString(Qt::ISODate);
    QString __datFim = __dataFim.toString(Qt::ISODate);
    emit retornaPlanoDeContas(__idEmp, __idFil, __datIni, __datFim, __tipoCalculo);
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
    QTableWidgetItem *item = new QTableWidgetItem(QString("%L1").arg(dValue, 0, 'f', 4));
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
            inserirItemTabela(linha, coluna, evento->getTipoDeCalculo());
        if(coluna == 9)
            inserirItemTabela(linha, coluna, evento->getSetor());
        if(coluna == 10)
            inserirItemTabela(linha, coluna, evento->getCodigoEvento());
        if(coluna == 11)
            inserirItemTabela(linha, coluna, evento->getEvento());
        if(coluna == 12)
            inserirItemTabela(linha, coluna, evento->getTipoEvento());
        if(coluna == 13)
            inserirItemTabela(linha, coluna, evento->getValorEvento());
    }
}

void PlanoDeContas::exportarParaExcel()
{
    QString nomeArquivoTitulo = "";
    if(ui->campoNomeEmpresa->text().isEmpty() || ui->campoNomeFilial->text().isEmpty())
        nomeArquivoTitulo = "FullData";
    else
        nomeArquivoTitulo = ui->campoNomeFilial->text().replace(' ','_');

    QString __nomeArquivo = "/"+nomeArquivoTitulo
            +"_"+ui->campoInicioPeriodo->text().replace('/','-')
            +"_"+ui->campoFinalPeriodo->text().replace('/','-');
    ExportarArquivo *exp = new ExportarArquivo(this, ui->tableWidget);
    connect(exp, SIGNAL(mensagemRetorno(QString)), this, SLOT(caixaMensagemUsuario(QString)));
    exp->exportar(__nomeArquivo,0);
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
    ui->campoValorEventoAgrupado->setText(QString("%L1").arg(dValue, 0, 'f', 4, Qt::AlignRight));
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

void PlanoDeContas::preencherTabela(QMap<int, Eventos *> __tempMap)
{
    emit finishThread();
    if(__tempMap.isEmpty()) {
        emit progressValue(0);
        emit fecharCaixaMensagem();
        QMessageBox::information(this, tr("Eventos Plano de Contas"), QString("Nenhuma informação Encontrada [!]"), QMessageBox::Ok);
        ui->campoInicioPeriodo->setFocus();
        emit fecharCaixaMensagem();
        return;
    }

    emit minimumProgressValue(0);
    emit maximumProgressValue(__tempMap.count());

    ui->tableWidget->setRowCount(__tempMap.count());
    QMapIterator<int, Eventos*> __tempItarator(__tempMap);
    int linha = 0;

    double _valTotReg = 0.0;
    while (__tempItarator.hasNext()) {
        __tempItarator.next();
        emit progressValue(__tempItarator.key());
        Eventos *evento = new Eventos;
        evento = __tempItarator.value();
        inserirLinhaTabela(linha, ui->tableWidget->columnCount(), evento);
        _valTotReg += evento->getValorEvento();
        linha++;
    }

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->sortByColumn(7);
    ui->campoValorEventoAgrupado->setText(QString("%L1").arg(0, 0, 'f', 4, Qt::AlignRight));
    ui->campoTotalRegistros->setText(QString("%L1").arg(__tempMap.count(), 0, 'f', 4, Qt::AlignRight));
    ui->campoValorTotalAgrupado->setText(QString("%L1").arg(_valTotReg, 0, 'f', 4, Qt::AlignRight));
    emit fecharCaixaMensagem();
    ui->botaoExportar->setFocus();
}

QMap<int, CadastroFilial *> PlanoDeContas::getMapFiliais() const
{
    return mapFiliais;
}

QMap<int, CadastroEmpresa *> PlanoDeContas::getMapEmpresas() const
{
    return mapEmpresas;
}
