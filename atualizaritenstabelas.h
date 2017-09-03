#ifndef ATUALIZARITENSTABELAS_H
#define ATUALIZARITENSTABELAS_H

#include <QObject>

class AtualizarItensTabelas : public QObject
{
    Q_OBJECT
public:
    explicit AtualizarItensTabelas(QObject *parent = nullptr);

signals:

public slots:
};

#endif // ATUALIZARITENSTABELAS_H