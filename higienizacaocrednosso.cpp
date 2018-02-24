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
    progresso->show();
    qApp->processEvents();

    if(ui->campoListaOpcoes->currentIndex() == 0) {
        qRegisterMetaType<QMap<int, ObjetoCrednossoRetorno*>>("__metaDataCrednossoRetorno");
        QThread *processo = new QThread(Q_NULLPTR);
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
        switch (coluna) {
        case 0: inserirItemTabela(linha, coluna, objeto->getCPF());break;
        case 1: inserirItemTabela(linha, coluna, objeto->getMatricula());break;
        case 2: inserirItemTabela(linha, coluna, objeto->getNome());break;
        case 3: inserirItemTabela(linha, coluna, 100);break;
        case 4: inserirItemTabela(linha, coluna, objeto->getCodigoDoEvento());break;
        case 5: inserirItemTabela(linha, coluna, objeto->getValor());break;
        }
    }
}

void HigienizacaoCrednosso::exportarParaExcel()
{
    QString nomeArquivoTitulo = "";
    if(ui->campoListaOpcoes->currentIndex() == 0) {
        nomeArquivoTitulo = "CrednossoRetornoDescontosFolha_"+ui->campoDataReferencia->text();
        if(ui->tableWidget->rowCount() <= 0) {
            return;
        } else {
            QFileDialog dialogo;
            dialogo.setDirectory(QDir::homePath());
            QString filename = dialogo.getSaveFileName(this, tr("Exportar para CSV"), QString(nomeArquivoTitulo), "CSV (*.csv);;Texto (*.txt)");
            if(filename.isEmpty())
                return;

            QFile f( filename );
            if (f.open(QFile::WriteOnly | QFile::Truncate)) {
                QTextStream data( &f );
                QStringList strList;
                QTableWidget *nTable = ui->tableWidget;
                if(QMessageBox::question(this, tr("Exportar arquivo..."), QString("Deseja exportar a descrição das colunas?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
                    strList.clear();
                    for (int coluna = 0; coluna < nTable->columnCount(); ++coluna) {
                        strList << nTable->horizontalHeaderItem(coluna)->data(Qt::DisplayRole).toString().trimmed();
                    }
                    data << strList.join( ";" )+"\n";
                }
                for( int r = 0; r < nTable->rowCount(); ++r ) {
                    strList.clear();
                    for( int c = 0; c < nTable->columnCount(); ++c ) {
                        strList << nTable->item( r, c )->text().trimmed();
                    }
                    data << strList.join( ";" )+"\n";
                }
                f.close();
                QMessageBox::information(this, tr("Exportação Concluída"), QString("Arquivo exportado com sucesso!"), QMessageBox::Ok);
            }
        }
    }
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
    QStringList labels = QStringList() << "CPF"
                                       << "Matricula"
                                       << "Nome"
                                       << "Tabela"
                                       << "Codigo do Evento"
                                       << "Valor";
    ui->tableWidget->setColumnCount(labels.count());
    ui->tableWidget->setHorizontalHeaderLabels(labels);

    double dValues = 0.0;

    QTableWidget *__tempTable = new QTableWidget;
    __tempTable->setColumnCount(labels.count());
    __tempTable->setRowCount(__tempMap.count());
    ui->campoTotalRegistros->setText(QString::number(__tempMap.count()));

    int linha = 0;
    progresso->setMaximumValue(__tempMap.count());
    ui->tableWidget->setRowCount(__tempMap.count());
    __tempIterator = QMapIterator<int, ObjetoCrednossoRetorno *>(__tempMap);
    while (__tempIterator.hasNext()) {
        __tempIterator.next();
        ObjetoCrednossoRetorno* objeto = __tempIterator.value();
        inserirLinhaTabela(linha, ui->tableWidget->columnCount(), objeto);
        dValues+=objeto->getValor();
        linha++;
    }

    ui->tableWidget->resizeColumnsToContents();
    ui->campoValorTotal->setText(QString("%L1").arg(dValues, 0, 'f', 4));
    emit fecharCaixaMensagem();
}

void HigienizacaoCrednosso::caixaMensagemUsuario(QString msg)
{
    QMessageBox::information(this, tr("Exportação de Arquivo"), QString(msg), QMessageBox::Ok);
}
