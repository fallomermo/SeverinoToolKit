#ifndef SOBRE_H
#define SOBRE_H

#include <QWidget>

namespace Ui {
class Sobre;
}

class Sobre : public QWidget
{
    Q_OBJECT

public:
    explicit Sobre(QWidget *parent = 0);
    ~Sobre();

private:
    Ui::Sobre *ui;
};

#endif // SOBRE_H
