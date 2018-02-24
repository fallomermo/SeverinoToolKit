#ifndef DOWNLOADAFD_H
#define DOWNLOADAFD_H

#include <QWidget>

namespace Ui {
class DownloadAFD;
}

class DownloadAFD : public QWidget
{
    Q_OBJECT

public:
    explicit DownloadAFD(QWidget *parent = 0);
    ~DownloadAFD();

private:
    Ui::DownloadAFD *ui;
};

#endif // DOWNLOADAFD_H
