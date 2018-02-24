#ifndef CONTROLEDEPONTO_H
#define CONTROLEDEPONTO_H

#include <QWidget>

namespace Ui {
class ControleDePonto;
}

class ControleDePonto : public QWidget
{
    Q_OBJECT

public:
    explicit ControleDePonto(QWidget *parent = 0);
    ~ControleDePonto();

private:
    Ui::ControleDePonto *ui;
};

#endif // CONTROLEDEPONTO_H
