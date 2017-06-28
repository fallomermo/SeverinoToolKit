#ifndef HOMEINICIO_H
#define HOMEINICIO_H

#include <QWidget>

namespace Ui {
class HomeInicio;
}

class HomeInicio : public QWidget
{
    Q_OBJECT

public:
    explicit HomeInicio(QWidget *parent = 0);
    ~HomeInicio();

private:
    Ui::HomeInicio *ui;
};

#endif // HOMEINICIO_H
