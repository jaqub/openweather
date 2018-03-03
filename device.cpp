#include "device.h"
#include <QtGlobal>
#include <QDebug>

Device::Device(udev_device *aDevice) : mDevice(aDevice)
{
    Q_CHECK_PTR(aDevice);
}

QStringList Device::getAttrNameList()
{
   QStringList attrList;
   udev_list_entry *attrs, *attr;

   attrs = udev_device_get_sysattr_list_entry(mDevice);
   if (attrs == nullptr) {
      qDebug() << "ATTRs for device not found";
      return attrList;
   }

   udev_list_entry_foreach(attr, attrs) {
      const char *attrName = udev_list_entry_get_name(attr);
      qDebug() << "Found ATTR[" << attrName << "]";
      attrList.append(attrName);
   }

   return attrList;
}

QByteArray Device::getAttrValue(char *aAttrName)
{
    if (aAttrName == nullptr) {
        qDebug() << "Attr name set to null";
        return nullptr;
    }

    const char *value = udev_device_get_sysattr_value(mDevice, aAttrName);
    if (value == nullptr)
        qWarning() << aAttrName << "not found";

    return value;
}

int Device::setAttrValue(char *aAttrName, char *aValue)
{
    int ret = udev_device_set_sysattr_value(mDevice, aAttrName, aValue);
    if (ret < 0)
        qWarning() << "Failed to set" << aAttrName << "with" << aValue;

    return ret;
}
