#ifndef RESPONSAVELSELECAOAGREGADO_H
#define RESPONSAVELSELECAOAGREGADO_H

#include <QObject>

class ResponsavelSelecaoAgregado : public QObject
{
    Q_OBJECT
public:
    explicit ResponsavelSelecaoAgregado(QObject *parent = nullptr);

    QString getReposavel() const;
    void setReposavel(const QString &value);

    int getNumeroAdmitidos() const;
    void setNumeroAdmitidos(int value);

    int getNumeroDemitidos() const;
    void setNumeroDemitidos(int value);

private:
    QString reposavel;
    int numeroAdmitidos;
    int numeroDemitidos;
};

#endif // RESPONSAVELSELECAOAGREGADO_H
