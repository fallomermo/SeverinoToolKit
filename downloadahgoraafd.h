#ifndef DOWNLOADAHGORAAFD_H
#define DOWNLOADAHGORAAFD_H

#include <QWidget>

namespace Ui {
class DownloadAhgoraAFD;
}

class DownloadAhgoraAFD : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadAhgoraAFD(QWidget *parent = 0);
    ~DownloadAhgoraAFD();

private:
    Ui::DownloadAhgoraAFD *ui;
};

#endif // DOWNLOADAHGORAAFD_H
