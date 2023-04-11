#include "embed_sql.h"

embed_sql::embed_sql()
{

}
bool embed_sql::init_sql()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    QString hostname = QString("127.0.0.1");
    QString database = QString("embedding");
    QString username = QString("root");
    QString password = QString("114514");
    int port = 3306;

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

/*QSqlDatabase embed_sql::getDb()
{
    return db;
}*/
