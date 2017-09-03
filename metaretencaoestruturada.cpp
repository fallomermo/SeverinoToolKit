#include "metaretencaoestruturada.h"
#include "ui_metaretencaoestruturada.h"

MetaRetencaoEstruturada::MetaRetencaoEstruturada(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MetaRetencaoEstruturada)
{
    ui->setupUi(this);
}

MetaRetencaoEstruturada::~MetaRetencaoEstruturada()
{
    delete ui;
}
