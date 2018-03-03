#ifndef UDEV_H
#define UDEV_H
#include <QVector>
#include <libudev.h>
#include "singleton.h"

class Udev
{
public:
    Udev();

    QVector<udev_device*> getDevice(const char* aSubsystem);

    void releaseDev(udev_device *aDevice);
private:
    udev *mUdev;
};

typedef Singleton<Udev> UdevSingleton;

#endif // UDEV_H
