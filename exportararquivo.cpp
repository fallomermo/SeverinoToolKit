#include "exportararquivo.h"

ExportarArquivo::ExportarArquivo(QWidget *parent) : QWidget(parent) { }

ExportarArquivo::ExportarArquivo(QWidget *parent, QTableWidget *t) : QWidget(parent), table(t)
{}

void ExportarArquivo::exportar(QString tituloArquivo)
{
    if(table->rowCount() <= 0) {
        emit mensagemRetorno(QString("Não existe informação para exportar!!!"));
        return;
    } else {
        QString filename = QFileDialog::getSaveFileName(this, tr("Exportação CSV"), QDir::homePath()+QString(tituloArquivo), "CSV (*.csv)");
        if(filename.isEmpty())
            return;

        QFile f( filename );
        if (f.open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream data( &f );
            QStringList strList;
            QTableWidget *nTable = table;

            strList.clear();
            for (int coluna = 0; coluna < nTable->columnCount(); ++coluna) {
                strList << "\""+nTable->horizontalHeaderItem(coluna)->data(Qt::DisplayRole).toString().trimmed()+"\"";
            }
            data << strList.join( ";" )+"\n";
            for( int r = 0; r < nTable->rowCount(); ++r )
            {
                strList.clear();
                for( int c = 0; c < nTable->columnCount(); ++c )
                {
                    strList << "\""+nTable->item( r, c )->text().trimmed()+"\"";
                }
                data << strList.join( ";" )+"\n";
            }
            f.close();
            emit mensagemRetorno(QString("Arquivo salvo com Sucesso!"));
        }
    }
}
