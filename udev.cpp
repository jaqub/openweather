#include "udev.h"
#include <QtGlobal>

Udev::Udev()
{
    mUdev = udev_new();
    Q_CHECK_PTR(mUdev);
}
