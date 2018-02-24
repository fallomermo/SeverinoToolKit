#include "conversordearquivo.h"
#include "ui_conversordearquivo.h"

ConversorDeArquivo::ConversorDeArquivo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConversorDeArquivo)
{
    ui->setupUi(this);

    processo = new QThread(Q_NULLPTR);
    controle = new ControleDAO(Q_NULLPTR);
    controle->moveToThread(processo);
    connect(this, SIGNAL(finishThread()), processo, SLOT(quit()), Qt::DirectConnection);
    connect(processo, SIGNAL(finished()), controle, SLOT(exitClass()));
    connect(ui->botaoEventosFichaFinanceira, SIGNAL(clicked(bool)), this, SLOT(analisarEventosImportados()));
    connect(controle, SIGNAL(retornaEventosImportados(QList<EventosImportadosOBJ*>)), this, SLOT(carregarEventosImportados(QList<EventosImportadosOBJ*>)));

    const QStringList tiposFolha = QStringList() << "Folha Normal" << "Folha de Dissídio" << "Folha de Adt. 13º Salário" << "Folha 13º Salário Integral";
    ui->tipoFolhaComboBox->addItems(tiposFolha);
    processo->start(QThread::NormalPriority);
}

ConversorDeArquivo::~ConversorDeArquivo()
{
    delete ui;
}

void ConversorDeArquivo::analisarEventosImportados()
{

    QString comando = QString("select numemp, codfil, numcad, nomfun, deseve, refeve, valeve, orieve from view_eventos_folha where numemp =%0 and ");
    emit obterEventosImportados(comando, 11);

}

void ConversorDeArquivo::carregarEventosImportados(QList<EventosImportadosOBJ *>)
{

}
