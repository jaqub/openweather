#include <QApplication>
#include <QCommandLineParser>

#include "openweather.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCommandLineParser parser;

    QCommandLineOption appIdOpt("appid",
      QApplication::translate("main", "api key obtainted from http://openweathermap.org"), "api key");
    parser.addOption(appIdOpt);

    QCommandLineOption idOpt("id", QApplication::translate("main", "city id"), "city id");
    parser.addOption(idOpt);

    parser.process(a);

    if (!parser.isSet(appIdOpt) || !parser.isSet(idOpt))
        parser.showHelp();

    QString appId = parser.value(appIdOpt);
    QString id = parser.value(idOpt);

    OpenWeather *openWeather = new OpenWeather(appId, id);
    openWeather->showFullScreen();
    int ret = a.exec();

    delete openWeather;

    return ret;
}
