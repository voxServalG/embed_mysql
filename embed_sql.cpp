#include "embed_sql.h"

bool connect_sql(QSqlDatabase& db, QString& hostname, QString& databaseName, QString& username, QString& password, int& port)
{
    db.setHostName(hostname); // Host name
    db.setDatabaseName(databaseName); // Database name
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
