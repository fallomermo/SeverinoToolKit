#ifndef ATUALIZARITENSTABELAS_H
#define ATUALIZARITENSTABELAS_H

#include <QObject>
#include <QTableWidget>

class AtualizarItensTabelas : public QObject
{
    Q_OBJECT
public:
    explicit AtualizarItensTabelas(QObject *parent = nullptr);
    AtualizarItensTabelas(QObject *parent = nullptr, QTableWidget *tabela = nullptr);

signals:

public slots:
};

#endif // ATUALIZARITENSTABELAS_H
