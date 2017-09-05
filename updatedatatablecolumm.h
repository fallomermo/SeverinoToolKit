#ifndef UPDATEDATATABLECOLUMM_H
#define UPDATEDATATABLECOLUMM_H

#include <QObject>

class UpdateDataTableColumm : public QObject
{
    Q_OBJECT
public:
    explicit UpdateDataTableColumm(QObject *parent = nullptr);

    int getLinha() const;
    void setLinha(int value);

    int getColuna() const;
    void setColuna(int value);

    int getCodigoEmpresa() const;
    void setCodigoEmpresa(int value);

    int getMatricula() const;
    void setMatricula(int value);

    QString getNome() const;
    void setNome(const QString &value);

    int getCodigoFilial() const;
    void setCodigoFilial(int value);

    QString getFilial() const;
    void setFilial(const QString &value);

    bool getUltimoRegistro() const;
    void setUltimoRegistro(bool value);

private:
    int linha;
    int coluna;
    int codigoEmpresa;
    int matricula;
    QString nome;
    int codigoFilial;
    QString filial;
    bool ultimoRegistro;
};

#endif // UPDATEDATATABLECOLUMM_H
