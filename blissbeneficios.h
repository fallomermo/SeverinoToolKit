#ifndef BLISSBENEFICIOS_H
#define BLISSBENEFICIOS_H

#include <QWidget>

namespace Ui {
class BlissBeneficios;
}

class BlissBeneficios : public QWidget
{
    Q_OBJECT

public:
    explicit BlissBeneficios(QWidget *parent = 0);
    ~BlissBeneficios();

private:
    Ui::BlissBeneficios *ui;
};

#endif // BLISSBENEFICIOS_H
