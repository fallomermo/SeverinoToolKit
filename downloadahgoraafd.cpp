#include "downloadahgoraafd.h"
#include "ui_downloadahgoraafd.h"

DownloadAhgoraAFD::DownloadAhgoraAFD(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadAhgoraAFD)
{
    ui->setupUi(this);
}

DownloadAhgoraAFD::~DownloadAhgoraAFD()
{
    delete ui;
}
