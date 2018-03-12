#ifndef DEVICE_H
#define DEVICE_H
#include <libudev.h>
#include <QStringList>

class Device
{
public:
    explicit Device(udev_device *aDevice = nullptr);

    virtual ~Device() { udev_device_unref(mDevice); }

    const QByteArray& getSysName() { return mSysName; }

    QStringList getAttrNameList();

    QByteArray getAttrValue(char *aAttrName);

    int setAttrValue(char *aAttrName, char *aValue);

private:

    udev_device *mDevice;
    QByteArray mSysName;
};

#endif // DEVICE_H
