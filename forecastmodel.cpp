#include <QSqlRecord>
#include <QSqlQuery>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "initdb.h"
#include "forecastmodel.h"

const char* ForecastModel::SQL_SELECT =
    "SELECT * FROM days";

ForecastModel::ForecastModel(QObject *parent)
    : QSqlQueryModel{parent}, network_manager_(new QNetworkAccessManager(this))
{
     initDb();
     this->setQuery(SQL_SELECT);
     connect(network_manager_, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(networkReplied(QNetworkReply*)));

     sendWeatherReuest();
}

void ForecastModel::sendWeatherReuest()
{
    QUrl url("http://api.weatherapi.com/v1/forecast.json");

    QUrlQuery params;

//TODO: parametrize the city  and forecast length
    params.addQueryItem("q", "Saint-Petersburg");
    params.addQueryItem("days", "8");

//This private key will expire soon
    params.addQueryItem("key", "666cce5ae904404592285101231711");
    url.setQuery(params);
    QNetworkRequest request(url);
    network_manager_->get(request);
}

void ForecastModel::networkReplied(QNetworkReply *reply)
{
    auto data = reply->readAll();

    QJsonParseError error_struct;
    QJsonDocument doc =  QJsonDocument::fromJson(data, &error_struct);
    if(doc.isNull())
    {
        return;
    }
    QJsonObject root = doc.object();
    if(root.contains("forecast"))
    {
        QJsonObject forecast = root["forecast"].toObject();
        if(forecast.contains("forecastday"))
        {
            assert(forecast["forecastday"].isArray());
            QJsonArray forecastdays = forecast["forecastday"].toArray();
            for (auto node : forecastdays) {
                QJsonObject forecastday = node.toObject();
                QJsonObject day = forecastday["day"].toObject();
                dbAddDay(QDate::fromString(forecastday["date"].toString(), "yyyy-MM-dd"), day["condition"].toObject()["text"].toString(), day["mintemp_c"].toDouble(), day["maxtemp_c"].toDouble(), day["avgtemp_c"].toDouble(), day["maxwind_kph"].toDouble(), day["daily_chance_of_rain"].toInt(), day["totalprecip_mm"].toDouble());
            }

        }
    }
    this->setQuery(SQL_SELECT);
}

QHash<int, QByteArray> ForecastModel::roleNames() const
{
   QHash<int, QByteArray> roles;
   for (int i = 0; i < this->record().count(); i ++) {
       roles.insert(Qt::UserRole + i + 1, record().fieldName(i).toUtf8());
   }
   return roles;
}

QVariant ForecastModel::data(const QModelIndex &index, int role) const
{
    QVariant value;

    if(role < Qt::UserRole) {
        value = QSqlQueryModel::data(index, role);
    }
    else {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
    }
    return value;
}

void ForecastModel::appendComment(int index, const QString & commentText)
{
    dbAddComment(index, commentText);

    //TODO: Update data notifier
    this->setQuery(SQL_SELECT);
}
