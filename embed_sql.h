#ifndef EMBED_SQL_H
#define EMBED_SQL_H

#endif // EMBED_SQL_H
#pragma once
#include <QString>
#include <QtSql>
#include <QDebug>

bool connect_sql(QSqlDatabase& db, QString& hostname, QString& databaseName, QString& username, QString& password, int& port);
