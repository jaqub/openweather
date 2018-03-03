#include "device.h"
#include <QtGlobal>

Device::Device(udev_device *aDevice) : mDevice(aDevice)
{
    Q_CHECK_PTR(aDevice);
}
