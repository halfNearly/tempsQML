#include <QApplication>
#include <QWindow>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QNetworkProxyFactory>

#include <googlebrowserlocation.h>
#include "weathercommon.h"
#include "weathermodel.h"
#include "weatherdailymodel.h"
#include "systemtrayicon.h"
#include "settings.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName("tempsQML");
    QCoreApplication::setOrganizationName("vladest");
    QCoreApplication::setOrganizationDomain("vladest.org");

    QApplication app(argc, argv);

#ifdef Q_OS_LINUX
    QApplication::setWindowIcon(QIcon(":/tempsqml.png"));
#endif /* Q_OS_LINUX */

    QNetworkProxyFactory::setUseSystemConfiguration(true);

    qmlRegisterType<WeatherData>("weathermodel", 1, 0, "WeatherData");
    qmlRegisterType<WeatherCommon>("weathermodel", 1, 0, "WeatherCommon");

    GoogleBrowserLocation gbl;
    WeatherCommon wcmn;
    WeatherModel wModel(&wcmn);
    WeatherDailyModel wDailyModel(&wcmn);
    SortFilterProxyModel filteredWeatherModel;

    filteredWeatherModel.setSourceModel(&wModel);

    QObject::connect(&gbl, &GoogleBrowserLocation::browserCoordinateChanged, &wcmn, &WeatherCommon::setCoordinates);
    QObject::connect(&gbl, &GoogleBrowserLocation::tzoffsetChanged, &wcmn, &WeatherCommon::setTimezoneOffset);

    QQmlApplicationEngine engine;
    SystemTrayIcon *trayIcon = new SystemTrayIcon;

    Settings settings;

    engine.rootContext()->setContextProperty("weatherCommon", &wcmn);
    engine.rootContext()->setContextProperty("systemTrayIcon", trayIcon);
    engine.rootContext()->setContextProperty("browserCoordinate", &gbl);
    engine.rootContext()->setContextProperty("weatherModel", &wModel);
    engine.rootContext()->setContextProperty("weatherDailyModel", &wDailyModel);
    engine.rootContext()->setContextProperty("filteredWeatherModel", &filteredWeatherModel);
    engine.rootContext()->setContextProperty("settings", &settings);

    engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));

    return app.exec();
}
