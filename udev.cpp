#include "udev.h"
#include <QtGlobal>
#include <QDebug>

Udev::Udev()
{
    mUdev = udev_new();
    Q_CHECK_PTR(mUdev);
}

Device* Udev::getDeviceBySysname(const char *aSysName, const char *aSubsytem)
{
    Device *dev = nullptr;

    // Acquire enumerate object
    udev_enumerate *enumerate = udev_enumerate_new(mUdev);
    Q_CHECK_PTR(enumerate);
    if (enumerate == nullptr) {
        qWarning() << "Failed to get enumerate object";
        return nullptr;
    }

    //Add filters
    if (udev_enumerate_add_match_sysname(enumerate, aSysName) < 0) {
        qWarning() << "Failed to add" << aSysName << "to match data";
        udev_enumerate_unref(enumerate);
        return nullptr;
    }

    if (aSubsytem && (udev_enumerate_add_match_subsystem(enumerate, aSubsytem) < 0)) {
        qWarning() << "Failed to add" << aSubsytem << "to match data";
        udev_enumerate_unref(enumerate);
        return nullptr;
    }

    // Find devices with specified sys name
    if (udev_enumerate_scan_devices(enumerate) < 0) {
        qWarning() << "Failed to scan devices";
        udev_enumerate_unref(enumerate);
        return nullptr;
    }

    // Iterate over found devices and add them do devices
    udev_list_entry *item, *items;
    items = udev_enumerate_get_list_entry(enumerate);
    udev_list_entry_foreach(item, items) {
        const char *devName = udev_list_entry_get_name(item);
        if (devName == nullptr)
            continue;

        udev_device *uDev = udev_device_new_from_syspath(mUdev, devName);
        if (uDev == nullptr)
            continue;

        const char* uDevSysName = udev_device_get_sysname(uDev);
        if (uDevSysName == nullptr) {
            udev_device_unref(uDev);
            continue;
        }

        if (!qstrncmp(aSysName, uDevSysName, qstrlen(aSysName))) {
            qDebug() << "Found device:" << devName;
            dev = new Device(uDev);
            break;
        }

        udev_device_unref(uDev);
    }

    // Release enumerate object and return devices found
    udev_enumerate_unref(enumerate);
    return dev;
}

QVector<Device *> Udev::getDevicesFromSubsystem(const char *aSubsystem)
{
    QVector<Device *> devices;

    // Acquire enumerate object
    udev_enumerate *enumerate = udev_enumerate_new(mUdev);
    Q_CHECK_PTR(enumerate);
    if (enumerate == nullptr) {
        qWarning() << "Failed to get enumerate object";
        return devices;
    }

    // Add filters
    if (udev_enumerate_add_match_subsystem(enumerate, aSubsystem) < 0) {
        qWarning() << "Failed to add subsystem: " << aSubsystem << "to match data";
        udev_enumerate_unref(enumerate);
        return devices;
    }

    // Find devices from specified subsytem
    if (udev_enumerate_scan_devices(enumerate) < 0) {
        qWarning() << "Failed to scan subsytems";
        udev_enumerate_unref(enumerate);
        return devices;
    }

    udev_list_entry *item, *items;
    items = udev_enumerate_get_list_entry(enumerate);
    udev_list_entry_foreach(item, items) {
        const char *devName = udev_list_entry_get_name(item);
        qDebug() << "Device found:" << devName;
        udev_device *device = udev_device_new_from_syspath(mUdev, devName);
        devices.append(new Device(device));
    }

    qDebug() << "Found:" << devices.size() << "devices";

    udev_enumerate_unref(enumerate);
    return devices;
}
