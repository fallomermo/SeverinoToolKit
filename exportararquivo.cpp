#include "exportararquivo.h"

ExportarArquivo::ExportarArquivo(QWidget *parent)
    :
      QWidget(parent) { }

ExportarArquivo::ExportarArquivo(QWidget *parent, QTableWidget *t)
    :
      QWidget(parent), table(t)
{}

void ExportarArquivo::exportar(QString tituloArquivo, int removeColunas)
{
    if(table->rowCount() <= 0) {
        return;
    } else {
        QFileDialog dialogo;
        dialogo.setDirectory(QDir::homePath());
        QString filename = dialogo.getSaveFileName(this, tr("Exportar para CSV"), QString(tituloArquivo), "CSV (*.csv);;Texto (*.txt)");
        if(filename.isEmpty())
            return;

        QFile f( filename );
        if (f.open(QFile::WriteOnly | QFile::Truncate)) {
            QTextStream data( &f );
            QStringList strList;
            QTableWidget *nTable = table;
            if(QMessageBox::question(this, tr("Exportar arquivo..."), QString("Deseja exportar a descrição das colunas?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes) {
                strList.clear();
                for (int coluna = 0; coluna < nTable->columnCount() - removeColunas; ++coluna) {
                    strList << "\""+nTable->horizontalHeaderItem(coluna)->data(Qt::DisplayRole).toString().trimmed()+"\"";
                }
                data << strList.join( ";" )+"\n";
            }
            for( int r = 0; r < nTable->rowCount(); ++r ) {
                strList.clear();
                for( int c = 0; c < nTable->columnCount()-removeColunas; ++c ) {
                    strList << "\""+nTable->item( r, c )->text().trimmed()+"\"";
                }
                data << strList.join( ";" )+"\n";
            }
            f.close();
            emit mensagemRetorno(QString("Arquivo exportado com sucesso!"));
        }
    }
}
