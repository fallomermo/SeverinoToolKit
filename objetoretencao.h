#ifndef OBJETORETENCAO_H
#define OBJETORETENCAO_H

#include <QObject>

class ObjetoRetencao : public QObject
{
    Q_OBJECT
public:
    explicit ObjetoRetencao(QObject *parent = nullptr);

signals:

public slots:
};

#endif // OBJETORETENCAO_H