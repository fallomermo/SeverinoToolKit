#include "metaretencaoestruturada.h"
#include "ui_metaretencaoestruturada.h"
#include "caixamensagemprogresso.h"
#include "detalhesretencao.h"

MetaRetencaoEstruturada::MetaRetencaoEstruturada(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MetaRetencaoEstruturada)
{
    ui->setupUi(this);
    definirParametrosIniciais();
}

MetaRetencaoEstruturada::~MetaRetencaoEstruturada()
{
    delete ui;
}

void MetaRetencaoEstruturada::definirParametrosIniciais()
{
    ui->inicioPeriodo->setToolTip(QString("Data de inicio do período.\nIsso não pode ultrapassar 90 dias!"));
    ui->finalPeriodo->setToolTip(QString("Data final do período.\nIsso não pode ultrapassar 90 dias!"));
    ui->inicioPeriodo->setDate(QDateTime::currentDateTime().date().addMonths(-2));
    ui->finalPeriodo->setDate(QDateTime::currentDateTime().date());

    connect(ui->inicioPeriodo, SIGNAL(editingFinished()), this, SLOT(focusPeriodoInicial()));
    connect(ui->finalPeriodo, SIGNAL(editingFinished()), this, SLOT(focusPeriodoFinal()));
    connect(ui->botaoProcessar,SIGNAL(clicked(bool)), this, SLOT(getDatatable()));
    connect(ui->botaoExportar, SIGNAL(clicked(bool)), this, SLOT(exportarParaExcel()));
    connect(ui->tableWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(detalhesRetencao(QModelIndex)));
    connect(ui->tableWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(itemClicado(QModelIndex)));
    connect(ui->botaoDetalhes, SIGNAL(clicked(bool)), this, SLOT(detalhesRetencao()));

    QStringList labels = QStringList() << "ANALISTAS"
                                       << "ADMITIDOS"
                                       << "DEMITIDOS"
                                       << "%"
                                       << "AÇÃO";
    ui->tableWidget->setColumnCount(labels.count());
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    ui->tableWidget->resizeColumnsToContents();
}

void MetaRetencaoEstruturada::filtroItemTabela(QString filter)
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

void MetaRetencaoEstruturada::focusPeriodoInicial()
{
    ui->finalPeriodo->setFocus();
}

void MetaRetencaoEstruturada::focusPeriodoFinal()
{
    ui->botaoProcessar->setFocus();
}

void MetaRetencaoEstruturada::getDatatable()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    caixaMensagem = new CaixaMensagemProgresso(this);
    connect(this, SIGNAL(fecharCaixaDeMensagem()),
            caixaMensagem, SLOT(fecharJanela()));
    connect(this, SIGNAL(setProgressoValue(int)),
            caixaMensagem, SLOT(setProgressValue(int)));
    connect(this, SIGNAL(setMinimumValue(int)),
            caixaMensagem, SLOT(setMinimumValue(int)));
    connect(this, SIGNAL(setMaximumValue(int)),
            caixaMensagem, SLOT(setMaximumValue(int)));

    caixaMensagem->setWindowFlag(Qt::Window);
    caixaMensagem->setWindowFlag(Qt::FramelessWindowHint);
    caixaMensagem->setWindowModality(Qt::ApplicationModal);
    caixaMensagem->setWindowTitle(QString("Trabalhando em sua requisição..."));
    caixaMensagem->show();
    qApp->processEvents();

    controle = new ControleDAO(nullptr);
    QThread* threadDAO = new QThread(nullptr);
    controle->moveToThread(threadDAO);

    QDate _tempDateIni = ui->inicioPeriodo->date();
    QDate _tempDateFim = ui->finalPeriodo->date();
    int _anoComIni = _tempDateIni.year();
    int _mesComIni = _tempDateIni.month();
    int _diaComIni = 1;
    QDate __dataIni( _anoComIni, _mesComIni, _diaComIni );

    int _anoComFim = _tempDateFim.year();
    int _mesComFim = _tempDateFim.month();
    int _diaComFim = 0;

    if((_mesComFim%2) == 0)
        _diaComFim = 30;
    else
        _diaComFim = 31;

    QDate __dataFim( _anoComFim, _mesComFim, _diaComFim );
    qRegisterMetaType<QMap<int,ObjetoRetencao*>>("__mapDataRetencaoEstruturada");
    connect(controle, SIGNAL(enviarMetaRetencao(QMap<int,ObjetoRetencao*>)),
            this, SLOT(preencherTabela(QMap<int,ObjetoRetencao*>)));

    connect(this, SIGNAL(obterMetaRetencao(QDate,QDate)),
            controle, SLOT(obterMetaRetencao(QDate,QDate)));

    connect(this, SIGNAL(finishThread()), threadDAO, SLOT(terminate()));
    connect(caixaMensagem, SIGNAL(cancelarProcesso()), threadDAO, SLOT(terminate()));
    connect(threadDAO, SIGNAL(finished()), controle, SLOT(deleteLater()));

    threadDAO->start(QThread::NormalPriority);

    QVariant variantDataIni = __dataIni;
    QVariant variantDataFim = __dataFim;

    emit obterMetaRetencao(variantDataIni.toDate(), variantDataFim.toDate());
}

void MetaRetencaoEstruturada::inserirItemTabela(int r, int c, QString sValue)
{
    ui->tableWidget->setItem(r,c,new QTableWidgetItem(sValue));
}

void MetaRetencaoEstruturada::inserirItemTabela(int r, int c, QDate dtValue)
{
    ui->tableWidget->setItem(r,c,new QTableWidgetItem(QDate(dtValue).toString("dd/MM/yyyy")));
}

void MetaRetencaoEstruturada::inserirItemTabela(int r, int c, double dValue)
{
    QTableWidgetItem *item = new QTableWidgetItem(QString("%L1").arg(dValue, 0, 'f', 4));
    item->setTextAlignment(Qt::AlignVCenter | Qt::AlignRight);
    ui->tableWidget->setItem(r,c,item);
}

void MetaRetencaoEstruturada::inserirItemTabela(int r, int c, int iValue)
{
    QTableWidgetItem *item = new QTableWidgetItem(QString::number(iValue));
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(r,c,item);
}

void MetaRetencaoEstruturada::inserirItemTabela(int r, int c, QWidget *btn)
{
    ui->tableWidget->setCellWidget(r, c, (QWidget*)btn);
}

void MetaRetencaoEstruturada::inserirLinhaTabela(int linha, int nrColunas, ResponsavelSelecaoAgregado *objeto)
{
    for (int coluna = 0; coluna < nrColunas; ++coluna) {
        if(coluna == 0)
            inserirItemTabela(linha, coluna, objeto->getReposavel() );
        if(coluna == 1)
            inserirItemTabela(linha, coluna, objeto->getNumeroAdmitidos() );
        if(coluna == 2)
            inserirItemTabela(linha, coluna, objeto->getNumeroDemitidos() );
        if(coluna == 3) {
            double percentualRetido = 0.0;
            if(objeto->getNumeroDemitidos() <= objeto->getNumeroAdmitidos())
                percentualRetido = ( static_cast<double>(objeto->getNumeroDemitidos()) / static_cast<double>(objeto->getNumeroAdmitidos()) ) * 100;
            inserirItemTabela(linha, coluna, percentualRetido );
        }
        if(coluna == 4) {
            QWidget* pWidget = new QWidget();
            QToolButton* btn_edit = new QToolButton();
            connect(btn_edit, SIGNAL(clicked(bool)), this, SLOT(removerItemTabela()));
            btn_edit->setIcon(QIcon(":/images/trash.png"));
            btn_edit->setToolTip(QString("Remover item"));
            QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
            pLayout->addWidget(btn_edit);
            pLayout->setAlignment(Qt::AlignCenter);
            pLayout->setContentsMargins(0, 0, 0, 0);
            pLayout->setStretch(0, 0);
            pWidget->setLayout(pLayout);
            inserirItemTabela(linha, coluna, pWidget);
        }
    }
}

void MetaRetencaoEstruturada::preencherTabela(const QMap<int, ObjetoRetencao *> __tempMap)
{
    emit finishThread();
    emit setMinimumValue(0);
    emit setMaximumValue(__tempMap.count());
    if(__tempMap.isEmpty()) {
        QMessageBox::information(this, tr("Meta de Retenção"), QString("Nenhuma informação encontrada!"), QMessageBox::Ok);
        emit fecharCaixaDeMensagem();
        return;
    }

    QMap<QString, ResponsavelSelecaoAgregado*> __mapCount = this->agregarValores(__tempMap);
    QMapIterator<QString, ResponsavelSelecaoAgregado*> __mapIterator(__mapCount);

    ui->tableWidget->setRowCount(__mapCount.count());
    int linha = 0;

    while (__mapIterator.hasNext()) {
        __mapIterator.next();
        emit setProgressoValue(linha);
        ResponsavelSelecaoAgregado *responsavel = __mapIterator.value();
        inserirLinhaTabela(linha, ui->tableWidget->columnCount(), responsavel);
        linha++;
    }
    ui->tableWidget->resizeColumnsToContents();
    emit fecharCaixaDeMensagem();
}

void MetaRetencaoEstruturada::exportarParaExcel()
{
    QString __nomeArquivo = "Listagem_Meta_Retencao_"
            +ui->inicioPeriodo->text().replace('/','-')
            +"_"
            +ui->finalPeriodo->text().replace('/','-');
    ExportarArquivo *exp = new ExportarArquivo(this, ui->tableWidget);
    connect(exp, SIGNAL(mensagemRetorno(QString)), this, SLOT(caixaMensagemUsuario(QString)));
    exp->exportar(__nomeArquivo,1);
}

void MetaRetencaoEstruturada::caixaMensagemUsuario(QString msg)
{
    QMessageBox::information(this, tr("Exportação de Dados"), QString(msg), QMessageBox::Ok);
}

void MetaRetencaoEstruturada::atualizarResultados(QModelIndex i)
{
    QTableWidget *tabela = ui->tableWidget;
    QTableWidgetItem *_item = tabela->item(i.row(), i.column());
    qDebug() << _item->column();
}

void MetaRetencaoEstruturada::setRetencao(const QMap<int, ObjetoRetencao *> &value)
{
    setMapRetencao(value);
}

void MetaRetencaoEstruturada::removerItemTabela()
{
    int linha = ui->tableWidget->currentRow();
    if(QMessageBox::question(this, tr("Remover Item"), QString("Deseja realmente remover a linha selecionada?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        ui->tableWidget->removeRow(linha);
}

void MetaRetencaoEstruturada::detalhesRetencao(QModelIndex i)
{
    QLocale local = this->locale();
    QString __responsavel = ui->tableWidget->item(i.row(), 0)->text();
    QString __admitidos = ui->tableWidget->item(i.row(), 1)->text();
    QString __demitidos = ui->tableWidget->item(i.row(), 2)->text();
    QString __percentual = ui->tableWidget->item(i.row(), 3)->text();
    QString __periodosel = ui->inicioPeriodo->date().toString("dd/MM/yyyy")
            +" a "+ui->finalPeriodo->date().toString("dd/MM/yyyy");
    double __perRetencao = local.toDouble(__percentual);
    QPixmap __imagemStatus;

    if(__perRetencao > 10.0) {__imagemStatus = QPixmap(":/images/circle_ruim.png");}
    if(__perRetencao >= 6.0 && __perRetencao <= 10.0) {__imagemStatus = QPixmap(":/images/circle_normal.png");}
    if(__perRetencao < 6.0) {__imagemStatus = QPixmap(":/images/circle_otimo.png");}

    detalhes = new DetalhesRetencao(this);
    detalhes->setResponsavelSelecao(__responsavel);
    detalhes->setNumeroAdmitidos(__admitidos);
    detalhes->setNumeroDemitidos(__demitidos);
    detalhes->setPercentualRetido(__percentual);
    detalhes->setPeriodoSelecionado(__periodosel);

    detalhes->setMapRetencao(getMapRetencao());
    detalhes->setModelIndex(i);
    detalhes->setImagemStatus(__imagemStatus);
    detalhes->setWindowFlag(Qt::Window);
    detalhes->setWindowModality(Qt::ApplicationModal);
    detalhes->show();
    qApp->processEvents();
}

void MetaRetencaoEstruturada::detalhesRetencao()
{
    if(ui->tableWidget->rowCount() <=0)
        return;

    if(ui->tableWidget->currentRow() < 0)
        return;

    QLocale local = this->locale();
    QString __responsavel = ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->text();
    QString __admitidos = ui->tableWidget->item(ui->tableWidget->currentRow(), 1)->text();
    QString __demitidos = ui->tableWidget->item(ui->tableWidget->currentRow(), 2)->text();
    QString __percentual = ui->tableWidget->item(ui->tableWidget->currentRow(), 3)->text();
    QString __periodosel = ui->inicioPeriodo->date().toString("dd/MM/yyyy")
            +" a "+ui->finalPeriodo->date().toString("dd/MM/yyyy");
    QPixmap __imagemStatus;
    double __perRetencao = local.toDouble(__percentual);

    if(__perRetencao > 10.0) {__imagemStatus = QPixmap(":/images/circle_ruim.png");}
    if(__perRetencao >= 6.0 && __perRetencao <= 10.0) {__imagemStatus = QPixmap(":/images/circle_normal.png");}
    if(__perRetencao < 6.0) {__imagemStatus = QPixmap(":/images/circle_otimo.png");}

    detalhes = new DetalhesRetencao(this);
    detalhes->setResponsavelSelecao(__responsavel);
    detalhes->setNumeroAdmitidos(__admitidos);
    detalhes->setNumeroDemitidos(__demitidos);
    detalhes->setPercentualRetido(__percentual);
    detalhes->setPeriodoSelecionado(__periodosel);
    detalhes->setMapRetencao(this->getMapRetencao());
    detalhes->setImagemStatus(__imagemStatus);
    detalhes->setWindowFlag(Qt::Window);
    detalhes->setWindowModality(Qt::ApplicationModal);
    detalhes->show();
    qApp->processEvents();
}

QMap<QString, ResponsavelSelecaoAgregado *> MetaRetencaoEstruturada::agregarValores(const QMap<int, ObjetoRetencao *> mapFull)
{
    QMap<QString, ResponsavelSelecaoAgregado*> __tempMap;
    QMapIterator<int, ObjetoRetencao*> __mapIterator(mapFull);
    while (__mapIterator.hasNext()) {
        __mapIterator.next();
        ResponsavelSelecaoAgregado *res = new ResponsavelSelecaoAgregado;
        ObjetoRetencao *reg = __mapIterator.value();
        if(!__tempMap.contains(reg->getRecrutador())) {
            res->setReposavel(reg->getRecrutador());
            res->setNumeroAdmitidos(reg->getAdmitidos());
            res->setNumeroDemitidos(reg->getDemitidos());
            __tempMap.insert(reg->getRecrutador(), res);
        } else {
            res = __tempMap.value(reg->getRecrutador());
            res->setNumeroAdmitidos(res->getNumeroAdmitidos() + reg->getAdmitidos());
            res->setNumeroDemitidos(res->getNumeroDemitidos() + reg->getDemitidos());
            __tempMap.insert(reg->getRecrutador(), res);
        }
    }
    return __tempMap;
}

void MetaRetencaoEstruturada::itemClicado(QModelIndex index)
{
    QTableWidgetItem *itemResponsavel = ui->tableWidget->item(index.row(), 0);
    QTableWidgetItem *itemAdmitidos = ui->tableWidget->item(index.row(), 1);
    QTableWidgetItem *itemDemitidos = ui->tableWidget->item(index.row(), 2);
    QTableWidgetItem *itemPercentual = ui->tableWidget->item(index.row(), 3);
    ui->campoResponsavelSelecao->setText(itemResponsavel->text().trimmed());
    ui->campoNumeroAdmitidos->setText(itemAdmitidos->text().trimmed());
    ui->campoNumeroDemitidos->setText(itemDemitidos->text().trimmed());
    ui->campoPercentualRetido->setText(itemPercentual->text().trimmed());
}

QMap<int, ObjetoRetencao *> MetaRetencaoEstruturada::getMapRetencao() const
{
    return mapRetencao;
}

void MetaRetencaoEstruturada::setMapRetencao(const QMap<int, ObjetoRetencao *> &value)
{
    mapRetencao = value;
}
