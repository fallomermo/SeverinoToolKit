#include "ecoclinic.h"
#include "ui_ecoclinic.h"

Ecoclinic::Ecoclinic(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ecoclinic)
{
    ui->setupUi(this);

    threadProcesso = new QThread(Q_NULLPTR);
    controle = new ControleDAO(Q_NULLPTR);

    connect(ui->botaoProcessar, SIGNAL(clicked(bool)), this, SLOT(processarRequisicao()));

    connect(this, SIGNAL(finishThread()), threadProcesso, SLOT(quit()), Qt::DirectConnection);
    connect(threadProcesso, SIGNAL(finished()), controle, SLOT(exitClass()));
    connect(this, SIGNAL(obterPlanoSaude(QString,QString,int)), controle, SLOT(obterPlanoSaude(QString,QString,int)));
    connect(controle, SIGNAL(enviarPlanoSaude(QMap<int,EcoclinicRepasses*>)), this, SLOT(processarRepasses(QMap<int,EcoclinicRepasses*>)));

    QStringList labels = QStringList() << "ID Empresa"
                                       << "Empresa"
                                       << "ID Filial"
                                       << "Filial"
                                       << "Matricula"
                                       << "CPF"
                                       << "Nome"
                                       << "Cargo"
                                       << "ID Sindicato"
                                       << "Setor"
                                       << "Sexo"
                                       << "DDD 1"
                                       << "Numero 1"
                                       << "DDD 2"
                                       << "Numero 2"
                                       << "Registro Identidade"
                                       << "Orgão Emissor"
                                       << "UF Emissor"
                                       << "Data Emissao"
                                       << "Data Admissao"
                                       << "Competencia"
                                       << "ID Evento"
                                       << "Evento"
                                       << "Valor";
    ui->tableWidget->setColumnCount(labels.count());
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    ui->tableWidget->resizeColumnsToContents();
}

Ecoclinic::~Ecoclinic()
{
    delete ui;
}

void Ecoclinic::processarRequisicao()
{
    ui->tableWidget->clearContents();
    CaixaMensagemProgresso *mensagem = new CaixaMensagemProgresso(this);
    mensagem->setWindowFlag(Qt::Window);
    mensagem->setWindowFlag(Qt::FramelessWindowHint);
    mensagem->setWindowModality(Qt::ApplicationModal);
    connect(mensagem, SIGNAL(cancelarProcesso()), this, SLOT(cancelarOperacao()));
    connect(this, SIGNAL(setMinimumValue(int)), mensagem, SLOT(setMinimumValue(int)));
    connect(this, SIGNAL(setMaximumValue(int)), mensagem, SLOT(setMaximumValue(int)));
    connect(this, SIGNAL(setProgressValue(int)), mensagem, SLOT(setProgressValue(int)));
    mensagem->setVisible(true);
    mensagem->show();
    qApp->processEvents();

    if(ui->campoRepasseEmpresa->isChecked())
        emit obterPlanoSaude(ui->campoCompetencia->date().toString(Qt::ISODate), ui->campoCompetencia->date().toString(Qt::ISODate),0);
    else
        emit obterPlanoSaude(ui->campoCompetencia->date().toString(Qt::ISODate), ui->campoCompetencia->date().toString(Qt::ISODate),1);

}

void Ecoclinic::processarRepasses(QMap<int, EcoclinicRepasses *> __tempMap)
{
    emit setMaximumValue(__tempMap.count());
    QMapIterator<int, EcoclinicRepasses*> __mapIterator(__tempMap);
    if(__tempMap.isEmpty()) {
        emit fecharCaixaDeMensagem();
        emit finishThread();
        ui->botaoProcessar->setFocus();
        return;
    } else {
        ui->tableWidget->setRowCount(__tempMap.count());
        int linha = 0;
        while (__mapIterator.hasNext()) {
            __mapIterator.next();
            emit setProgressValue(linha);
            EcoclinicRepasses *repasses = new EcoclinicRepasses(Q_NULLPTR);
            repasses = __mapIterator.value();
            this->inserirLinhaTabela(linha, ui->tableWidget->columnCount(), repasses);
            linha++;
        }
        ui->tableWidget->resizeColumnsToContents();
        emit setProgressValue(linha+1);
        emit fecharCaixaDeMensagem();
        ui->botaoExportar->setFocus();
    }
}

void Ecoclinic::cancelarOperacao()
{
    emit cancelarProcesso();
}

void Ecoclinic::inserirItemTabela(int r, int c, QString sValue)
{
    ui->tableWidget->setItem(r,c,new QTableWidgetItem(sValue));
}

void Ecoclinic::inserirItemTabela(int r, int c, QDate v)
{
    ui->tableWidget->setItem(r,c,new QTableWidgetItem(QDate(v).toString("dd/MM/yyyy"),Qt::AlignCenter));
}

void Ecoclinic::inserirItemTabela(int r, int c, double dValue)
{
    QTableWidgetItem *item = new QTableWidgetItem(QString("%L1").arg(dValue, 0, 'f', 4));
    item->setTextAlignment(Qt::AlignRight);
    ui->tableWidget->setItem(r,c,item);
}

void Ecoclinic::inserirItemTabela(int r, int c, int iValue)
{
    QTableWidgetItem *item = new QTableWidgetItem(QString::number(iValue));
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(r,c,item);
}

void Ecoclinic::inserirLinhaTabela(int linha, int nrColunas, EcoclinicRepasses *evento)
{
    for (int coluna = 0; coluna < nrColunas; ++coluna) {
        if(coluna == 0)
            inserirItemTabela(linha, coluna, evento->getID_Empresa());
        if(coluna == 1)
            inserirItemTabela(linha, coluna, evento->getEmpresa());
        if(coluna == 2)
            inserirItemTabela(linha, coluna, evento->getID_Filial());
        if(coluna == 3)
            inserirItemTabela(linha, coluna, evento->getFilial());
        if(coluna == 4)
            inserirItemTabela(linha, coluna, evento->getMatricula());
        if(coluna == 5)
            inserirItemTabela(linha, coluna, evento->getCPF());
        if(coluna == 6)
            inserirItemTabela(linha, coluna, evento->getNome());
        if(coluna == 7)
            inserirItemTabela(linha, coluna, evento->getID_Sindicato());
        if(coluna == 8)
            inserirItemTabela(linha, coluna, evento->getSetor());
        if(coluna == 9)
            inserirItemTabela(linha, coluna, evento->getSexo());
        if(coluna == 10) {
            QString numero1 = QString("%0-%1").arg(evento->getDdd1()).arg(evento->getNumero1());
            inserirItemTabela(linha, coluna, numero1);
        }
        if(coluna == 11) {
            QString numero1 = QString("%0-%1").arg(evento->getDdd2()).arg(evento->getNumero2());
            inserirItemTabela(linha, coluna, numero1);
        }
        if(coluna == 12)
            inserirItemTabela(linha, coluna, evento->getRegistroGeralIdentidade());
        if(coluna == 13)
            inserirItemTabela(linha, coluna, evento->getOrgaoEmissor());
        if(coluna == 14)
            inserirItemTabela(linha, coluna, evento->getUF_Emissor());
        if(coluna == 15)
            inserirItemTabela(linha, coluna, evento->getDataEmissao());
        if(coluna == 16)
            inserirItemTabela(linha, coluna, evento->getDataAdmissao());
        if(coluna == 17)
            inserirItemTabela(linha, coluna, evento->getCompetencia());
        if(coluna == 18)
            inserirItemTabela(linha, coluna, evento->getID_Evento());
        if(coluna == 19)
            inserirItemTabela(linha, coluna, evento->getEvento());
        if(coluna == 20)
            inserirItemTabela(linha, coluna, evento->getValor());
    }
}

void Ecoclinic::exportarParaExcel()
{

}
