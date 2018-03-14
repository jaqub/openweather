#ifndef BACKLIGHTDEV_H
#define BACKLIGHTDEV_H
#include <QObject>

class Device;

class BacklightDev : public QObject
{
    Q_OBJECT
public:
    explicit BacklightDev(Device *device, QObject *parent = nullptr);

    virtual ~BacklightDev() {}

    int getMaxBrightness();

    int getBrightness();

    int setBrightness(int brightness);

signals:

public slots:

private:

    Device *mDevice;
};

#endif // BACKLIGHTDEV_H
