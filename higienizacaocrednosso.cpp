#include "higienizacaocrednosso.h"
#include "ui_higienizacaocrednosso.h"
#include "controledao.h"
#include "caixamensagemprogresso.h"

HigienizacaoCrednosso::HigienizacaoCrednosso(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HigienizacaoCrednosso)
{
    ui->setupUi(this);
    ui->campoDataReferencia->setDate(QDateTime::currentDateTime().date());
    connect(ui->botaoProcessar,SIGNAL(clicked(bool)), this, SLOT(getDatatable()));
    connect(ui->botaoExportar,SIGNAL(clicked(bool)), this, SLOT(exportarParaExcel()));
    ui->campoPesquisarObjetosTabela->addAction(QIcon(":/images/filter.png"), QLineEdit::TrailingPosition);
    ui->campoPesquisarObjetosTabela->setPlaceholderText(QString("Pesquisar Itens na Tabela..."));
    connect(ui->campoPesquisarObjetosTabela, SIGNAL(textChanged(QString)), this, SLOT(filtroItemTabela(QString)));
}

HigienizacaoCrednosso::~HigienizacaoCrednosso()
{
    delete ui;
}

void HigienizacaoCrednosso::getDatatable()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    progresso = new CaixaMensagemProgresso(this);
    connect(this, SIGNAL(fecharCaixaMensagem()), progresso, SLOT(fecharJanela()));
    progresso->setWindowFlag(Qt::Window);
    progresso->setWindowFlag(Qt::FramelessWindowHint);
    progresso->setWindowModality(Qt::ApplicationModal);
    progresso->setVisible(true);
    progresso->setWindowTitle(QString("Trabalhando em sua requisição..."));
    progresso->show();
    qApp->processEvents();

    if(ui->campoListaOpcoes->currentIndex() == 0) {
        qRegisterMetaType<QMap<int, ObjetoCrednossoRetorno*>>("__metaDataCrednossoRetorno");
        QThread *processo = new QThread;
        controle = new ControleDAO(nullptr);
        controle->moveToThread(processo);

        connect(this, SIGNAL(finishThread()), processo, SLOT(terminate()));
        connect(processo, SIGNAL(finished()), controle, SLOT(deleteLater()));
        connect(controle, SIGNAL(enviarCrednossoRetorno(QMap<int,ObjetoCrednossoRetorno*>)), this, SLOT(preencherTabela(QMap<int,ObjetoCrednossoRetorno*>)));
        connect(this, SIGNAL(obterCrednossoRetorno(QDate)), controle, SLOT(obterCrednossoRetorno(QDate)));

        processo->start(QThread::NormalPriority);
        QVariant dateVariant = ui->campoDataReferencia->date();
        emit obterCrednossoRetorno(dateVariant.toDate());
    }
}

void HigienizacaoCrednosso::inserirItemTabela(int r, int c, QString sValue)
{
    ui->tableWidget->setItem(r,c,new QTableWidgetItem(sValue));
}

void HigienizacaoCrednosso::inserirItemTabela(int r, int c, QDate dtValue)
{
    ui->tableWidget->setItem(r,c,new QTableWidgetItem(QDate(dtValue).toString("dd/MM/yyyy")));
}

void HigienizacaoCrednosso::inserirItemTabela(int r, int c, double dValue)
{
    QTableWidgetItem *item = new QTableWidgetItem(QString("%L1").arg(dValue, 0, 'f', 4));
    item->setTextAlignment(Qt::AlignRight);
    ui->tableWidget->setItem(r,c,item);
}

void HigienizacaoCrednosso::inserirItemTabela(int r, int c, int iValue)
{
    QTableWidgetItem *item = new QTableWidgetItem(QString::number(iValue));
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(r,c,item);
}

void HigienizacaoCrednosso::inserirLinhaTabela(int linha, int nrColunas, ObjetoCrednossoRetorno *objeto)
{
    for (int coluna = 0; coluna < nrColunas; ++coluna) {
        if(coluna == 0)
            inserirItemTabela(linha, coluna, objeto->getCodigoDaEmpresa());
        if(coluna == 1)
            inserirItemTabela(linha, coluna, objeto->getCPF());
        if(coluna == 2)
            inserirItemTabela(linha, coluna, objeto->getEmpresa());
        if(coluna == 3)
            inserirItemTabela(linha, coluna, objeto->getCodigoDaFilial());
        if(coluna == 4)
            inserirItemTabela(linha, coluna, objeto->getFilial());
        if(coluna == 5)
            inserirItemTabela(linha, coluna, objeto->getCNPJ());
        if(coluna == 6)
            inserirItemTabela(linha, coluna, objeto->getMatricula());
        if(coluna == 7)
            inserirItemTabela(linha, coluna, objeto->getNome());
        if(coluna == 8)
            inserirItemTabela(linha, coluna, objeto->getCodigoDoEvento());
        if(coluna == 9)
            inserirItemTabela(linha, coluna, objeto->getEvento());
        if(coluna == 10)
            inserirItemTabela(linha, coluna, objeto->getValor());
    }
}

void HigienizacaoCrednosso::exportarParaExcel()
{
    QString nomeArquivoTitulo = "";
    if(ui->campoListaOpcoes->currentIndex() == 0)
        nomeArquivoTitulo = "CrednossoRetornoDescontosFolha";
    else
        nomeArquivoTitulo = "HigienizacaoCrednossoCorte";

    QString __nomeArquivo = "/"+nomeArquivoTitulo
            +"_"+ui->campoDataReferencia->text().replace('/','-');
    ExportarArquivo *exp = new ExportarArquivo(this, ui->tableWidget);
    connect(exp, SIGNAL(mensagemRetorno(QString)), this, SLOT(caixaMensagemUsuario(QString)));
    exp->exportar(__nomeArquivo,0);
}

void HigienizacaoCrednosso::atualizarResultados(QModelIndex index)
{
    qDebug() << index.column();
}

void HigienizacaoCrednosso::mensagemInfoUsuario(QString msg)
{
    QMessageBox::critical(
                this,
                tr("Erro Conexão com Banco de Dados"),
                QString("Não foi possível consultar a base de dado!\n").append(msg), QMessageBox::Ok);
}

void HigienizacaoCrednosso::filtroItemTabela(QString filtro)
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

void HigienizacaoCrednosso::preencherTabela(QMap<int, ObjetoCrednossoRetorno *> __tempMap)
{
    emit finishThread();
    QMapIterator<int,ObjetoCrednossoRetorno *> __tempIterator(__tempMap);
    QStringList labels = QStringList() << "Codigo da Empresa"
                                       << "CPF"
                                       << "Empresa"
                                       << "Codigo da Filial"
                                       << "Filial"
                                       << "CNPJ"
                                       << "Matricula"
                                       << "Nome"
                                       << "Codigo do Evento"
                                       << "Evento"
                                       << "Valor";
    ui->tableWidget->setColumnCount(labels.count());
    ui->tableWidget->setHorizontalHeaderLabels(labels);

    double dValues = 0.0;

    QTableWidget *__tempTable = new QTableWidget;
    __tempTable->setColumnCount(labels.count());
    __tempTable->setRowCount(__tempMap.count());
    ui->campoTotalRegistros->setText(QString::number(__tempMap.count()));
    __tempMap.clear();

    while (__tempIterator.hasNext()) {
        __tempIterator.next();
        ObjetoCrednossoRetorno* objetoAtual = __tempIterator.value();
        if(!__tempMap.contains(objetoAtual->getCPF())) {
            dValues+= objetoAtual->getValor();
            __tempMap.insert(objetoAtual->getCPF(), objetoAtual);
        } else {
            ObjetoCrednossoRetorno *__tempObj = __tempMap.value(objetoAtual->getCPF());
            objetoAtual->setValor(objetoAtual->getValor() + __tempObj->getValor());
            dValues+=__tempObj->getValor();
            __tempMap.insert(objetoAtual->getCPF(), objetoAtual);
        }
    }

    ui->campoValorTotal->setText(QString("%L1").arg(dValues, 0, 'f', 4));
    int linha = 0;
    progresso->setMaximumValue(__tempMap.count());
    ui->tableWidget->setRowCount(__tempMap.count());
    __tempIterator = QMapIterator<int, ObjetoCrednossoRetorno *>(__tempMap);
    while (__tempIterator.hasNext()) {
        __tempIterator.next();
        ObjetoCrednossoRetorno* objeto = __tempIterator.value();
        inserirLinhaTabela(linha, ui->tableWidget->columnCount(), objeto);
        linha++;
    }
    ui->tableWidget->resizeColumnsToContents();
    emit fecharCaixaMensagem();
}

void HigienizacaoCrednosso::caixaMensagemUsuario(QString msg)
{
    QMessageBox::information(this, tr("Exportação de Arquivo"), QString(msg), QMessageBox::Ok);
}
