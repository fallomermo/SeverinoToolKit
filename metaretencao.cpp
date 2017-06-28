#include "metaretencao.h"
#include "ui_metaretencao.h"

MetaRetencao::MetaRetencao(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MetaRetencao)
{
    ui->setupUi(this);
}

MetaRetencao::~MetaRetencao()
{
    delete ui;
}
