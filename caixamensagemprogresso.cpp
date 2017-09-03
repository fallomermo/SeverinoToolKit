#include "caixamensagemprogresso.h"
#include "ui_caixamensagemprogresso.h"

CaixaMensagemProgresso::CaixaMensagemProgresso(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CaixaMensagemProgresso)
{
    ui->setupUi(this);
}

CaixaMensagemProgresso::~CaixaMensagemProgresso()
{
    delete ui;
}
