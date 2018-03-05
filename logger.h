#ifndef LOGGER_H
#define LOGGER_H

#include <QTextStream>

#ifndef LOG_PATH
  #define LOG_PATH "/tmp/"
#endif  //LOG_PATH

class QFile;

class Logger : public QTextStream
{
public:
    static Logger& get();

private:
    Logger();

    ~Logger();

    Q_DISABLE_COPY(Logger)

    static const unsigned int mMAX_FILE_SIZE = 5 * 1024 * 1024;

    static Logger mInstance;

    QString mFileName;

    QFile *mLogFile;
};

#endif // LOGGER_H
