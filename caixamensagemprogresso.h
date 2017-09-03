#ifndef CAIXAMENSAGEMPROGRESSO_H
#define CAIXAMENSAGEMPROGRESSO_H

#include <QDialog>

namespace Ui {
class CaixaMensagemProgresso;
}

class CaixaMensagemProgresso : public QDialog
{
    Q_OBJECT

public:
    explicit CaixaMensagemProgresso(QWidget *parent = 0);
    ~CaixaMensagemProgresso();

private:
    Ui::CaixaMensagemProgresso *ui;
};

#endif // CAIXAMENSAGEMPROGRESSO_H
