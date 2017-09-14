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

    threadInstancia = new QThread(Q_NULLPTR);
    controlador = new ControleDAO(Q_NULLPTR);

    connect(this, SIGNAL(cancelarProcesso()), threadInstancia, SLOT(quit()), Qt::DirectConnection);
    connect(threadInstancia, SIGNAL(finished()), controlador, SLOT(exitClass()));
    connect(this, SIGNAL(obterUpdateDadosColaborador(UpdateDataTableColumm*)), controlador, SLOT(obterUpdateDadosCadastroColaborador(UpdateDataTableColumm*)));
    connect(controlador, SIGNAL(enviarUpdateDadosCadastroColaborador(QMap<int,UpdateDataTableColumm*>)), this, SLOT(updateDadosArquivoLayoutBasico(QMap<int,UpdateDataTableColumm*>)));
}

ProcessarArquivoCrednosso::~ProcessarArquivoCrednosso()
{
    delete ui;
}

void ProcessarArquivoCrednosso::abrirArquivo()
{
    QFileDialog d;
    d.setFileMode(QFileDialog::AnyFile);
    QString local = d.getOpenFileName(this, tr("Abrir"), QDir::currentPath(), QString("Arquivo Texto (*.txt);;Arquivo DAT(*.dat);;Todos os arquivos(*.*)"));
    QFileInfo f(local);
    if(!local.isEmpty()) {
        ui->campoLocalDoArquivo->setText(local);
        QMessageBox q;
        int ok = q.question(this, QString("Arquivo: %0").arg(f.fileName()), QString("Deseja carregar o arquivo %0?").arg(f.fileName()), QMessageBox::Yes, QMessageBox::No);
        if(ok == QMessageBox::Yes) { this->converterArquivo(); ui->botaoAtualizar->setFocus();
        } else { ui->botaoConverter->setFocus();}
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
    case 2: this->converterArquivoLayoutBasico();
        break;
    case 3: this->converterArquivoCorteFeriasDAT();
        break;
    case 4: this->converterArquivoCorteFeriasDAT();
        break;
    case 5: this->converterArquivoCorteFeriasDAT();
        break;
    case 6: this->converterArquivoLayoutBasico();
        break;
    default:
        break;
    }
}

void ProcessarArquivoCrednosso::converterArquivo(QString c)
{
    qDebug() << c;
}

void ProcessarArquivoCrednosso::converterArquivoLayoutBasico()
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
                    QTableWidgetItem *item = new QTableWidgetItem(QString("%L1").arg(dValue, 2, 'f', 2));
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
                    QTableWidgetItem *item = new QTableWidgetItem(QString("%L1").arg(dValue, 2, 'f', 2));
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
    ui->botaoAtualizar->setFocus();
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
    while (!in.atEnd()) {
        linha++;
        emit minimumProgressValue(linha);
        emit progressValue(linha);
        qApp->processEvents();

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
    connect(mensagem, SIGNAL(cancelarProcesso()), this, SLOT(cancelarOperacao()));
    connect(this, SIGNAL(cancelarOperacao()), thread_Update, SLOT(terminate()));
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
                    QTableWidgetItem *item = new QTableWidgetItem(QString("%L1").arg(dValue, 0, 'f', 2));
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
                    QTableWidgetItem *item = new QTableWidgetItem(QString("%L1").arg(dValue, 0, 'f', 2));
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

void ProcessarArquivoCrednosso::cancelarOperacao()
{
    if(QMessageBox::question(this, tr("Cancelar Operação?"), QString("Deseja realmente cancelar?"), QMessageBox::No, QMessageBox::Yes) == QMessageBox::Yes) {
        emit cancelarProcesso();
        if(threadInstancia->isRunning())
            this->thread()->quit();
    }
}

void ProcessarArquivoCrednosso::atualizarDados()
{
    int control = ui->comboBox->currentIndex();
    switch (control) {
    case 0: break;
    case 1: break;
    case 2: this->updateDadosArquivoLayoutBasico(); break;
    case 3: this->atualizarDadosCorteFeriasDATSenior(); break;
    case 4: this->atualizarDadosCorteFeriasDATSenior(); break;
    case 5: this->atualizarDadosCorteFeriasDATSenior(); break;
    case 6: this->updateDadosArquivoLayoutBasico(); break;
    default:this->atualizarDadosCorteFeriasDATSenior(); break;
    }
}

void ProcessarArquivoCrednosso::limparDadosTempTable()
{
    __tempTable->setRowCount(1);
}

void ProcessarArquivoCrednosso::updateDadosArquivoLayoutBasico()
{
    msg = new CaixaMensagemProgresso(this);
    connect(this, SIGNAL(progressValue(int)), msg, SLOT(setProgressValue(int)));
    connect(this, SIGNAL(minimumProgressValue(int)), msg, SLOT(setMinimumValue(int)));
    connect(this, SIGNAL(maximumProgressValue(int)), msg, SLOT(setMaximumValue(int)));
    connect(this, SIGNAL(fecharCaixaDeMensagem()), msg, SLOT(fecharJanela()));
    connect(msg, SIGNAL(cancelarProcesso()), this, SLOT(cancelarOperacao()));
    msg->setWindowFlag(Qt::Window);
    msg->setWindowFlag(Qt::FramelessWindowHint);
    msg->setWindowModality(Qt::ApplicationModal);
    msg->setMinimumValue(0);
    msg->setVisible(true);
    msg->show();
    qApp->processEvents(QEventLoop::AllEvents);

    QTableWidget *tabela = ui->tableWidget;

    qRegisterMetaType<QMap<int,UpdateDataTableColumm*>>("__mapUpdtDatTabCol");
    qRegisterMetaType<QMap<int,UpdateDataTableColumm*>>("__objUpdtDatTabCol");
    threadInstancia = new QThread(nullptr);
    controlador = new ControleDAO(nullptr);

    controlador->moveToThread(threadInstancia);
    threadInstancia->start(QThread::NormalPriority);

    emit maximumProgressValue(tabela->rowCount());
    for (int linha = 0; linha < tabela->rowCount(); ++linha) {
        UpdateDataTableColumm *parametros = new UpdateDataTableColumm(nullptr);
        parametros->setLinha(linha);
        parametros->setCodigoEmpresa(ui->tableWidget->item(linha, 2)->text().toInt(nullptr));
        parametros->setMatricula(ui->tableWidget->item(linha, 4)->text().toInt(nullptr));

        if(linha == (tabela->rowCount()-1)) {
            parametros->setUltimoRegistro(true);
            emit obterUpdateDadosColaborador(parametros);
        } else {
            parametros->setUltimoRegistro(false);
            emit obterUpdateDadosColaborador(parametros);
        }
    }
}

void ProcessarArquivoCrednosso::updateDadosArquivoLayoutBasico(QMap<int, UpdateDataTableColumm *> __tempMap)
{
    UpdateDataTableColumm *i = __tempMap.value(0);
    qDebug() << QString("Linha: %0 - %1 [ %2 ]").arg(i->getLinha()+1).arg(i->getNome()).arg(i->getUltimoRegistro());

    if(i->getUltimoRegistro()) {
        emit finishThread();
        emit cancelarProcesso();
        emit progressValue(i->getLinha()+1);
        ui->tableWidget->setItem(i->getLinha(), 3, new QTableWidgetItem(i->getFilial()));
        ui->tableWidget->setItem(i->getLinha(), 5, new QTableWidgetItem(i->getNome()));
        ui->tableWidget->resizeColumnsToContents();
        emit fecharCaixaDeMensagem();
        qDebug() << QString("Linha: %0 - Concluido!").arg(i->getLinha()+1);
        ui->botaoExportar->setFocus();
    } else {
        emit progressValue(i->getLinha()+1);
        ui->tableWidget->setItem(i->getLinha(), 3, new QTableWidgetItem(i->getFilial()));
        ui->tableWidget->setItem(i->getLinha(), 5, new QTableWidgetItem(i->getNome()));
    }
}

void ProcessarArquivoCrednosso::updateDadosArquivoCrednossoFerias(QMap<int, ObjetoCadastroUpdateFile *>)
{
    //Para quando for o de férias
}

void ProcessarArquivoCrednosso::updateDadosArquivoCrednossoRetorno(QMap<int, ObjetoCadastroUpdateFile *>)
{
    //Para quando for retorno da folha
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
