#include "planosaude.h"
#include "ui_planosaude.h"
#include "caixamensagemprogresso.h"

PlanoSaude::PlanoSaude(QWidget *parent) : QWidget(parent), ui(new Ui::PlanoSaude)
{
    ui->setupUi(this);
    QStringList operacoes = QStringList() << "Repasse Empresa" << "Repasse Titular" << "Débitos Cadastro Completo";
    ui->comboBox->addItems(operacoes);

    ui->periodoInicial->setDate(QDateTime::currentDateTime().date());
    ui->periodoFinal->setDate(QDateTime::currentDateTime().date());

    connect(ui->botaoProcessar, SIGNAL(clicked(bool)), this, SLOT(getDatatable()));
    connect(ui->botaoExportar, SIGNAL(clicked(bool)), this, SLOT(exportarParaExcel()));

    ui->campoPesquisarObjetosTabela->addAction(QIcon(":/images/filter.png"), QLineEdit::TrailingPosition);
    ui->campoPesquisarObjetosTabela->setPlaceholderText(QString("Pesquisar Itens na Tabela"));
    connect(ui->campoPesquisarObjetosTabela, SIGNAL(textChanged(QString)), this, SLOT(filtroItemTabela(QString)));

    QStringList headerLabels = QStringList() << "CODEMPRESA"
                                             << "EMPRESA"
                                             << "CODFIL"
                                             << "FILIAL"
                                             << "MATRICULA"
                                             << "CPF"
                                             << "NOME"
                                             << "CARGO"
                                             << "SINDICATO"
                                             << "SETOR"
                                             << "SEXO"
                                             << "DDD1"
                                             << "NUMERO1"
                                             << "DDD2"
                                             << "NUMERO2"
                                             << "RG"
                                             << "ORGAOEMISSOR"
                                             << "UFEMISSOR"
                                             << "DATAEMISSAO"
                                             << "ADMISSAO"
                                             << "COMPETENCIA"
                                             << "CODEVENTO"
                                             << "EVENTO"
                                             << "VALOR";
    ui->tableWidget->setColumnCount(headerLabels.count());
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget->resizeColumnsToContents();

}

PlanoSaude::~PlanoSaude()
{
    delete ui;
}

void PlanoSaude::getDatatable()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    caixaDeMensagem = new CaixaMensagemProgresso(this);
    connect(this, SIGNAL(minimumProgressValue(int)), caixaDeMensagem, SLOT(setMinimumValue(int)));
    connect(this, SIGNAL(maximumProgressValue(int)), caixaDeMensagem, SLOT(setMaximumValue(int)));
    connect(this, SIGNAL(progressValue(int)), caixaDeMensagem, SLOT(setProgressValue(int)));
    connect(this, SIGNAL(fecharMensagemProgresso()), caixaDeMensagem, SLOT(fecharJanela()));
    caixaDeMensagem->setWindowTitle(QString("Trabalhando em sua requisição..."));
    caixaDeMensagem->setWindowFlag(Qt::Window);
    caixaDeMensagem->setWindowFlag(Qt::FramelessWindowHint);
    caixaDeMensagem->setWindowModality(Qt::ApplicationModal);
    caixaDeMensagem->show();
    qApp->processEvents();;

    QThread *threadDAO = new QThread(nullptr);
    controle = new ControleDAO(nullptr);
    qRegisterMetaType<QMap<int,EcoclinicRepasses*>>("__metaDataObjectPS");
    connect(this, SIGNAL(obterPlanoSaude(QString,QString,int)), controle, SLOT(obterPlanoSaude(QString,QString,int)));
    connect(controle, SIGNAL(enviarPlanoSaude(QMap<int,EcoclinicRepasses*>)), this, SLOT(setPlanoSaude(QMap<int,EcoclinicRepasses*>)));
    connect(this, SIGNAL(finishThread()), threadDAO, SLOT(terminate()));
    connect(threadDAO, SIGNAL(finished()), controle, SLOT(deleteLater()));
    controle->moveToThread(threadDAO);
    threadDAO->start(QThread::NormalPriority);

    QDate _tempDateIni = ui->periodoInicial->date();
    QDate _tempDateFim = ui->periodoFinal->date();
    int _anoComIni = _tempDateIni.year();
    int _mesComIni = _tempDateIni.month();
    int _diaComIni = 1;
    QDate __dataIni( _anoComIni, _mesComIni, _diaComIni );

    int _anoComFim = _tempDateFim.year();
    int _mesComFim = _tempDateFim.month();
    int _diaComFim = 1;
    QDate __dataFim( _anoComFim, _mesComFim, _diaComFim );

    int __indiceCBX = ui->comboBox->currentIndex();
    emit obterPlanoSaude(__dataIni.toString(Qt::ISODate),
                         __dataFim.toString(Qt::ISODate),
                         __indiceCBX);
}

void PlanoSaude::exportarParaExcel()
{
    QString __nomeArquivo = "/"+ui->comboBox->currentText()
            +"_"+ui->periodoInicial->text().replace('/','-')
            +"_"+ui->periodoFinal->text().replace('/','-');
    ExportarArquivo *exp = new ExportarArquivo(this, ui->tableWidget);
    connect(exp, SIGNAL(mensagemRetorno(QString)), this, SLOT(caixaMensagemUsuario(QString)));
    exp->exportar(__nomeArquivo,0);
}

void PlanoSaude::inserirLinhaTabela(int linha, int nrColunas, EcoclinicRepasses * repasses)
{
    for (int coluna = 0; coluna < nrColunas; ++coluna) {
        if(coluna == 0)
            inserirItemTabela(linha, coluna, repasses->getID_Empresa());
        if(coluna == 1)
            inserirItemTabela(linha, coluna, repasses->getEmpresa());
        if(coluna == 2)
            inserirItemTabela(linha, coluna, repasses->getID_Filial());
        if(coluna == 3)
            inserirItemTabela(linha, coluna, repasses->getFilial());
        if(coluna == 4)
            inserirItemTabela(linha, coluna, repasses->getMatricula());
        if(coluna == 5)
            inserirItemTabela(linha, coluna, repasses->getCPF());
        if(coluna == 6)
            inserirItemTabela(linha, coluna, repasses->getNome());
        if(coluna == 7)
            inserirItemTabela(linha, coluna, repasses->getCargo());
        if(coluna == 8)
            inserirItemTabela(linha, coluna, repasses->getID_Sindicato());
        if(coluna == 9)
            inserirItemTabela(linha, coluna, repasses->getSetor());
        if(coluna == 10)
            inserirItemTabela(linha, coluna, repasses->getSexo());
        if(coluna == 11)
            inserirItemTabela(linha, coluna, repasses->getDdd1());
        if(coluna == 12)
            inserirItemTabela(linha, coluna, repasses->getNumero1());
        if(coluna == 13)
            inserirItemTabela(linha, coluna, repasses->getDdd2());
        if(coluna == 14)
            inserirItemTabela(linha, coluna, repasses->getNumero2());
        if(coluna == 15)
            inserirItemTabela(linha, coluna, repasses->getRegistroGeralIdentidade());
        if(coluna == 16)
            inserirItemTabela(linha, coluna, repasses->getOrgaoEmissor());
        if(coluna == 17)
            inserirItemTabela(linha, coluna, repasses->getUF_Emissor());
        if(coluna == 18)
            inserirItemTabela(linha, coluna, repasses->getDataEmissao());
        if(coluna == 19)
            inserirItemTabela(linha, coluna, repasses->getDataAdmissao());
        if(coluna == 20)
            inserirItemTabela(linha, coluna, repasses->getCompetencia());
        if(coluna == 21)
            inserirItemTabela(linha, coluna, repasses->getID_Evento());
        if(coluna == 22)
            inserirItemTabela(linha, coluna, repasses->getEvento());
        if(coluna == 23)
            inserirItemTabela(linha, coluna, repasses->getValor());
    }
}

void PlanoSaude::inserirItemTabela(int r, int c, int iValue)
{
    QTableWidgetItem *item = new QTableWidgetItem(QString::number(iValue));
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(r,c,item);
}

void PlanoSaude::inserirItemTabela(int r , int c, double dValue)
{
    QTableWidgetItem *item = new QTableWidgetItem(QString("%L1").arg(dValue, 0, 'f', 4));
    item->setTextAlignment(Qt::AlignRight);
    ui->tableWidget->setItem(r,c,item);
}

void PlanoSaude::inserirItemTabela(int r, int c, QString sValue)
{
    ui->tableWidget->setItem(r,c,new QTableWidgetItem(sValue));
}

void PlanoSaude::inserirItemTabela(int r, int c, QDate dtValue)
{
    ui->tableWidget->setItem(r,c,new QTableWidgetItem(QDate(dtValue).toString("dd/MM/yyyy")));
}

void PlanoSaude::filtroItemTabela(QString filter)
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

void PlanoSaude::caixaMensagemUsuario(QString msg)
{
    QMessageBox::information(this, tr("Exportação de Dados"), QString(msg), QMessageBox::Ok);
}

void PlanoSaude::setPlanoSaude(QMap<int, EcoclinicRepasses *> __tempMap)
{
    emit finishThread();
    this->setMapPlanoSaude(__tempMap);
    this->preencherTabela(__tempMap);
}

void PlanoSaude::preencherTabela(QMap<int, EcoclinicRepasses *> __tempMap)
{
    emit minimumProgressValue(0);
    emit maximumProgressValue(__tempMap.count());
    QMapIterator<int, EcoclinicRepasses*> __mapIterator(__tempMap);
    if(__tempMap.isEmpty()) {
        emit fecharMensagemProgresso();
        return;
    } else {
        ui->tableWidget->setRowCount(__tempMap.count());
        int linha = 0;
        while (__mapIterator.hasNext()) {
            __mapIterator.next();
            emit progressValue(linha);
            EcoclinicRepasses *repasses = new EcoclinicRepasses;
            repasses = __mapIterator.value();
            for (int coluna = 0; coluna < ui->tableWidget->columnCount(); ++coluna) {
                inserirLinhaTabela(linha, coluna, repasses);
            }
            linha++;
        }
        ui->tableWidget->resizeColumnsToContents();
        emit fecharMensagemProgresso();
    }
}

QMap<int, EcoclinicRepasses *> PlanoSaude::getMapPlanoSaude() const
{
    return mapPlanoSaude;
}

void PlanoSaude::setMapPlanoSaude(const QMap<int, EcoclinicRepasses *> &value)
{
    mapPlanoSaude = value;
}
