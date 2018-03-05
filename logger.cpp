#include "logger.h"
#include <QFile>
#include <QFileInfo>
#include <QDebug>

Logger Logger::mInstance;

Logger &Logger::get()
{
   QFileInfo info(LOG_PATH);
   if (info.size() > mMAX_FILE_SIZE) {
       mInstance.mLogFile->close();
       QFile::rename(mInstance.mFileName, QString(mInstance.mFileName) + "1");
       mInstance.mLogFile->open(QIODevice::WriteOnly | QIODevice::Unbuffered);
   }

   return mInstance;
}

Logger::Logger() : mFileName(QStringLiteral(LOG_PATH) + QStringLiteral("openWeather.log"))
{
    mLogFile = new QFile(mFileName);
    Q_CHECK_PTR(mLogFile);

    bool ret = mLogFile->open(QIODevice::WriteOnly | QIODevice::Unbuffered);
    Q_ASSERT_X(ret, "Logger", "Log file cannot be created");

    setDevice(mLogFile);
}

Logger::~Logger()
{
    mInstance.flush();
    mLogFile->close();
    delete mLogFile;
}
