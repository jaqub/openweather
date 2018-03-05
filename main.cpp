#include <QApplication>
#include <QCommandLineParser>
#include <QTime>

#include "logger.h"
#include "openweather.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();

    Logger::get() << QTime::currentTime().toString(QStringLiteral("hh:mm:ss.zzz"));

    switch (type) {
    case QtDebugMsg:
        Logger::get() << " [Debug] " << localMsg.constData() << endl;
        break;
    case QtInfoMsg:
        Logger::get() << " [Info] " << localMsg.constData() << endl;
        break;
    case QtWarningMsg:
        Logger::get() << " [Warning] " << context.file << context.line << context.function << localMsg.constData() << endl;
        break;
    case QtCriticalMsg:
        Logger::get() << " [Critical] " << context.file << context.line << context.function << localMsg.constData() << endl;
        break;
    case QtFatalMsg:
        Logger::get() << " [Fatal] " << context.file << context.line << context.function << localMsg.constData() << endl;
        abort();
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);
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
