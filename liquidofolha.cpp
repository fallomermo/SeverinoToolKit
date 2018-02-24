#include "liquidofolha.h"
#include "ui_liquidofolha.h"

LiquidoFolha::LiquidoFolha(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LiquidoFolha)
{
    ui->setupUi(this);
}

LiquidoFolha::~LiquidoFolha()
{
    delete ui;
}
