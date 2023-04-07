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
    /*
    //set login information
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    QString hostname = QString("127.0.0.1");
    QString database = QString("embedding");
    QString username = QString("root");
    QString password = QString("114514");
    int port = 3306;
    bool connect_state = false;

    //Connect database
    connect_state = connect_sql(db,
                                hostname,
                                database,
                                username,
                                password,
                                port);



    //
    QLabel lab;
    lab.setAlignment(Qt::AlignCenter);
    lab.setGeometry(100,100,400,400);


    if (connect_state)
    {
        QSqlTableModel* model = new QSqlTableModel(nullptr, db);
        model->setTable("field_data");
        model->setSort(1, Qt::AscendingOrder);
        model->setHeaderData(1, Qt::Horizontal, "time");
        model->setHeaderData(2, Qt::Horizontal, "temp");
        model->setHeaderData(3, Qt::Horizontal, "humid");
        model->setHeaderData(4, Qt::Horizontal, "light");
        model->select();

        QTableView* view = new QTableView();
        view->setModel(model);
        view->setSelectionMode(QAbstractItemView::SingleSelection);
        view->setSelectionBehavior(QAbstractItemView::SelectRows);
        view->resizeColumnsToContents();
        view->setEditTriggers(QAbstractItemView::NoEditTriggers);

        QHeaderView* header = view->horizontalHeader();
        header->setStretchLastSection(true);

        view->show();

    }

    else
    {
        lab.setText("Connect failed");
        lab.show();
    }
*/
    MainWindow w;
    w.show();
    return a.exec();
}
