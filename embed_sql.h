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
    bool init_sql(QString hostname=QString("127.0.0.1"),
                  QString database=QString("embedding"),
                  QString username=QString("root"),
                  QString password=QString("114514"),
                  int port=3306
                  );
    QSqlDatabase getDb();
    QStringList getAllDates();
    QStringList getAllTimesProvidedDate(QString date);
private:
    QSqlDatabase db;
};
