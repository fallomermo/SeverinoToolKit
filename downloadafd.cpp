#include "downloadafd.h"
#include "ui_downloadafd.h"

DownloadAFD::DownloadAFD(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadAFD)
{
    ui->setupUi(this);
}

DownloadAFD::~DownloadAFD()
{
    delete ui;
}
