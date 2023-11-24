#ifndef INITDB_H
#define INITDB_H
// --------------------------------------------------------------------------------------------
// Database initialization
// --------------------------------------------------------------------------------------------

#include <QtSql>

const auto DAYS_SQL = QLatin1String(R"(
    create table days(id integer primary key, forecast_date date, condition varchar, min_temp_c double, max_temp_c double, avg_temp_c double, maxwind_kph double, chance_of_rain int, totalprecip_mm double, comment varchar)
    )");
const auto HOURS_SQL = QLatin1String(R"(
    create table hours(id integer primary key, forecast_date_id date_id, forecast_time integer, condition varchar, temp_c double, wind_kph double, wind_dir varchar, precip_mm double, presure_mb double)
    )");
const auto SELECT_DAY_SQL = QLatin1String(R"(
    select * from days where forecast_date = ?
    )");
const auto INSERT_DAY_SQL = QLatin1String(R"(
    insert into days(forecast_date, condition, min_temp_c, max_temp_c, avg_temp_c, maxwind_kph, chance_of_rain, totalprecip_mm, comment)
                      values(?, ?, ?, ?, ?, ?, ?, ?, '')
    )");
const auto UPDATE_DAY_SQL = QLatin1String(R"(
    update days(condition, min_temp_c, max_temp_c, avg_temp_c, maxwind_kph, chance_of_rain, totalprecip_mm)
                      values(?, ?, ?, ?, ?, ?, ?) WHERE forecast_date = ?
    )");
const auto UPDATE_COMMENT_SQL = QLatin1String(R"(
    update days set comment = ? WHERE id = ? )");


int dbAddDay(const QDate & forecast_date, const QString & condition, double min_temp_c, double max_temp_c, double avg_temp_c, double maxwind_kph, int chance_of_rain, double totalprecip_mm)
{
    QSqlQuery q_select;
    if (!q_select.prepare(SELECT_DAY_SQL))
        return -1;
    q_select.addBindValue(forecast_date.toString("yyyy-MM-dd"));
    q_select.exec();
    if(q_select.next())
    {
        QSqlQuery q;
        if (!q.prepare(UPDATE_DAY_SQL))
            return -1;
        q.addBindValue(condition);
        q.addBindValue(min_temp_c);
        q.addBindValue(max_temp_c);
        q.addBindValue(avg_temp_c);
        q.addBindValue(maxwind_kph);
        q.addBindValue(chance_of_rain);
        q.addBindValue(totalprecip_mm);
        q.addBindValue(forecast_date.toString("yyyy-MM-dd"));
        q.exec();
        return q.lastInsertId().toInt();
    }
    else
    {
        QSqlQuery q;
        if (!q.prepare(INSERT_DAY_SQL))
            return -1;
        q.addBindValue(forecast_date.toString("yyyy-MM-dd"));
        q.addBindValue(condition);
        q.addBindValue(min_temp_c);
        q.addBindValue(max_temp_c);
        q.addBindValue(avg_temp_c);
        q.addBindValue(maxwind_kph);
        q.addBindValue(chance_of_rain);
        q.addBindValue(totalprecip_mm);
        q.exec();
        return q.lastInsertId().toInt();
    }
}

int dbAddComment(int id, const QString & comment)
{
    QSqlQuery q;
    if (!q.prepare(UPDATE_COMMENT_SQL))
    {
        return -1;
    }

    q.addBindValue(comment);
    q.addBindValue(id);
    if(!q.exec())
        qDebug() << q.lastError();
    return q.lastInsertId().toInt();
}

QSqlError initDb()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("ziv_rstest");

    if (!db.open())
        return db.lastError();

    QStringList tables = db.tables();
    if (tables.contains("days", Qt::CaseInsensitive)
        && tables.contains("hours", Qt::CaseInsensitive))
        return QSqlError();

    QSqlQuery q;
    if (!q.exec(DAYS_SQL))
        return q.lastError();
    if (!q.exec(HOURS_SQL))
        return q.lastError();

    return QSqlError();
}

#endif
