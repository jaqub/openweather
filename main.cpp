#include <QApplication>
#include <QCommandLineParser>
#include <QTime>

#include "logger.h"
#include "openweather.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString logMsg = (QStringLiteral("\t%1 %2 %3 %4"))
    .arg(QTime::currentTime().toString(QStringLiteral("hh:mm:ss.zzz")))
    .arg(context.file)
    .arg(context.line)
    .arg(msg);

    switch (type) {
    case QtDebugMsg:
        Logger::get() << left << "[Debug]" << logMsg << endl;
        break;
    case QtInfoMsg:
        Logger::get() << left << "[Info]" << logMsg << endl;
        break;
    case QtWarningMsg:
        Logger::get() << left << "[Warning]" << logMsg << endl;
        break;
    case QtCriticalMsg:
        Logger::get() << left << "[Critical]" << logMsg << endl;
        break;
    case QtFatalMsg:
        Logger::get() << left << "[Fatal]" << logMsg << endl;
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

    if (!parser.isSet(appIdOpt) || !parser.isSet(idOpt)) {
        qWarning() << "Missing mandatory input parameter";
        parser.showHelp();
    }

    QString appId = parser.value(appIdOpt);
    QString id = parser.value(idOpt);

    OpenWeather *openWeather = new OpenWeather(appId, id);
    openWeather->showFullScreen();

    int ret = a.exec();

    delete openWeather;

    qInfo() << "Application exit with return code:" << ret;

    return ret;
}
