#ifndef EMBED_SQL_H
#define EMBED_SQL_H

#endif // EMBED_SQL_H
#pragma once
#include <QString>
#include <QtSql>
#include <QDebug>


class embed_sql
{
public:
    embed_sql();
    bool init_sql();
    QSqlDatabase db;
    QSqlDatabase getDb();
private:

};
