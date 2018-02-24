#ifndef PROPRIEDADESUSUARIO_H
#define PROPRIEDADESUSUARIO_H

#include <QWidget>

namespace Ui {
class PropriedadesUsuario;
}

class PropriedadesUsuario : public QWidget
{
    Q_OBJECT

public:
    explicit PropriedadesUsuario(QWidget *parent = 0);
    ~PropriedadesUsuario();

private:
    Ui::PropriedadesUsuario *ui;
};

#endif // PROPRIEDADESUSUARIO_H
