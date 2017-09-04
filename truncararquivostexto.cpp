#include "truncararquivostexto.h"
#include "ui_truncararquivostexto.h"

TruncarArquivosTexto::TruncarArquivosTexto(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TruncarArquivosTexto)
{
    ui->setupUi(this);
    QAction* pesquisar = ui->campoLocalDoArquivo->addAction(QIcon(":/images/open-folder.png"), QLineEdit::TrailingPosition);
    ui->campoLocalDoArquivo->setPlaceholderText(QString("Inserir arquivo aqui..."));
    connect(pesquisar, SIGNAL(triggered(bool)), this, SLOT(abrir()));
    connect(ui->botaoProcessar, SIGNAL(clicked(bool)), this, SLOT(processar()));
    connect(ui->botaoSalvar, SIGNAL(clicked(bool)), this, SLOT(salvar()));
}

TruncarArquivosTexto::~TruncarArquivosTexto()
{
    delete ui;
}

void TruncarArquivosTexto::abrir()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);

    QString local = dialog.getOpenFileName(this, tr("Abrir Arquivo"), QDir::currentPath(), QString("Arquivo Texto (*.txt);;Arquivo DAT(*.dat);;Todos os arquivos(*.*)"));
    if(!local.isEmpty())
        ui->campoLocalDoArquivo->setText(local);
    dialog.deleteLater();
    ui->botaoProcessar->setFocus();
}

void TruncarArquivosTexto::processar()
{
    QFile file(ui->campoLocalDoArquivo->text().trimmed());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    ui->plainTextEdit->clear();

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString linha = in.readLine();
        linha = linha.trimmed()+"\n";
        ui->plainTextEdit->insertPlainText(linha);
    }
}

void TruncarArquivosTexto::salvar()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);

    QString local = dialog.getSaveFileName(this, tr("Abrir Arquivo"), QDir::currentPath(), QString("Arquivo Texto (*.txt)"));
    if(local.isEmpty()) {
        dialog.deleteLater();
        return;
    }

    QFile file(local);
    if(file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        QString texto = ui->plainTextEdit->toPlainText();
        stream << texto.toLatin1();
        stream.flush();
    }
    file.close();
}
