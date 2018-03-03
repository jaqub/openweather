#include "device.h"
#include <QtGlobal>
#include <QDebug>

Device::Device(udev_device *aDevice) : mDevice(aDevice)
{
    Q_CHECK_PTR(aDevice);
    mName = udev_device_get_sysname(mDevice);
}

QStringList Device::getAttrNameList()
{
   QStringList attrList;
   udev_list_entry *attrs, *attr;

   attrs = udev_device_get_sysattr_list_entry(mDevice);
   if (attrs == nullptr) {
      qWarning() << "ATTR list for" << mName << "not found";
      return attrList;
   }

   udev_list_entry_foreach(attr, attrs) {
      const char *attrName = udev_list_entry_get_name(attr);
      qDebug() << mName << "- found ATTR:" << attrName;
      attrList.append(attrName);
   }

   return attrList;
}

QByteArray Device::getAttrValue(char *aAttrName)
{
    if (aAttrName == nullptr) {
        qInfo() << "Attr name set to null";
        return nullptr;
    }

    const char *value = udev_device_get_sysattr_value(mDevice, aAttrName);
    if (value == nullptr)
        qWarning() << aAttrName << "not found";

    qDebug() << mName << "ATTR:" << aAttrName << ":" << value;

    return value;
}

int Device::setAttrValue(char *aAttrName, char *aValue)
{
    int ret = udev_device_set_sysattr_value(mDevice, aAttrName, aValue);
    if (ret < 0)
        qWarning() << "Failed to set" << aAttrName << "with" << aValue;

    qDebug() << mName << "ATTR:" << aAttrName << "set to" << aValue;

    return ret;
}
