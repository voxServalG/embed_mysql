#include "embed_sql.h"

embed_sql::embed_sql()
{

}
bool embed_sql::init_sql(QString hostname,
                         QString database,
                         QString username,
                         QString password,
                         int port
                         )
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    /*QString hostname = QString("127.0.0.1");
    QString database = QString("embedding");
    QString username = QString("root");
    QString password = QString("114514");
    int port = 3306;*/

    db.setHostName(hostname); // Host name
    db.setDatabaseName(database); // Database name
    db.setUserName(username);     // User name
    db.setPassword(password);   // Password
    db.setPort(port);           // Port to connect

    if( db.open() == true ) // Open connection
    {
        return true;
    }
    else
    {
        return false;
    }
}

QSqlDatabase embed_sql::getDb()
{
    return db;
}

QStringList embed_sql::getAllDates()
{
    QSqlQuery query;
    QStringList tableNames;
    if (query.exec("SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME LIKE \"\%field\%\" AND TABLE_ROWS > 0;"))
    {
        while (query.next())
        {
            QString tableName = query.value(0).toString();
            tableNames.append(tableName.mid(11, 8));
        }
    }
    tableNames = tableNames.toSet().toList(); //去除重复
    tableNames.sort();
    return tableNames;
}

QStringList embed_sql::getAllTimesProvidedDate(QString date)
{
    QSqlQuery query;
    QStringList tableNames;
    QString querySql= "SELECT TABLE_NAME FROM INFORMATION_SCHEMA.TABLES WHERE TABLE_NAME LIKE \"\%" + date + "\%\" AND TABLE_ROWS > 0;";
    qDebug() << querySql;
    if(query.exec(querySql))
    {
        while(query.next())
        {
            QString tableName = query.value(0).toString();
            tableNames.append(tableName.right(6));
        }
    }
    tableNames = tableNames.toSet().toList();
    tableNames.sort();
    return tableNames;
}
