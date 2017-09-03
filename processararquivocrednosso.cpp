#include "processararquivocrednosso.h"
#include "ui_processararquivocrednosso.h"

ProcessarArquivoCrednosso::ProcessarArquivoCrednosso(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProcessarArquivoCrednosso)
{
    ui->setupUi(this);
}

ProcessarArquivoCrednosso::~ProcessarArquivoCrednosso()
{
    delete ui;
}
