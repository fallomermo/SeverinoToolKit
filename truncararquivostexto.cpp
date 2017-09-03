#include "truncararquivostexto.h"
#include "ui_truncararquivostexto.h"

TruncarArquivosTexto::TruncarArquivosTexto(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TruncarArquivosTexto)
{
    ui->setupUi(this);
}

TruncarArquivosTexto::~TruncarArquivosTexto()
{
    delete ui;
}
