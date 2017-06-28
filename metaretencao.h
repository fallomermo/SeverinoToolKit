#ifndef METARETENCAO_H
#define METARETENCAO_H

#include <QWidget>

namespace Ui {
class MetaRetencao;
}

class MetaRetencao : public QWidget
{
    Q_OBJECT

public:
    explicit MetaRetencao(QWidget *parent = 0);
    ~MetaRetencao();

private:
    Ui::MetaRetencao *ui;
};

#endif // METARETENCAO_H
