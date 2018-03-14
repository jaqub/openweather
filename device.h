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

    const QByteArray& getSubsystem() { return mSubsystem; }

    QStringList getAttrNameList();

    QByteArray getAttrValue(const char *aAttrName);

    int setAttrValue(const char *aAttrName, char *aValue);

private:

    udev_device *mDevice;

    QByteArray mSysName;

    QByteArray mSubsystem;
};

#endif // DEVICE_H
