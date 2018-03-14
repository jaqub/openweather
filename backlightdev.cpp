#include "backlightdev.h"
#include "device.h"
#include <QDebug>

BacklightDev::BacklightDev(Device *device, QObject *parent) : QObject(parent)
{
   Q_ASSERT(device != nullptr);
   mDevice = device;

   mMaxBrightness = mDevice->getAttrValue("max_brightness").toInt();
}

}

int BacklightDev::getBrightness()
{
   if (!mDevice)
       return -1;

   QByteArray brightness = mDevice->getAttrValue("brightness");
   return brightness.size() ? brightness.toInt() : -1;
}

int BacklightDev::setBrightness(int brightness)
{
    if (!mDevice)
        return -1;

    return mDevice->setAttrValue("brightness", QByteArray::number(brightness).data());
}
