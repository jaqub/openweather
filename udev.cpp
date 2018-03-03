#include "udev.h"
#include <QtGlobal>
#include <QDebug>

Udev::Udev()
{
    mUdev = udev_new();
    Q_CHECK_PTR(mUdev);
}

QVector<Device *> Udev::getDevice(const char *aSubsystem)
{
    QVector<Device *> devices;
    udev_enumerate *enumerate = udev_enumerate_new(mUdev);
    Q_CHECK_PTR(enumerate);

    // Add filters
    if (udev_enumerate_add_match_subsystem(enumerate, aSubsystem) < 0) {
        qDebug() << "Failed to add subsystem: " << aSubsystem << "to match data";
        udev_enumerate_unref(enumerate);
        return devices;
    }

    // Find devices from specified subsytem
    if (udev_enumerate_scan_devices(enumerate) < 0) {
        qDebug() << "Failed to stan subsytems";
        udev_enumerate_unref(enumerate);
        return devices;
    }

    udev_list_entry *items = udev_enumerate_get_list_entry(enumerate);

    if (items == nullptr) {
        qDebug() << "Non of devices found";
        udev_enumerate_unref(enumerate);
        return devices;
    }

    udev_list_entry *item;
    udev_list_entry_foreach(item, items) {
        const char *devName = udev_list_entry_get_name(item);
        qDebug() << "Device found: " << devName;
        udev_device *device = udev_device_new_from_syspath(mUdev, devName);
        devices.append(new Device(device));
    }
    udev_enumerate_unref(enumerate);
    return devices;
}
