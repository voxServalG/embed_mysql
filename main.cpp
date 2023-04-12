#include <QApplication>
#include <QtSql>
#include <QDebug>
#include <QString>
#include <QLabel>

#include <QTableView>
#include <QSqlTableModel>
#include <QHeaderView>
#include "embed_sql.h"
#include "mainwindow.h"



int main(int argc, char *argv[])
{
    //Create an QApplication a, and create a MainWindow inside it.
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
