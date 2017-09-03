#ifndef METARETENCAOESTRUTURADA_H
#define METARETENCAOESTRUTURADA_H

#include <QWidget>

namespace Ui {
class MetaRetencaoEstruturada;
}

class MetaRetencaoEstruturada : public QWidget
{
    Q_OBJECT

public:
    explicit MetaRetencaoEstruturada(QWidget *parent = 0);
    ~MetaRetencaoEstruturada();

private:
    Ui::MetaRetencaoEstruturada *ui;
};

#endif // METARETENCAOESTRUTURADA_H
