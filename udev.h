#ifndef UDEV_H
#define UDEV_H
#include <QVector>
#include <libudev.h>
#include "singleton.h"
#include "device.h"

class Udev
{
public:
    Udev();

    QVector<Device *> getDevice(const char* aSubsystem);

private:
    udev *mUdev;
};

typedef Singleton<Udev> UdevSingleton;

#endif // UDEV_H
