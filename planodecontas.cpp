#include "planodecontas.h"
#include "ui_planodecontas.h"

PlanoDeContas::PlanoDeContas(QWidget *parent) : QWidget(parent), ui(new Ui::PlanoDeContas)
{
    ui->setupUi(this);
    controle = new ControleDAO(this);
    connect(controle, SIGNAL(mensagemRetorno(QString)), this, SLOT(mensagemInfoUsuario(QString)));
    this->update();
    this->atualizarTabela();
}

PlanoDeContas::PlanoDeContas(QWidget *parent, QMap<int, CadastroEmpresa *> ce, QMap<int, CadastroFilial *> cf) :
    QWidget(parent),
    ui(new Ui::PlanoDeContas),
    mapEmpresas(ce),
    mapFiliais(cf)
{
    ui->setupUi(this);
    controle = new ControleDAO(this);
    connect(controle, SIGNAL(mensagemRetorno(QString,QString)), this, SLOT(mensagemInfoUsuario(QString,QString)));
    ui->campoInicioPeriodo->setDate(QDateTime::currentDateTime().date());
    ui->campoFinalPeriodo->setDate(QDateTime::currentDateTime().date());
    this->atualizarTabela();
    connect(ui->campoInicioPeriodo, SIGNAL(editingFinished()), this, SLOT(focusFinalPeriodo()));
    connect(ui->campoFinalPeriodo, SIGNAL(editingFinished()), this, SLOT(focusProcessar()));
    connect(ui->botaoProcessar,SIGNAL(clicked(bool)), this, SLOT(getDatatable()));
    connect(ui->botaoExportar,SIGNAL(clicked(bool)), this, SLOT(exportarParaExcel()));
    connect(ui->tableWidget,SIGNAL(clicked(QModelIndex)), this, SLOT(atualizarResultados(QModelIndex)));
    connect(this, SIGNAL(statusProgresso(bool)), this, SLOT(progressoRequisicao(bool)));

    ui->campoIDEmpresa->setPlaceholderText(QString("ID Empresa"));
    QAction *_act_emp = ui->campoIDEmpresa->addAction(QIcon(":/images/search.png"), QLineEdit::TrailingPosition);
    connect(_act_emp, SIGNAL(triggered(bool)), this, SLOT(pesquisarEmpresa()));

    ui->campoIDFilial->setPlaceholderText(QString("ID Filial"));
    QAction *_act_fil = ui->campoIDFilial->addAction(QIcon(":/images/search.png"), QLineEdit::TrailingPosition);
    connect(_act_fil, SIGNAL(triggered(bool)), this, SLOT(pesquisarFilial()));

    ui->campoPesquisarObjetosTabela->addAction(QIcon(":/images/search.png"), QLineEdit::TrailingPosition);
    ui->campoPesquisarObjetosTabela->setPlaceholderText(QString("Pesquisar..."));
    connect(ui->campoPesquisarObjetosTabela, SIGNAL(textChanged(QString)), this, SLOT(filtroItemTabela(QString)));

}

PlanoDeContas::~PlanoDeContas()
{
    delete ui;
}

void PlanoDeContas::atualizarTabela()
{
    QStringList labels = QStringList() << "ID Empresa" << "Empresa" << "ID Filial" << "Filial" << "CNPJ" << "Cidade Região" << "Cálculo" << "Competência" << "Tipo de Cálculo" << "Setor" << "Código do Evento" << "Descrição do Evento" << "Tipo Evento"<< "Valor";
    ui->tableWidget->setColumnCount(labels.count());
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    ui->tableWidget->resizeColumnsToContents();
}

void PlanoDeContas::empresaSelecionada()
{
    if(!this->getCodigoEmpresa().isEmpty()) {
        QMapIterator<int, CadastroEmpresa*> mi(getMapEmpresas());
        QString _nomeEmpresa;

        while (mi.hasNext()) {
            mi.next();
            CadastroEmpresa *_cemp = new CadastroEmpresa(this);
            _cemp = mi.value();

            if(_cemp->getID_Empresa() == this->getCodigoEmpresa()) {
                _nomeEmpresa = QString("%0 - %1").arg(_cemp->getID_Empresa()).arg(_cemp->getEmpresa());
            }
        }

        if(_nomeEmpresa.isEmpty()) {
            QMessageBox::critical(this, tr("Cadastro Empresa [!]"), QString("Empresa não encontrada [!]"), QMessageBox::Ok);
            ui->campoIDEmpresa->setFocus();
        } else {
            ui->campoIDEmpresa->setText(_nomeEmpresa);
            ui->campoIDFilial->setFocus();
        }
    } else {
        ui->campoIDFilial->setFocus();
    }
}

void PlanoDeContas::filialSelecionada()
{
    if(this->getCodigoEmpresa().isEmpty()) {
        QMessageBox::critical(this, tr("Seleção de Filtro"), QString("Nenhuma Empresa Selecionada"), QMessageBox::Ok);
        ui->campoIDEmpresa->setFocus();
    } else {
        if(!this->getCodigoFilial().isEmpty()) {
            QMapIterator<int, CadastroFilial*> mi(getMapFiliais());
            QString _nomeFilial;

            while (mi.hasNext()) {
                mi.next();
                CadastroFilial *_cfil = new CadastroFilial(this);
                _cfil = mi.value();

                if(_cfil->getID_Empresa() == this->getCodigoEmpresa()) {
                    if(_cfil->getID_Filial() == this->getCodigoFilial()) {
                        _nomeFilial = QString("%0 - %1").arg(_cfil->getID_Filial()).arg(_cfil->getFilial());
                    }
                }
            }

            if(_nomeFilial.isEmpty()) {
                QMessageBox::critical(this, tr("Cadastro Filial [!]"), QString("Filial não encontrada [!]"), QMessageBox::Ok);
                ui->campoIDFilial->setFocus();
            } else {
                ui->campoIDFilial->setText(_nomeFilial);
                ui->botaoProcessar->setFocus();
            }
        } else {
            ui->campoIDFilial->setFocus();
        }
    }
}

void PlanoDeContas::filialSelecionada(const QString _ID_Filial)
{
    if(this->getCodigoEmpresa().isEmpty()) {
        QMessageBox::critical(this, tr("Seleção de Filtro"), QString("Nenhuma Empresa Selecionada"), QMessageBox::Ok);
        ui->campoIDEmpresa->setFocus();
    } else {
        QMapIterator<int, CadastroFilial*> mi(getMapFiliais());
        QString _nomeFilial;

        while (mi.hasNext()) {
            mi.next();
            CadastroFilial *_cfil = new CadastroFilial(this);
            _cfil = mi.value();
            QString _ID_Empresa = this->getCodigoEmpresa();
            if(_cfil->getID_Empresa() == _ID_Empresa && _cfil->getID_Filial() == _ID_Filial) {
                _nomeFilial = QString("%0 - %1").arg(_cfil->getID_Filial()).arg(_cfil->getFilial());
            }
        }

        if(_nomeFilial.isEmpty()) {
            QMessageBox::critical(this, tr("Cadastro Filial [!]"), QString("Filial Inválida [!]"), QMessageBox::Ok);
            ui->campoIDFilial->setFocus();
        } else {
            ui->campoIDFilial->setText(_nomeFilial);
            ui->botaoProcessar->setFocus();
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

void PlanoDeContas::caixaMensagemUsuario(const QString msg)
{
    QMessageBox::information(this, tr("Exportação de Dados"), QString(msg), QMessageBox::Ok);
}

QString PlanoDeContas::pesquisarItem()
{
    return QString("");
}

void PlanoDeContas::pesquisarEmpresa()
{
    Pesquisar * p = new Pesquisar(this, "", 1);
    p->setWindowModality(Qt::ApplicationModal);
    p->setWindowFlags(Qt::Window);
    p->setWindowTitle("Selecionar Empresa");
    connect(p, SIGNAL(getEmpresa(QString)), this, SLOT(setEmpresa(QString)));
    p->show();
}

void PlanoDeContas::pesquisarFilial()
{
    if(this->getCodigoEmpresa().isEmpty()) {
        QMessageBox::critical(this, tr("Erro: Pesquisar Cadastro de Filial"), QString("Você precisa selecionar uma empresa!"), QMessageBox::Ok);
        ui->campoIDEmpresa->setFocus();
    } else {
        Pesquisar * p = new Pesquisar(this, this->getCodigoEmpresa(), 2);
        p->setWindowModality(Qt::ApplicationModal);
        p->setWindowFlags(Qt::Window);
        p->setWindowTitle("Selecionar Filial");
        connect(p, SIGNAL(getFilial(QString)), this, SLOT(setFilial(QString)));
        p->show();
    }
}

void PlanoDeContas::setEmpresa(const QString e)
{
    this->setCodigoEmpresa(e);
    this->empresaSelecionada();
}

void PlanoDeContas::setFilial(const QString f)
{
    this->setCodigoFilial(f);
    this->filialSelecionada();
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

    int tipoCalculo = 0;
    switch (ui->comboBoxTiposCalculos->currentIndex()) {
    case 0: tipoCalculo = 11; break;
    case 1: tipoCalculo = 14; break;
    case 2: tipoCalculo = 31; break;
    case 3: tipoCalculo = 32; break;
    }

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
    emit retornaPlanoDeContas(__idEmp, __idFil, __datIni, __datFim, tipoCalculo);
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
        switch (coluna) {
        case ID_EMPRESA: this->inserirItemTabela(linha, coluna, evento->getID_Empresa());break;
        case EMPRESA: this->inserirItemTabela(linha, coluna, evento->getEmpresa());break;
        case ID_FILIAL: this->inserirItemTabela(linha, coluna, evento->getID_Filial());break;
        case FILIAL: this->inserirItemTabela(linha, coluna, evento->getFilial());break;
        case CNPJ: this->inserirItemTabela(linha, coluna, evento->getCnpj());break;
        case CIDADE_REGIAO: this->inserirItemTabela(linha, coluna, evento->getCidadeRegiao());break;
        case CALCULO: this->inserirItemTabela(linha, coluna, evento->getCalculo());break;
        case COMPETENCIA: this->inserirItemTabela(linha, coluna, evento->getCompetencia().toString("dd/MM/yyyy"));break;
        case TIPO_CALCULO: this->inserirItemTabela(linha, coluna, evento->getTipoDeCalculo());break;
        case SETOR: this->inserirItemTabela(linha, coluna, evento->getSetor());break;
        case CODEVENTO: this->inserirItemTabela(linha, coluna, evento->getCodigoEvento());break;
        case TIPOEVENTO: this->inserirItemTabela(linha, coluna, evento->getTipoEvento());break;
        case EVENTO: this->inserirItemTabela(linha, coluna, evento->getEvento());break;
        case VALOR: this->inserirItemTabela(linha, coluna, evento->getValorEvento());break;
        }
    }
}

void PlanoDeContas::exportarParaExcel()
{
    QString nomeArquivoTitulo = "";
    if(ui->campoIDEmpresa->text().isEmpty() | ui->campoIDFilial->text().isEmpty())
        nomeArquivoTitulo = "FullData";
    else
        nomeArquivoTitulo = ui->campoIDFilial->text().replace(' ','_');

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

void PlanoDeContas::mensagemInfoUsuario(const QString t, const QString m)
{
    QMessageBox::critical(this, QString(t), QString("Nenhum resultado obtido!\n").append(m), QMessageBox::Ok);
}

void PlanoDeContas::filtroItemTabela(const QString filtro)
{
    QLocale local = this->locale();
    double dValue = 0.0;
    int registros = 0;
    for( int i = 0; i < ui->tableWidget->rowCount(); ++i ) {
        bool match = false;
        for( int j = 0; j < ui->tableWidget->columnCount(); ++j ) {
            QTableWidgetItem *item = ui->tableWidget->item( i, j );
            if( item->text().contains(filtro) ) {
                dValue+= local.toDouble(ui->tableWidget->item(i, 12)->text());
                registros++;
                match = true;
                break;
            }
        }
        ui->tableWidget->setRowHidden( i, !match );
    }

    ui->campoTotalRegistros->setText(QString("%L1").arg(registros, 0, 'f', 4, Qt::AlignRight));
    ui->campoValorTotalAgrupado->setText(QString("%L1").arg(dValue, 0, 'f', 4, Qt::AlignRight));
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

QString PlanoDeContas::getCodigoFilial() const
{
    return codigoFilial;
}

void PlanoDeContas::setCodigoFilial(const QString &value)
{
    codigoFilial = value;
}

QString PlanoDeContas::getCodigoEmpresa() const
{
    return codigoEmpresa;
}

void PlanoDeContas::setCodigoEmpresa(const QString &value)
{
    codigoEmpresa = value;
}

QMap<int, CadastroFilial *> PlanoDeContas::getMapFiliais() const
{
    return mapFiliais;
}

QMap<int, CadastroEmpresa *> PlanoDeContas::getMapEmpresas() const
{
    return mapEmpresas;
}
