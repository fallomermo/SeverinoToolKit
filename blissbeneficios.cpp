#include "blissbeneficios.h"
#include "ui_blissbeneficios.h"

BlissBeneficios::BlissBeneficios(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlissBeneficios)
{
    ui->setupUi(this);
    QAction* pesquisar = ui->campoLocalDoArquivo->addAction(QIcon(":/images/open-folder.png"), QLineEdit::TrailingPosition);
    connect(pesquisar, SIGNAL(triggered(bool)), this, SLOT(abrirArquivo()));
    connect(ui->botaoProcessar, SIGNAL(clicked(bool)), this, SLOT(processarArquivoSelecionado()));
    connect(ui->botaoExportar, SIGNAL(clicked(bool)), this, SLOT(exportarArquivo()));
}

BlissBeneficios::~BlissBeneficios()
{
    delete ui;
}

void BlissBeneficios::abrirArquivo()
{
    QString local = QFileDialog::getOpenFileName(this, tr("Abrir arquivo"), QDir::homePath(), QString("Texto(*.txt)"));
    ui->campoLocalDoArquivo->setText(local);
    ui->botaoProcessar->setFocus();
}

void BlissBeneficios::processarArquivoSelecionado()
{
    if(ui->campoLocalDoArquivo->text().isEmpty())
        QMessageBox::critical(this, tr("Erro!"), QString("Nenhum arquivo informado!"),QMessageBox::Ok);

    QFile file(ui->campoLocalDoArquivo->text().trimmed());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    QTextStream in(&file);

    int linha = 0;
    while (!in.atEnd()) {
        linha++;
        QString registro = in.readLine();
        QStringList lista = registro.split(";");
        if(linha == 1) {
            QStringList labels;
            ui->tableWidget->setRowCount(linha);
            for (int coluna = 0; coluna < lista.count(); ++coluna) {
                labels << QString("Coluna %0").arg(coluna);
            }

            ui->tableWidget->setColumnCount(labels.count());
            ui->tableWidget->setHorizontalHeaderLabels(labels);

            if(ui->campoImportacaoRubi->isChecked()) {
                for (int coluna = 0; coluna < lista.count(); ++coluna) {
                    ui->tableWidget->setRowCount(linha);
                    ui->tableWidget->setItem(linha-1, coluna, new QTableWidgetItem(lista.at(coluna)));
                }
            } else {

            }
        } else {
            ui->tableWidget->setRowCount(linha);
            for (int coluna = 0; coluna < ui->tableWidget->columnCount(); ++coluna) {
                ui->tableWidget->setItem(linha-1, coluna, new QTableWidgetItem(lista.at(coluna)));
            }
        }
    }
    file.close();
    ui->tableWidget->resizeColumnsToContents();
    QMessageBox::information(this, tr("Processamento de arquivo"), QString("Concluído!"), QMessageBox::Ok);
    ui->campoLocalDoArquivo->setFocus();
}

void BlissBeneficios::exportarArquivo()
{
    if(ui->campoImportacaoRubi->isChecked())
        this->processarBlissSeniorTexto();
    else
        this->processarBlissEmpresaVTTexto();
}

void BlissBeneficios::processarBlissSeniorTexto()
{
    if(ui->tableWidget->rowCount() <= 0) {
        return;
    } else {
        QFileDialog dialogo;
        dialogo.setDirectory(QDir::homePath());
        QString filename = dialogo.getSaveFileName(this, tr("Exportar arquivo"), QString("ImportacaoArquivoBlissSenior-Rubi"), "Texto (*.txt)");
        if(filename.isEmpty())
            return;

        QFile f( filename );
        if (f.open(QFile::WriteOnly | QFile::Truncate)) {
            QTextStream data( &f );
            QStringList strList;
            for( int r = 0; r < ui->tableWidget->rowCount(); ++r ) {
                strList.clear();
                for( int c = 0; c < ui->tableWidget->columnCount(); ++c ) {
                    QString linha = ui->tableWidget->item( r, c )->text().trimmed();
                    linha.remove(',');
                    linha.remove('.');
                    linha.remove(' ');
                    int i = linha.toInt(nullptr);
                    linha = QString::number(i).rightJustified(11,'0');
                    strList << linha;
                }
                foreach (QString linha, strList) {
                    data << linha;
                }
                data << "\n";
            }
            f.close();
            QMessageBox::information(this, tr("Exportação concluída!"), QString("Arquivo exportado com sucesso!"), QMessageBox::Ok);
        }
    }
}

void BlissBeneficios::processarBlissEmpresaVTTexto()
{
    if(ui->tableWidget->rowCount() <= 0) {
        return;
    } else {
        QFileDialog dialogo;
        dialogo.setDirectory(QDir::homePath());
        QString filename = dialogo.getSaveFileName(this, tr("Exportar arquivo"), QString("ImportacaoArquivoBlissSenior-Rubi"), "Texto (*.txt)");
        if(filename.isEmpty())
            return;

        QFile f( filename );
        if (f.open(QFile::WriteOnly | QFile::Truncate)) {
            QTextStream data( &f );
            QStringList strList;
            QLocale local;
            local.setDefault(this->locale());
            for( int r = 0; r < ui->tableWidget->rowCount(); ++r ) {
                strList.clear();
                for( int c = 0; c < ui->tableWidget->columnCount(); ++c ) {
                    QString linha = ui->tableWidget->item( r, c )->text().trimmed();
                    strList << linha;
                }
                foreach (QString linha, strList) {
                    data << linha;
                }
                data << "\n";
            }
            f.close();
            QMessageBox::information(this, tr("Exportação concluída!"), QString("Arquivo exportado com sucesso!"), QMessageBox::Ok);
        }
    }
}
