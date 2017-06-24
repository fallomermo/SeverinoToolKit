#include "pesquisar.h"
#include "ui_pesquisar.h"

Pesquisar::Pesquisar(QWidget *parent) : QWidget(parent), ui(new Ui::Pesquisar)
{
    ui->setupUi(this);
}

Pesquisar::Pesquisar(QWidget *parent, QMap<int, CadastroEmpresa *> ce, QMap<int, CadastroFilial *> cf, QString es, int fp):
    QWidget(parent),
    ui(new Ui::Pesquisar),
    mapEmpresas(ce),
    mapFiliais(cf),
    empresaSelecionada(es),
    flagTipoPesquisa(fp)
{
    ui->setupUi(this);

    if(cf.isEmpty()) {
        QMessageBox::critical(this, tr("Cadatro de Empresa e Filial"), QString("Nenhuma informação encontrada!"), QMessageBox::Ok);
        close();
    }

    if(getFlagTipoPesquisa() == 1) {
        connect(ui->tableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(setEmpresa(int,int)));
        connect(ui->tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(setEmpresa(int,int)));
    } else {
        connect(ui->tableWidget, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(setFilial(int,int)));
        connect(ui->tableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(setFilial(int,int)));
    }

    connect(ui->botaoCancelar,SIGNAL(clicked(bool)), this, SLOT(close()));
    preencherTabela();
}

Pesquisar::~Pesquisar()
{
    delete ui;
}

QMap<int, CadastroEmpresa *> Pesquisar::getMapEmpresas() const
{
    return mapEmpresas;
}

QMap<int, CadastroFilial *> Pesquisar::getMapFiliais() const
{
    return mapFiliais;
}

void Pesquisar::preencherTabela()
{
    QStringList labels = QStringList() << "ID" << "Descrição";
    ui->tableWidget->setColumnCount(labels.count());
    ui->tableWidget->setHorizontalHeaderLabels(labels);

    if(getFlagTipoPesquisa() == 1) {
        ui->tableWidget->clearContents();
        QMap<int, CadastroEmpresa*> listMap = getMapEmpresas();
        int linha = 0;
        ui->tableWidget->setRowCount(listMap.count());
        QMapIterator<int, CadastroEmpresa*> map(listMap);
        while (map.hasNext()) {
            map.next();
            CadastroEmpresa *c = new CadastroEmpresa(this);
            c = map.value();
            for(int coluna = 0; coluna < 2; coluna++){
                if(coluna == 0)
                    ui->tableWidget->setItem(linha, coluna, new QTableWidgetItem(c->getID_Empresa()));
                else
                    ui->tableWidget->setItem(linha, coluna, new QTableWidgetItem(c->getEmpresa()));
            }
            linha++;
        }
    } else {
        ui->tableWidget->clearContents();
        QMap<int, CadastroFilial*> listMap = getMapFiliais();
        QMap<QString, QString> _n;
        QMapIterator<int, CadastroFilial*> map(listMap);
        while (map.hasNext()) {
            map.next();
            CadastroFilial *c = new CadastroFilial(this);
            c = map.value();
            if(c->getID_Empresa() == getEmpresaSelecionada()) {
                _n.insert(c->getID_Filial(), c->getFilial());
            }
        }

        ui->tableWidget->setRowCount(_n.count());
        QMapIterator<QString, QString> _mi(_n);
        int linha = 0;
        while (_mi.hasNext()) {
            _mi.next();
            for(int coluna = 0; coluna < 2; coluna++) {
                if(coluna == 0)
                    ui->tableWidget->setItem(linha,coluna,new QTableWidgetItem(_mi.key()));
                else
                    ui->tableWidget->setItem(linha,coluna,new QTableWidgetItem(_mi.value()));
            }
            linha++;
        }
    }
    ui->tableWidget->resizeColumnsToContents();
}

void Pesquisar::setData(int r, int c)
{
    c = 0;
    QTableWidgetItem *item = ui->tableWidget->item(r,c);
    emit getData(item->text());
    this->close();
}

void Pesquisar::setData()
{
    QTableWidgetItem *item = ui->tableWidget->item(ui->tableWidget->currentRow(), 0);
    emit getData(item->text());
    this->close();
}

void Pesquisar::setEmpresa(int r, int c)
{
    c = 0;
    QTableWidgetItem *item = ui->tableWidget->item(r,c);
    emit getEmpresa(item->text());
    this->close();
}

void Pesquisar::setFilial(int r, int c)
{
    c = 0;
    QTableWidgetItem *item = ui->tableWidget->item(r,c);
    emit getFilial(item->text());
    this->close();
}

int Pesquisar::getFlagTipoPesquisa() const
{
    return flagTipoPesquisa;
}

QString Pesquisar::getEmpresaSelecionada() const
{
    return empresaSelecionada;
}
