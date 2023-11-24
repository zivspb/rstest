#ifndef DAYSTABLEMODEL_H
#define DAYSTABLEMODEL_H
// --------------------------------------------------------------------------------------------
// Forecast model:
// 1. Initialize forecast SQLite database
// 2. Read JSON-file from server using REST-Api
// 3. Read data from JSON file and put it to the database
// 4. Show the data in QML page using MVC pattern
// 5. Add user comments to the database
// 6. Update the database after every run
// --------------------------------------------------------------------------------------------

#include <QSqlQueryModel>
class QNetworkReply;
class QNetworkAccessManager;
class ForecastModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ForecastModel(QObject *parent = nullptr);
    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE void appendComment(int index, const QString & commentText);
signals:


public slots:
    void networkReplied(QNetworkReply *reply);

private:
    void sendWeatherReuest();

    const static char* SQL_SELECT;
    QNetworkAccessManager *network_manager_;
};

#endif // DAYSTABLEMODEL_H
