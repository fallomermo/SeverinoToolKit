#ifndef LIQUIDOFOLHA_H
#define LIQUIDOFOLHA_H

#include <QWidget>

namespace Ui {
class LiquidoFolha;
}

class LiquidoFolha : public QWidget
{
    Q_OBJECT

public:
    explicit LiquidoFolha(QWidget *parent = 0);
    ~LiquidoFolha();

private:
    Ui::LiquidoFolha *ui;
};

#endif // LIQUIDOFOLHA_H
