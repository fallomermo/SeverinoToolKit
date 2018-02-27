#include "metaretencaoestruturada.h"
#include "ui_metaretencaoestruturada.h"
#include "caixamensagemprogresso.h"
#include "detalhesretencao.h"
#include <qcustomplot.h>

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
    gridLayout = new QGridLayout();

    connect(ui->inicioPeriodo, SIGNAL(editingFinished()), this, SLOT(focusPeriodoInicial()));
    connect(ui->finalPeriodo, SIGNAL(editingFinished()), this, SLOT(focusPeriodoFinal()));
    connect(ui->botaoProcessar,SIGNAL(clicked(bool)), this, SLOT(getDatatable()));
    connect(ui->botaoImprimir, SIGNAL(clicked(bool)), this, SLOT(imprimirPlotagemGrafico()));
    connect(ui->tableWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(detalhesRetencao(QModelIndex)));
    connect(ui->tableWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(detalhesRetencao()));
    connect(ui->botaoDetalhes, SIGNAL(clicked(bool)), this, SLOT(detalhesRetencao()));
    connect(ui->girarEtiquetas, SIGNAL(sliderMoved(int)), this, SLOT(girarEtiquetas(int)));
    connect(ui->botaoSalvarScreenshot, SIGNAL(clicked(bool)), this, SLOT(salvarImagemGrafico()));

    QStringList labels = (QStringList() << "ANALISTAS" << "ADMITIDOS" << "DEMITIDOS" << "% RETENÇÃO" << "AÇÃO");
    ui->tableWidget->setColumnCount(labels.count());
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    ui->tableWidget->resizeColumnsToContents();
    this->detalhesRetencao();
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

void MetaRetencaoEstruturada::inserirItemTabela(int r, int c, QWidget *w)
{
    ui->tableWidget->setCellWidget(r, c, (QWidget*)w);
}

void MetaRetencaoEstruturada::inserirLinhaTabela(int linha, int nrColunas, ResponsavelSelecaoAgregado *objeto)
{
    for (int coluna = 0; coluna < nrColunas; ++coluna) {
        switch (coluna) {
        case RECRUTA: inserirItemTabela(linha, coluna, objeto->getReposavel()); break;
        case ADMITIDOS: inserirItemTabela(linha, coluna, objeto->getNumeroAdmitidos() ); break;
        case DEMITIDOS: inserirItemTabela(linha, coluna, objeto->getNumeroDemitidos() ); break;
        case RETENCAO: {
            double percentualRetido = 0.0;
            if(objeto->getNumeroDemitidos() <= objeto->getNumeroAdmitidos())
                percentualRetido = ( static_cast<double>(objeto->getNumeroDemitidos()) / static_cast<double>(objeto->getNumeroAdmitidos()) ) * 100;
            inserirItemTabela(linha, coluna, percentualRetido );
        } break;
        case ACOES: {
            QWidget *w = new QWidget();
            QPushButton *botaoRemove = new QPushButton(this);
            botaoRemove->setIcon(QIcon(":/images/trash.png"));
            QHBoxLayout* l = new QHBoxLayout(w);
            connect(botaoRemove, &QPushButton::clicked, [=]() {removerItemTabela(w);});
            l->addWidget(botaoRemove);
            l->setAlignment(Qt::AlignCenter);
            l->setContentsMargins(0, 0, 0, 0);
            w->setLayout(l);
            inserirItemTabela(linha, coluna, w);
        } break;
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
    this->updateDadosGrafico();
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
    if(QMessageBox::question(this, QString("Remover ( %0 )").arg(item_sel),
                             QString("Deseja realmente remover esse registro\n%0?").arg(item_sel),
                             QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
    {
        ui->tableWidget->removeRow(linha);
        this->updateDadosGrafico();
    }

}

void MetaRetencaoEstruturada::removerItemTabela(const QWidget *sw)
{
    for (int linha = 0; linha < ui->tableWidget->rowCount(); ++linha) {

        if(ui->tableWidget->cellWidget(linha, 4) == sw) {
            QString item_sel = QString(ui->tableWidget->item(linha, 0)->text());
            if(QMessageBox::question(this, QString("Remover ( %0 )").arg(item_sel),
                                     QString("Deseja realmente remover esse registro\n%0?").arg(item_sel),
                                     QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes)
            {
                ui->tableWidget->removeRow(linha);
                this->updateDadosGrafico();
            }
        }
    }
}

void MetaRetencaoEstruturada::detalhesRetencao()
{
    if((ui->tableWidget->rowCount() <= 0) || (ui->tableWidget->currentRow() < 0))
        return;

    qDebug() << "Clicado em: " << ui->tableWidget->item(ui->tableWidget->currentRow(), 0)->text();
}

void MetaRetencaoEstruturada::detalhesRetencao(QModelIndex index)
{
    if(ui->tableWidget->rowCount() <=0)
        return;

    if(ui->tableWidget->currentRow() < 0)
        return;

    QString __responsavel = ui->tableWidget->item(index.row(), 0)->text();
    QString __admitidos = ui->tableWidget->item(index.row(), 1)->text();
    QString __demitidos = ui->tableWidget->item(index.row(), 2)->text();
    QString __percentual = ui->tableWidget->item(index.row(), 3)->text();
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

void MetaRetencaoEstruturada::updateDadosGrafico()
{
    // limpando plotagens antigas
    ui->customPlot->clearPlottables();

    // set dark background gradient:
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));
    gradient.setColorAt(0.38, QColor(105, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));
    ui->customPlot->setBackground(QBrush(gradient));
    ui->customPlot->yAxis->setLabel(QString("Valores"));

    // create empty bar chart objects:
    QCPBars *regen = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    QCPBars *nuclear = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    QCPBars *fossil = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    regen->setAntialiased(false); // gives more crisp, pixel aligned bar borders
    nuclear->setAntialiased(false);
    fossil->setAntialiased(false);
    regen->setStackingGap(1);
    nuclear->setStackingGap(1);
    fossil->setStackingGap(1);

    // Definir Nomes e Cores para as Barras:
    fossil->setName("Admitidos");
    fossil->setPen(QPen(QColor(111, 9, 176).lighter(170)));
    fossil->setBrush(QColor(111, 9, 176));

    nuclear->setName("Demitidos");
    nuclear->setPen(QPen(QColor(250, 170, 20).lighter(150)));
    nuclear->setBrush(QColor(250, 170, 20));

    regen->setName("Retenção");
    regen->setPen(QPen(QColor(0, 168, 140).lighter(130)));
    regen->setBrush(QColor(0, 168, 140));

    // Barras de Pilha Uma Sobre a Outra:
    nuclear->moveAbove(fossil);
    regen->moveAbove(nuclear);

    // Preparando o Eixo Y-Vertical:
    ui->customPlot->yAxis->setRange(0, 12.1);
    ui->customPlot->yAxis->setPadding(5); // a bit more space to the left border
    ui->customPlot->yAxis->setLabel("Meta Retenção\nAno 2017");
    ui->customPlot->yAxis->setBasePen(QPen(Qt::white));
    ui->customPlot->yAxis->setTickPen(QPen(Qt::white));
    ui->customPlot->yAxis->setSubTickPen(QPen(Qt::white));
    ui->customPlot->yAxis->grid()->setSubGridVisible(true);
    ui->customPlot->yAxis->setTickLabelColor(Qt::white);
    ui->customPlot->yAxis->setLabelColor(Qt::white);
    ui->customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
    ui->customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

    // Preparando o Eixo X-Horizontal com os labels Recrutadores:
    QVector<double> ticks;
    QVector<QString> labels;

    // Adicionando Dados Obtidos Através da QTableWidget:
    QVector<double> fossilData, nuclearData, regenData;
    QLocale local = this->locale();
    for (int linha = 0; linha < ui->tableWidget->rowCount(); ++linha) {
        ticks << linha;
        labels << ui->tableWidget->item(linha, 0)->text();
        fossilData << local.toDouble(ui->tableWidget->item(linha, 1)->text());
        nuclearData << local.toDouble(ui->tableWidget->item(linha, 2)->text());
        regenData << local.toDouble(ui->tableWidget->item(linha, 3)->text());
    }

    fossil->setData(ticks, fossilData);
    nuclear->setData(ticks, nuclearData);
    regen->setData(ticks, regenData);

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(ticks, labels);

    ui->customPlot->xAxis->setTicker(textTicker);
    if(ui->girarEtiquetas->value() == 0) {
        ui->customPlot->xAxis->setTickLabelRotation(16);
        ui->girarEtiquetas->setValue(16);
        ui->customPlot->xAxis->setLabel(QString("Responsável pela Seleção\nRotação: %0 graus").arg(QString::number(ui->girarEtiquetas->value())));
    } else {
        ui->customPlot->xAxis->setTickLabelRotation(ui->girarEtiquetas->value());
        ui->customPlot->xAxis->setLabel(QString("Responsável pela Seleção\nRotação: %0 graus").arg(QString::number(ui->girarEtiquetas->value())));
    }
    ui->customPlot->xAxis->setSubTicks(false);
    ui->customPlot->xAxis->setTickLength(0, 4);
    ui->customPlot->xAxis->setRange(0, 8);
    ui->customPlot->xAxis->setBasePen(QPen(Qt::white));
    ui->customPlot->xAxis->setTickPen(QPen(Qt::white));
    ui->customPlot->xAxis->grid()->setVisible(true);
    ui->customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    ui->customPlot->xAxis->setTickLabelColor(Qt::white);
    ui->customPlot->xAxis->setLabelColor(Qt::white);

    // Iniciando Legendas:
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    ui->customPlot->legend->setBrush(QColor(255, 255, 255, 100));
    ui->customPlot->legend->setBorderPen(Qt::NoPen);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->customPlot->rescaleAxes(true);
    ui->customPlot->replot();

    this->detalhesRetencao();
}

void MetaRetencaoEstruturada::girarEtiquetas(int g)
{
    ui->customPlot->xAxis->setTickLabelRotation(g);
    ui->customPlot->xAxis->setLabel(QString("Responsável pela Seleção\nRotação: %0 graus").arg(QString::number(ui->girarEtiquetas->value())));
    ui->customPlot->replot();
}

void MetaRetencaoEstruturada::imprimirPlotagemGrafico()
{
    QMessageBox::information(this, tr("Impressão"), QString("Impressão realizada com sucesso!"), QMessageBox::Ok);
    ui->botaoDetalhes->setFocus();
}

void MetaRetencaoEstruturada::salvarImagemGrafico()
{
    QString outputDir = QDir::homePath();
    QString fileName = "graph.jpg" ;
    QFile file(outputDir+"/"+fileName);

    if (!file.open(QIODevice::WriteOnly|QFile::WriteOnly))
    {
        QMessageBox::warning(0,"Could not create Project File",
                             QObject::tr( "\n Could not create Project File on disk"));
    }

    ui->customPlot->saveJpg( fileName,  0, 0, 1.0, -1  );
}

QMap<int, ObjetoRetencao *> MetaRetencaoEstruturada::getMapRetencao() const
{
    return mapRetencao;
}

void MetaRetencaoEstruturada::setMapRetencao(const QMap<int, ObjetoRetencao *> &value)
{
    mapRetencao = value;
}
