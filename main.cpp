// --------------------------------------------------------------------------------------------
// Weather forecast application
// Shows weather forecast for Saint-Petersburg data
// Commandline parameters will be added in the next version
// --------------------------------------------------------------------------------------------

#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlFileSelector>
#include <QQuickView>
#include <QQmlContext>

#include "forecastmodel.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setOrganizationName("Weather forcast app");

    QGuiApplication app(argc, argv);
    ForecastModel *forecast_model = new ForecastModel();

    QQuickView view;
    view.engine()->rootContext()->setContextProperty("forecastModel", forecast_model);
    view.connect(view.engine(), &QQmlEngine::quit, &app, &QCoreApplication::quit);
    view.setSource(QUrl("qrc:/main.qml"));
    if (view.status() == QQuickView::Error)
        return -1;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.show();
    return app.exec();
}

