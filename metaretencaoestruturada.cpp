#include "metaretencaoestruturada.h"
#include "ui_metaretencaoestruturada.h"
#include "caixamensagemprogresso.h"
#include "detalhesretencao.h"

MetaRetencaoEstruturada::MetaRetencaoEstruturada(QWidget *parent) : QWidget(parent), ui(new Ui::MetaRetencaoEstruturada)
{
    ui->setupUi(this); this->definirParametrosIniciais();
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
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);

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
                                       << "% RETENÇÃO";
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
    connect(this, SIGNAL(fecharCaixaDeMensagem()), caixaMensagem, SLOT(fecharJanela()));
    connect(this, SIGNAL(setProgressoValue(int)), caixaMensagem, SLOT(setProgressValue(int)));
    connect(this, SIGNAL(setMinimumValue(int)), caixaMensagem, SLOT(setMinimumValue(int)));
    connect(this, SIGNAL(setMaximumValue(int)), caixaMensagem, SLOT(setMaximumValue(int)));

    caixaMensagem->setWindowFlag(Qt::Window);
    caixaMensagem->setWindowFlag(Qt::FramelessWindowHint);
    caixaMensagem->setWindowModality(Qt::ApplicationModal);
    caixaMensagem->show();
    qApp->processEvents();

    controle = new ControleDAO(nullptr);
    QThread* threadDAO = new QThread(nullptr);
    controle->moveToThread(threadDAO);

    qRegisterMetaType<QMap<int,ObjetoRetencao*>>("__mapDataRetencaoEstruturada");
    connect(controle, SIGNAL(enviarMetaRetencao(QMap<int,ObjetoRetencao*>)), this, SLOT(preencherTabela(QMap<int,ObjetoRetencao*>)));
    connect(this, SIGNAL(obterMetaRetencao(QDate,QDate)), controle, SLOT(obterMetaRetencao(QDate,QDate)));
    connect(this, SIGNAL(finishThread()), threadDAO, SLOT(quit()), Qt::DirectConnection);
    connect(caixaMensagem, SIGNAL(cancelarProcesso()), threadDAO, SLOT(quit()), Qt::DirectConnection);
    connect(threadDAO, SIGNAL(finished()), controle, SLOT(deleteLater()));

    threadDAO->start(QThread::NormalPriority);
    emit obterMetaRetencao(ui->inicioPeriodo->date(), ui->finalPeriodo->date());
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
    }
}

void MetaRetencaoEstruturada::preencherTabela(const QMap<int, ObjetoRetencao *> __tempMap)
{
    emit finishThread();
    emit setMinimumValue(0);
    emit setMaximumValue(__tempMap.count());
    if(__tempMap.isEmpty()) {
        emit fecharCaixaDeMensagem();
        QMessageBox::information(this, tr("Meta de Retenção"), QString("Nenhuma informação encontrada!"), QMessageBox::Ok);
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
    if(linha < 0) { linha = 0; }

    QTableWidgetItem *item = ui->tableWidget->item(linha, 0);
    QString item_sel = QString(item->text());
    qDebug() << item_sel;
    if(QMessageBox::question(this, QString("Remover ( %0 )").arg(item_sel), QString("Deseja realmente remover esse registro?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
        ui->tableWidget->removeRow(linha);
}

void MetaRetencaoEstruturada::detalhesRetencao(QModelIndex i)
{
    QString __responsavel = ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->text();
    QString __admitidos = ui->tableWidget->item(ui->tableWidget->currentRow(), 1)->text();
    QString __demitidos = ui->tableWidget->item(ui->tableWidget->currentRow(), 2)->text();
    QString __percentual = ui->tableWidget->item(ui->tableWidget->currentRow(), 3)->text();
    QString __periodosel = ui->inicioPeriodo->date().toString("dd/MM/yyyy")+" a "+ui->finalPeriodo->date().toString("dd/MM/yyyy");

    detalhes = new DetalhesRetencao(this, __responsavel, __admitidos, __demitidos, __percentual, __periodosel);
    detalhes->setWindowFlag(Qt::Window);
    detalhes->setWindowModality(Qt::ApplicationModal);
    detalhes->setVisible(true);
    detalhes->show();
}

void MetaRetencaoEstruturada::detalhesRetencao()
{
    if(ui->tableWidget->rowCount() <=0)
        return;

    if(ui->tableWidget->currentRow() < 0)
        return;

    QString __responsavel = ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->text();
    QString __admitidos = ui->tableWidget->item(ui->tableWidget->currentRow(), 1)->text();
    QString __demitidos = ui->tableWidget->item(ui->tableWidget->currentRow(), 2)->text();
    QString __percentual = ui->tableWidget->item(ui->tableWidget->currentRow(), 3)->text();
    QString __periodosel = ui->inicioPeriodo->date().toString("dd/MM/yyyy")+" a "+ui->finalPeriodo->date().toString("dd/MM/yyyy");

    detalhes = new DetalhesRetencao(this, __responsavel, __admitidos, __demitidos, __percentual, __periodosel);
    detalhes->setWindowFlag(Qt::Window);
    detalhes->setWindowModality(Qt::ApplicationModal);
    detalhes->setVisible(true);
    detalhes->show();
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
