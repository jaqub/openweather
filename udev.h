#ifndef UDEV_H
#define UDEV_H
#include <libudev.h>
#include "singleton.h"

class Udev
{
public:
    Udev();

private:
    udev *mUdev;
};

typedef Singleton<Udev> UdevSingleton;

#endif // UDEV_H
