#include "processararquivocrednosso.h"
#include "ui_processararquivocrednosso.h"

ProcessarArquivoCrednosso::ProcessarArquivoCrednosso(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProcessarArquivoCrednosso)
{
    ui->setupUi(this);
    ui->campoPesquisarObjetosTabela->setPlaceholderText(QString("Pesquisar item na tabela..."));
    connect(ui->campoPesquisarObjetosTabela, SIGNAL(textEdited(QString)), this, SLOT(filtroItemTabela(QString)));
    connect(ui->botaoConverter, SIGNAL(clicked(bool)), this, SLOT(converterArquivo()));
    connect(ui->botaoAtualizar, SIGNAL(clicked(bool)), this, SLOT(atualizarDados()));
    connect(ui->botaoExportar, SIGNAL(clicked(bool)), this, SLOT(exportarParaExcel()));
    connect(ui->comboBox, SIGNAL(activated(int)), this, SLOT(abrirArquivo()));
    QStringList itensConversao = QStringList() << "CREDNOSSO FÉRIAS"
                                               << "CREDNOSSO RETORNO"
                                               << "CREDNOSSO GARANTIA EXTENDIDA"
                                               << "CREDNOSSO FÉRIAS DAT"
                                               << "HIGIENIZAÇÃO CREDNOSSO CORTE/MÊS"
                                               << "HIGIENIZAÇÃO CREDNOSSO DEMITIDPS"
                                               << "HIGIENIZAÇÃO CREDNOSSO PLANO DE SAÚDE";
    ui->comboBox->addItems(itensConversao);
}

ProcessarArquivoCrednosso::~ProcessarArquivoCrednosso()
{
    delete ui;
}

void ProcessarArquivoCrednosso::abrirArquivo()
{
    QString local = QFileDialog::getOpenFileName(this, tr("Abrir"), QDir::currentPath(), QString("Arquivo Texto (*.txt);;Arquivo DAT(*.dat);;Todos os arquivos(*.*)"));
    if(!local.isEmpty()) {
        ui->campoLocalDoArquivo->setText(local);
        ui->botaoConverter->setFocus();
    }
}

void ProcessarArquivoCrednosso::converterArquivo()
{
    int control = ui->comboBox->currentIndex();
    qDebug() << control;

    switch (control) {
    case 0: this->converterArquivoCorteFerias();
        break;
    case 1: this->converterArquivo("R");
        break;
    case 2: this->converterArquivoGarantiaCredinosso();
        break;
    case 3: this->converterArquivoCorteFeriasDAT();
        break;
    case 4: this->converterArquivoCorteFeriasDAT();
        break;
    case 5: this->converterArquivoCorteFeriasDAT();
        break;
    case 6: this->converterArquivoCrednossoPlanoSaudeLinhas();
        break;
    default:
        break;
    }
}

void ProcessarArquivoCrednosso::converterArquivo(QString c)
{
    qDebug() << c;
}

void ProcessarArquivoCrednosso::converterArquivoCorteFerias()
{
    QFile file(ui->campoLocalDoArquivo->text().trimmed());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    CaixaMensagemProgresso *caixaMensagem = new CaixaMensagemProgresso(this);
    connect(this, SIGNAL(progressValue(int)), caixaMensagem, SLOT(setProgressValue(int)));
    connect(this, SIGNAL(minimumProgressValue(int)), caixaMensagem, SLOT(setMinimumValue(int)));
    connect(this, SIGNAL(maximumProgressValue(int)), caixaMensagem, SLOT(setMaximumValue(int)));
    connect(this, SIGNAL(fecharCaixaDeMensagem()), caixaMensagem, SLOT(fecharJanela()));
    caixaMensagem->setWindowFlag(Qt::Window);
    caixaMensagem->setWindowFlag(Qt::FramelessWindowHint);
    caixaMensagem->setWindowModality(Qt::ApplicationModal);
    caixaMensagem->setVisible(true);
    caixaMensagem->show();
    qApp->processEvents();

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    QTextStream in(&file);
    int linha = 0;
    emit minimumProgressValue(0);
    while (!in.atEnd()) {
        linha++;
        emit progressValue(linha);
        QString registro = in.readLine();
        QStringList lista = registro.split("|");
        if(linha == 1) {
            QStringList labels;
            ui->tableWidget->setRowCount(linha);
            for (int coluna = 0; coluna < lista.count(); ++coluna) {
                labels << QString("Coluna %0").arg(coluna);
            }
            ui->tableWidget->setColumnCount(labels.count());
            ui->tableWidget->setHorizontalHeaderLabels(labels);
            for (int coluna = 0; coluna < lista.count(); ++coluna) {
                ui->tableWidget->setRowCount(linha);
                ui->tableWidget->setItem(linha-1, coluna, new QTableWidgetItem(lista.at(coluna)));
            }
        } else {
            ui->tableWidget->setRowCount(linha);
            for (int coluna = 0; coluna < ui->tableWidget->columnCount(); ++coluna) {
                ui->tableWidget->setItem(linha-1, coluna, new QTableWidgetItem(lista.at(coluna)));
            }
        }
    }
    emit maximumProgressValue(linha);
    file.close();
    ui->tableWidget->resizeColumnsToContents();
    emit fecharCaixaDeMensagem();
    QMessageBox::information(this, tr("Processamento de arquivo"), QString("Concluído!"), QMessageBox::Ok);
    ui->campoLocalDoArquivo->setFocus();
}

void ProcessarArquivoCrednosso::converterArquivoCorteFeriasDAT()
{
    QFile file(ui->campoLocalDoArquivo->text().trimmed());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    CaixaMensagemProgresso *mensagem = new CaixaMensagemProgresso(this);
    connect(this, SIGNAL(progressValue(int)), mensagem, SLOT(setProgressValue(int)));
    connect(this, SIGNAL(minimumProgressValue(int)), mensagem, SLOT(setMinimumValue(int)));
    connect(this, SIGNAL(maximumProgressValue(int)), mensagem, SLOT(setMaximumValue(int)));
    connect(this, SIGNAL(fecharCaixaDeMensagem()), mensagem, SLOT(fecharJanela()));
    mensagem->setWindowFlag(Qt::Window);
    mensagem->setWindowFlag(Qt::FramelessWindowHint);
    mensagem->setWindowModality(Qt::ApplicationModal);
    mensagem->setMinimumValue(0);
    mensagem->setVisible(true);
    mensagem->show();
    qApp->processEvents();

    QTextStream in(&file);
    int linha = 0;
    while (!in.atEnd()) {
        linha++;
        emit minimumProgressValue(linha);
        QString registro = in.readLine();
        QRegExp rx("^(\\d{2})+(\\d{4})+(\\d{1})+(\\d{9})+(\\d{3})+(\\d{4})+(\\d{11})+(\\d{11})+([A-Z]{1})$");
        rx.indexIn(registro);
        QStringList labels;
        labels = QStringList() << "Linha Completa" << "Indice" << "Empresa" << "Filial" << "Matricula" << "Tabela-Nome" << "Evento" << "Referencia" << "Valor" << "Origem";
        QStringList lista = rx.capturedTexts();
        if(linha == 1) {
            ui->tableWidget->setRowCount(linha);
            ui->tableWidget->setColumnCount(labels.count());
            ui->tableWidget->setHorizontalHeaderLabels(labels);
            for (int coluna = 0; coluna < lista.count(); ++coluna) {
                ui->tableWidget->setRowCount(linha);
                if(coluna == 8) {
                    QString sValue = lista.at(coluna);
                    double dValue = (static_cast<double>(sValue.toInt(nullptr))) / 100;
                    QTableWidgetItem *item = new QTableWidgetItem(QString::number(dValue));
                    item->setTextAlignment(Qt::AlignRight);
                    ui->tableWidget->setItem(linha-1, coluna, item);
                } else {
                    ui->tableWidget->setItem(linha-1, coluna, new QTableWidgetItem(lista.at(coluna)));
                }
            }
        } else {
            ui->tableWidget->setRowCount(linha);
            for (int coluna = 0; coluna < ui->tableWidget->columnCount(); ++coluna) {
                if(coluna == 8) {
                    QString sValue = lista.at(coluna);
                    double dValue = (static_cast<double>(sValue.toInt(nullptr))) / 100;
                    QTableWidgetItem *item = new QTableWidgetItem(QString::number(dValue));
                    item->setTextAlignment(Qt::AlignRight);
                    ui->tableWidget->setItem(linha-1, coluna, item);
                } else {
                    ui->tableWidget->setItem(linha-1, coluna, new QTableWidgetItem(lista.at(coluna)));
                }
            }
        }
    }
    file.close();
    emit fecharCaixaDeMensagem();
    ui->tableWidget->resizeColumnsToContents();
    QMessageBox::information(this, tr("Processamento de arquivo"), QString("Concluído!"), QMessageBox::Ok);
    ui->botaoAtualizar->setFocus();
}

void ProcessarArquivoCrednosso::converterArquivoGarantiaCredinosso()
{
    QFile file(ui->campoLocalDoArquivo->text().trimmed());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    CaixaMensagemProgresso *mensagem = new CaixaMensagemProgresso(this);
    connect(this, SIGNAL(progressValue(int)), mensagem, SLOT(setProgressValue(int)));
    connect(this, SIGNAL(minimumProgressValue(int)), mensagem, SLOT(setMinimumValue(int)));
    connect(this, SIGNAL(maximumProgressValue(int)), mensagem, SLOT(setMaximumValue(int)));
    connect(this, SIGNAL(fecharCaixaDeMensagem()), mensagem, SLOT(fecharJanela()));
    mensagem->setWindowFlag(Qt::Window);
    mensagem->setWindowFlag(Qt::FramelessWindowHint);
    mensagem->setWindowModality(Qt::ApplicationModal);
    mensagem->setMinimumValue(0);
    mensagem->setVisible(true);
    mensagem->show();
    qApp->processEvents();

    QTextStream in(&file);
    int linha = 0;
    int colunaCalculada = 10;
    while (!in.atEnd()) {
        linha++;
        emit minimumProgressValue(linha);
        QString registro = in.readLine();
        QRegExp rx("^(\\d{2})+(\\d{4})+(\\d{1})+(\\d{9})+(\\d{4})+(\\d{3})+(\\d{4})+(\\d{11})+(\\d{11})+(\\d{11})$");
        rx.indexIn(registro);
        QStringList labels;
        labels = QStringList() << "Linha Completa" << "Indice" << "Empresa" << "Filial" << "Matricula" << "Nome" << "Tabela" << "Evento" << "Referencia" << "RAT" << "Valor";
        QStringList lista = rx.capturedTexts();
        if(linha == 1) {
            ui->tableWidget->setRowCount(linha);
            ui->tableWidget->setColumnCount(labels.count());
            ui->tableWidget->setHorizontalHeaderLabels(labels);
            for (int coluna = 0; coluna < lista.count(); ++coluna) {
                ui->tableWidget->setRowCount(linha);
                if(coluna == colunaCalculada) {
                    QString sValue = lista.at(coluna);
                    double dValue = (static_cast<double>(sValue.toInt(nullptr))) / 100;
                    QTableWidgetItem *item = new QTableWidgetItem(QString::number(dValue).replace('.',','));
                    item->setTextAlignment(Qt::AlignRight);
                    ui->tableWidget->setItem(linha-1, coluna, item);
                } else {
                    ui->tableWidget->setItem(linha-1, coluna, new QTableWidgetItem(lista.at(coluna)));
                }
            }
        } else {
            ui->tableWidget->setRowCount(linha);
            for (int coluna = 0; coluna < ui->tableWidget->columnCount(); ++coluna) {
                if(coluna == colunaCalculada) {
                    QString sValue = lista.at(coluna);
                    double dValue = (static_cast<double>(sValue.toInt(nullptr))) / 100;
                    QTableWidgetItem *item = new QTableWidgetItem(QString::number(dValue).replace('.',','));
                    item->setTextAlignment(Qt::AlignRight);
                    ui->tableWidget->setItem(linha-1, coluna, item);
                } else {
                    ui->tableWidget->setItem(linha-1, coluna, new QTableWidgetItem(lista.at(coluna)));
                }
            }
        }
    }
    file.close();
    emit fecharCaixaDeMensagem();
    ui->tableWidget->resizeColumnsToContents();
    QMessageBox::information(this, tr("Processamento de arquivo"), QString("Concluído!"), QMessageBox::Ok);
    ui->botaoAtualizar->setFocus();
}

void ProcessarArquivoCrednosso::atualizarDadosCorteFeriasDATSenior()
{
    CaixaMensagemProgresso *mensagem = new CaixaMensagemProgresso(this);
    connect(this, SIGNAL(progressValue(int)), mensagem, SLOT(setProgressValue(int)));
    connect(this, SIGNAL(minimumProgressValue(int)), mensagem, SLOT(setMinimumValue(int)));
    connect(this, SIGNAL(maximumProgressValue(int)), mensagem, SLOT(setMaximumValue(int)));
    connect(this, SIGNAL(fecharCaixaDeMensagem()), mensagem, SLOT(fecharJanela()));
    mensagem->setWindowFlag(Qt::Window);
    mensagem->setWindowFlag(Qt::FramelessWindowHint);
    mensagem->setWindowModality(Qt::ApplicationModal);
    mensagem->setMinimumValue(0);
    mensagem->setVisible(true);
    mensagem->show();
    qApp->processEvents();

    QTableWidget *tabela = ui->tableWidget;
    qRegisterMetaType<QMap<int,UpdateDataTableColumm*>>("__metaTypeUpdateDataTableColumm");
    QThread *thread_Update = new QThread(nullptr);
    connect(this, SIGNAL(finishThread()), thread_Update, SLOT(terminate()));
    connect(mensagem, SIGNAL(cancelarProcesso()), thread_Update, SLOT(terminate()));
    ControleDAO *controle = new ControleDAO(nullptr);
    connect(thread_Update, SIGNAL(finished()), controle, SLOT(deleteLater()));
    connect(this, SIGNAL(obterUpdateDadosColaborador(int,int,int,int,bool)),
            controle, SLOT(obterUpdateDadosCadastroColaborador(int,int,int,int,bool)));
    connect(controle, SIGNAL(enviarUpdateDadosCadastroColaborador(QMap<int,UpdateDataTableColumm*>)),
            this, SLOT(updateDadosArquivoCrednossoFeriasDAT(QMap<int,UpdateDataTableColumm*>)));
    controle->moveToThread(thread_Update);
    thread_Update->start(QThread::NormalPriority);

    int registros = (tabela->rowCount()) - 1;
    for (int linha = 0; linha < tabela->rowCount(); ++linha) {
        QTableWidgetItem *iemp = ui->tableWidget->item(linha, 2);
        QTableWidgetItem *icad = ui->tableWidget->item(linha, 4);
        int numemp = iemp->text().toInt(nullptr);
        int numcad = icad->text().toInt(nullptr);
        if(linha == registros) {
            emit obterUpdateDadosColaborador(numemp, numcad, linha, 0, true);
        } else {
            emit obterUpdateDadosColaborador(numemp, numcad, linha, 0, false);
        }
    }
}

void ProcessarArquivoCrednosso::converterArquivoCrednossoPlanoSaudeLinhas()
{
    QFile file(ui->campoLocalDoArquivo->text().trimmed());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    CaixaMensagemProgresso *caixaMensagem = new CaixaMensagemProgresso(this);
    connect(this, SIGNAL(progressValue(int)), caixaMensagem, SLOT(setProgressValue(int)));
    connect(this, SIGNAL(minimumProgressValue(int)), caixaMensagem, SLOT(setMinimumValue(int)));
    connect(this, SIGNAL(maximumProgressValue(int)), caixaMensagem, SLOT(setMaximumValue(int)));
    connect(this, SIGNAL(fecharCaixaDeMensagem()), caixaMensagem, SLOT(fecharJanela()));
    caixaMensagem->setWindowFlag(Qt::Window);
    caixaMensagem->setWindowFlag(Qt::FramelessWindowHint);
    caixaMensagem->setWindowModality(Qt::ApplicationModal);
    caixaMensagem->setVisible(true);
    caixaMensagem->show();
    qApp->processEvents();

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    QTextStream in(&file);
    int linha = 0;
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList("Registro"));
    while (!in.atEnd()) {
        linha++;
        emit progressValue(linha);
        QString registro = in.readLine().trimmed();
        ui->tableWidget->setRowCount(linha);
        ui->tableWidget->setItem(linha,0, new QTableWidgetItem(registro));
    }
    file.close();
    emit maximumProgressValue(linha);
    ui->tableWidget->resizeColumnsToContents();
    emit fecharCaixaDeMensagem();
    QMessageBox::information(this, tr("Processamento de arquivo"), QString("Concluído!"), QMessageBox::Ok);
    ui->botaoAtualizar->setFocus();
}

void ProcessarArquivoCrednosso::filtroItemTabela(QString filter)
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

void ProcessarArquivoCrednosso::exportarParaExcel()
{
    QString __nomeArquivo = "/"+ui->comboBox->currentText();
    ExportarArquivo *exp = new ExportarArquivo(this, ui->tableWidget);
    connect(exp, SIGNAL(mensagemRetorno(QString)), this, SLOT(caixaMensagemUsuario(QString)));
    exp->exportar(__nomeArquivo,0);
}

void ProcessarArquivoCrednosso::caixaMensagemUsuario(QString msg)
{
    QMessageBox::information(this, tr("Exportação de Dados"), QString(msg), QMessageBox::Ok);
}

void ProcessarArquivoCrednosso::atualizarDados()
{
    int control = ui->comboBox->currentIndex();
    qDebug() << control;
    switch (control) {
    case 0:
        break;
    case 1:
        break;
    case 2:
        this->updateDadosArquivoCrednossoGarantia();
        break;
    case 3:
        this->atualizarDadosCorteFeriasDATSenior();
        break;
    case 4:
        this->atualizarDadosCorteFeriasDATSenior();
        break;
    default:
        this->atualizarDadosCorteFeriasDATSenior();
        break;
    }
}

void ProcessarArquivoCrednosso::limparDadosTempTable()
{
    __tempTable->setRowCount(1);
}

void ProcessarArquivoCrednosso::updateDadosArquivoCrednossoFerias(QMap<int, ObjetoCadastroUpdateFile *>)
{
    //Para quando for o de férias
}

void ProcessarArquivoCrednosso::updateDadosArquivoCrednossoRetorno(QMap<int, ObjetoCadastroUpdateFile *>)
{
    //Para quando for retorno da folha
}

void ProcessarArquivoCrednosso::updateDadosArquivoCrednossoGarantia()
{
    CaixaMensagemProgresso *mensagem = new CaixaMensagemProgresso(this);
    connect(this, SIGNAL(progressValue(int)), mensagem, SLOT(setProgressValue(int)));
    connect(this, SIGNAL(minimumProgressValue(int)), mensagem, SLOT(setMinimumValue(int)));
    connect(this, SIGNAL(maximumProgressValue(int)), mensagem, SLOT(setMaximumValue(int)));
    connect(this, SIGNAL(fecharCaixaDeMensagem()), mensagem, SLOT(fecharJanela()));
    mensagem->setWindowFlag(Qt::Window);
    mensagem->setWindowFlag(Qt::FramelessWindowHint);
    mensagem->setWindowModality(Qt::ApplicationModal);
    mensagem->setMinimumValue(0);
    mensagem->setVisible(true);
    mensagem->show();
    qApp->processEvents();

    QTableWidget *tabela = ui->tableWidget;
    qRegisterMetaType<QMap<int,UpdateDataTableColumm*>>("__metaTypeUpdateDataTableColumm");
    QThread *thread_Update = new QThread(nullptr);
    connect(this, SIGNAL(finishThread()), thread_Update, SLOT(terminate()));
    connect(mensagem, SIGNAL(cancelarProcesso()), thread_Update, SLOT(terminate()));
    ControleDAO *controle = new ControleDAO(nullptr);
    connect(thread_Update, SIGNAL(finished()), controle, SLOT(deleteLater()));
    connect(this, SIGNAL(obterUpdateDadosColaborador(int,int,int,int,bool)),
            controle, SLOT(obterUpdateDadosCadastroColaborador(int,int,int,int,bool)));
    connect(controle, SIGNAL(enviarUpdateDadosCadastroColaborador(QMap<int,UpdateDataTableColumm*>)),
            this, SLOT(updateDadosArquivoCrednossoGarantia(QMap<int,UpdateDataTableColumm*>)));
    controle->moveToThread(thread_Update);
    thread_Update->start(QThread::NormalPriority);

    int registros = (tabela->rowCount()) - 1;
    for (int linha = 0; linha < tabela->rowCount(); ++linha) {
        QTableWidgetItem *iemp = ui->tableWidget->item(linha, 2);
        QTableWidgetItem *icad = ui->tableWidget->item(linha, 4);
        int numemp = iemp->text().toInt(nullptr);
        int numcad = icad->text().toInt(nullptr);
        if(linha == registros) {
            emit obterUpdateDadosColaborador(numemp, numcad, linha, 0, true);
        } else {
            emit obterUpdateDadosColaborador(numemp, numcad, linha, 0, false);
        }
    }
}

void ProcessarArquivoCrednosso::updateDadosArquivoCrednossoGarantia(QMap<int, UpdateDataTableColumm *> __tempMap)
{
    UpdateDataTableColumm *i = __tempMap.value(0);
    if(i->getUltimoRegistro()) {
        emit finishThread();
        emit fecharCaixaDeMensagem();
        ui->tableWidget->setItem(i->getLinha(), 3, new QTableWidgetItem(i->getFilial()));
        ui->tableWidget->setItem(i->getLinha(), 5, new QTableWidgetItem(i->getNome()));
        ui->tableWidget->resizeColumnsToContents();
        QMessageBox::information(this, tr("Processamento de arquivo"), QString("Concluído!"), QMessageBox::Ok);
        ui->botaoExportar->setFocus();
    } else {
        ui->tableWidget->setItem(i->getLinha(), 3, new QTableWidgetItem(i->getFilial()));
        ui->tableWidget->setItem(i->getLinha(), 5, new QTableWidgetItem(i->getNome()));
    }

    i->deleteLater();
}

void ProcessarArquivoCrednosso::updateDadosArquivoCrednossoFeriasDAT(QMap<int, UpdateDataTableColumm *> __tempMap)
{
    UpdateDataTableColumm *i = __tempMap.value(0);
    if(i->getUltimoRegistro()) {
        emit finishThread();
        emit fecharCaixaDeMensagem();
        ui->tableWidget->setItem(i->getLinha(), 3, new QTableWidgetItem(i->getFilial()));
        ui->tableWidget->setItem(i->getLinha(), 5, new QTableWidgetItem(i->getNome()));
        ui->tableWidget->resizeColumnsToContents();
        QMessageBox::information(this, tr("Processamento de arquivo"), QString("Concluído!"), QMessageBox::Ok);
        ui->botaoExportar->setFocus();
    } else {
        ui->tableWidget->setItem(i->getLinha(), 3, new QTableWidgetItem(i->getFilial()));
        ui->tableWidget->setItem(i->getLinha(), 5, new QTableWidgetItem(i->getNome()));
    }

    i->deleteLater();
}

void ProcessarArquivoCrednosso::updateDadosArquivoCrednossoMesCorte(QMap<int, ObjetoCadastroUpdateFile *>)
{
    //Para quando for higienizacao do mes normal
}

void ProcessarArquivoCrednosso::updateDadosArquivoCrednossoDemitidos(QMap<int, ObjetoCadastroUpdateFile *>)
{
    //para quando for os demitidos do corte do mes de ferias
}
