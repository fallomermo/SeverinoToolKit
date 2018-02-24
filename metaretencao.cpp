#include "metaretencao.h"
#include "ui_metaretencao.h"

MetaRetencao::MetaRetencao(QWidget *parent) : QWidget(parent), ui(new Ui::MetaRetencao)
{
    ui->setupUi(this);

    ui->periodoInicio->setDate(QDateTime::currentDateTime().date().addMonths(-2));
    ui->periodoFim->setDate(QDateTime::currentDateTime().date());
    connect(ui->periodoInicio, SIGNAL(editingFinished()), this, SLOT(focusPeriodoInicial()));
    connect(ui->periodoFim, SIGNAL(editingFinished()), this, SLOT(focusPeriodoFinal()));
    ui->campoPesquisarObjetosTabela->addAction(QIcon(":/images/search.png"), QLineEdit::TrailingPosition);
    ui->campoPesquisarObjetosTabela->setPlaceholderText(QString("Pesquisar Itens na Tabela"));
    connect(ui->campoPesquisarObjetosTabela, SIGNAL(textChanged(QString)), this, SLOT(filtroItemTabela(QString)));
    connect(ui->botaoProcessar, SIGNAL(clicked(bool)), this, SLOT(getDatatable()));
    connect(ui->botaoExportar, SIGNAL(clicked(bool)), this, SLOT(exportarParaExcel()));

    QStringList labels = QStringList() << "CADASTRO"
                                       << "NOME"
                                       << "DEMISSÃO"
                                       << "ADMISSÃO"
                                       << "CARGO"
                                       << "SETOR"
                                       << "CODIGO CAUSA"
                                       << "CAUSA"
                                       << "CODIGO DA EMPRESA"
                                       << "CODIGO DA FILIAL"
                                       << "FILIAL"
                                       << "RECRUTADOR"
                                       << "ADMITIDOS"
                                       << "DEMITIDOS"
                                       << "AÇÃO";
    ui->tableWidget->setColumnCount(labels.count());
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    ui->tableWidget->resizeColumnsToContents();
}

MetaRetencao::~MetaRetencao()
{
    delete ui;
}

void MetaRetencao::filtroItemTabela(QString filter)
{
    for( int linha = 0; linha < ui->tableWidget->rowCount(); ++linha ) {
        bool match = false;
        for( int coluna = 0; coluna < ui->tableWidget->columnCount() -1; ++coluna ) {
            QTableWidgetItem *item = ui->tableWidget->item( linha, coluna );
            if( item->text().contains(filter) ) {
                match = true;
                break;
            }
        }
        ui->tableWidget->setRowHidden( linha, !match );
    }
}

void MetaRetencao::focusPeriodoInicial()
{
    ui->periodoFim->setFocus();
}

void MetaRetencao::focusPeriodoFinal()
{
    ui->botaoProcessar->setFocus();
}

void MetaRetencao::getDatatable()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    cxMensagem = new CaixaMensagemProgresso(this);
    connect(this, SIGNAL(fecharCaixaDeMensagem()), cxMensagem, SLOT(fecharJanela()));
    connect(this, SIGNAL(minimumProgressValue(int)), cxMensagem, SLOT(setMinimumValue(int)));
    connect(this, SIGNAL(maximumProgressValue(int)), cxMensagem, SLOT(setMaximumValue(int)));
    connect(this, SIGNAL(progressValue(int)), cxMensagem, SLOT(setProgressValue(int)));
    connect(cxMensagem, SIGNAL(cancelarProcesso()), this, SLOT(cancelarOperacao()));
    cxMensagem->setWindowFlag(Qt::Window);
    cxMensagem->setWindowFlag(Qt::FramelessWindowHint);
    cxMensagem->setWindowModality(Qt::ApplicationModal);
    cxMensagem->show();
    qApp->processEvents();

    qRegisterMetaType<QMap<int,ObjetoRetencao*>>("__tempMapRetencao");
    QDate __dataIni( ui->periodoInicio->date().year(), ui->periodoInicio->date().month(), 1 );
    QDate __dataFim( ui->periodoFim->date().year(), ui->periodoFim->date().month(), ui->periodoFim->date().daysInMonth() );
    QThread *threadDAO = new QThread(Q_NULLPTR);
    controle = new ControleDAO(Q_NULLPTR);
    connect(this, SIGNAL(finishThread()), threadDAO, SLOT(quit()), Qt::DirectConnection);
    connect(threadDAO, SIGNAL(finished()), controle, SLOT(deleteLater()));
    connect(this, SIGNAL(obterMetaRetencao(QDate,QDate)), controle, SLOT(obterMetaRetencao(QDate,QDate)));
    connect(controle, SIGNAL(enviarMetaRetencao(QMap<int,ObjetoRetencao*>)),this, SLOT(preencherTabela(QMap<int,ObjetoRetencao*>)));
    controle->moveToThread(threadDAO);
    threadDAO->start(QThread::NormalPriority);
    emit obterMetaRetencao(__dataIni, __dataFim);
}

void MetaRetencao::inserirItemTabela(int r, int c, QString sValue)
{
    ui->tableWidget->setItem(r,c,new QTableWidgetItem(sValue));
}

void MetaRetencao::inserirItemTabela(int r, int c, QDate dtValue)
{
    ui->tableWidget->setItem(r,c,new QTableWidgetItem(QDate(dtValue).toString("dd/MM/yyyy")));
}

void MetaRetencao::inserirItemTabela(int r, int c, double dValue)
{
    QTableWidgetItem *item = new QTableWidgetItem(QString("%L1").arg(dValue, 0, 'f', 4));
    item->setTextAlignment(Qt::AlignRight);
    ui->tableWidget->setItem(r,c,item);
}

void MetaRetencao::inserirItemTabela(int r, int c, int iValue)
{
    QTableWidgetItem *item = new QTableWidgetItem(QString::number(iValue));
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(r,c,item);
}

void MetaRetencao::inserirItemTabela(int r, int c, QWidget *btn)
{
    ui->tableWidget->setCellWidget(r, c, (QWidget*)btn);
}

void MetaRetencao::inserirLinhaTabela(int linha, int nrColunas, ObjetoRetencao * retencao)
{
    for (int coluna = 0; coluna < nrColunas; ++coluna) {
        if(coluna == 0)
            inserirItemTabela(linha, coluna, retencao->getCadastro() );
        if(coluna == 1)
            inserirItemTabela(linha, coluna, retencao->getNome() );
        if(coluna == 2)
            inserirItemTabela(linha, coluna, retencao->getDemissao() );
        if(coluna == 3)
            inserirItemTabela(linha, coluna, retencao->getAdmissao() );
        if(coluna == 4)
            inserirItemTabela(linha, coluna, retencao->getCargo() );
        if(coluna == 5)
            inserirItemTabela(linha, coluna, retencao->getSetor() );
        if(coluna == 6)
            inserirItemTabela(linha, coluna, retencao->getCodigoDaCausa());
        if(coluna == 7)
            inserirItemTabela(linha, coluna, retencao->getCausa() );
        if(coluna == 8)
            inserirItemTabela(linha, coluna, retencao->getCodigoDaEmpresa() );
        if(coluna == 9)
            inserirItemTabela(linha, coluna, retencao->getCodigoDaFilial() );
        if(coluna == 10)
            inserirItemTabela(linha, coluna, retencao->getFilial() );
        if(coluna == 11)
            inserirItemTabela(linha, coluna, retencao->getRecrutador() );
        if(coluna == 12)
            inserirItemTabela(linha, coluna, retencao->getAdmitidos() );
        if(coluna == 13)
            inserirItemTabela(linha, coluna, retencao->getDemitidos() );
        if(coluna == 14) {
            QWidget* pWidget = new QWidget();
            QPushButton* btn_edit = new QPushButton(this);
            connect(btn_edit, SIGNAL(clicked(bool)), this, SLOT(removerItemTabela()));
            btn_edit->setIcon(QIcon(":/images/trash.png"));
            QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
            pLayout->addWidget(btn_edit);
            pLayout->setAlignment(Qt::AlignCenter);
            pLayout->setContentsMargins(0, 0, 0, 0);
            pWidget->setLayout(pLayout);
            inserirItemTabela(linha, coluna, pWidget);
        }
    }
}

void MetaRetencao::exportarParaExcel()
{
    QString __nomeArquivo = "Listagem_Meta_Retencao_"+ui->periodoInicio->text().replace('/','-')+"_"+ui->periodoFim->text().replace('/','-');
    ExportarArquivo *exp = new ExportarArquivo(this, ui->tableWidget);
    connect(exp, SIGNAL(mensagemRetorno(QString)), this, SLOT(caixaMensagemUsuario(QString)));
    exp->exportar(__nomeArquivo, 1);
}

void MetaRetencao::caixaMensagemUsuario(QString msg)
{
    QMessageBox::information(this, tr("Exportação de Dados"), QString(msg), QMessageBox::Ok);
}

void MetaRetencao::atualizarResultados()
{
    QTableWidget *tabela = ui->tableWidget;
    int numeroAdmitidos = 0;
    int numeroDemitidos = 0;
    for (int linha = 0; linha < tabela->rowCount(); ++linha) {
        if(tabela->item(linha, 12)->text().toInt(nullptr) == 1)
            numeroAdmitidos++;
        if(tabela->item(linha, 13)->text().toInt(nullptr) == 1)
            numeroDemitidos++;
    }

    double percentualRetido = 0.0;
    if(numeroDemitidos <= numeroAdmitidos)
        percentualRetido = (static_cast<double>(numeroDemitidos) / static_cast<double>(numeroAdmitidos)) * 100;

    ui->campoPercentualRetido->setText(QString("%L1").arg(percentualRetido, 0, 'f', 2, Qt::AlignLeft));
    ui->campoNumeroDeAdmitidos->setText(QString::number(numeroAdmitidos));
    ui->campoNumeroDeDemitidos->setText(QString::number(numeroDemitidos));
}

void MetaRetencao::setRetencao(const QMap<int, ObjetoRetencao *> &value)
{
    setMapRetencao(value);
}

void MetaRetencao::preencherTabela(QMap<int, ObjetoRetencao *> __tempMap)
{
    if(__tempMap.isEmpty()) {
        emit finishThread();
        emit fecharCaixaDeMensagem();
        QMessageBox::information(this, tr("Meta de Retenção"), QString("Nenhuma informação encontrada!"), QMessageBox::Ok);
        return;
    }

    QMapIterator<int, ObjetoRetencao*> __mapIterator(__tempMap);
    emit maximumProgressValue(__tempMap.count());
    ui->tableWidget->setRowCount(__tempMap.count());
    int linha = 0;

    int numeroAdmitidos = 0;
    int numeroDemitidos = 0;

    while (__mapIterator.hasNext()) {
        __mapIterator.next();
        emit progressValue(__mapIterator.key());
        ObjetoRetencao *retencao = __mapIterator.value();
        if(retencao->getAdmitidos() == 1)
            numeroAdmitidos++;

        if(retencao->getDemitidos() == 1)
            numeroDemitidos++;

        inserirLinhaTabela(linha, ui->tableWidget->columnCount(), retencao);
        linha++;
    }
    ui->tableWidget->resizeColumnsToContents();

    double percentualRetido = 0.0;
    if(numeroDemitidos <= numeroDemitidos)
        percentualRetido = (static_cast<double>(numeroDemitidos) / static_cast<double>(numeroAdmitidos)) * 100;

    ui->campoPercentualRetido->setText(QString("%L1").arg(percentualRetido, 0, 'f', 2, Qt::AlignLeft));
    ui->campoNumeroDeAdmitidos->setText(QString::number(numeroAdmitidos));
    ui->campoNumeroDeDemitidos->setText(QString::number(numeroDemitidos));
    emit finishThread();
    emit fecharCaixaDeMensagem();
}

void MetaRetencao::removerItemTabela()
{
    int linha = ui->tableWidget->currentRow();
    if(QMessageBox::question(this, tr("Remover Item"), QString("Deseja realmente remover a linha selecionada?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
        ui->tableWidget->removeRow(linha);
        this->atualizarResultados();
    }
}

void MetaRetencao::cancelarOperacao()
{
    if(QMessageBox::question(this, tr("Cancelar"), QString("Deseja realmente cancelar a requisição?"), QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
        emit finishThread();
        emit fecharCaixaDeMensagem();
    }
}

QMap<int, ObjetoRetencao *> MetaRetencao::getMapRetencao() const
{
    return mapRetencao;
}

void MetaRetencao::setMapRetencao(const QMap<int, ObjetoRetencao *> &value)
{
    mapRetencao = value;
}
