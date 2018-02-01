#include <QPushButton>
#include <QtNetwork>
#include <QDebug>

#include "ui_openweathermain.h"

int main(int argc, char *argv[])
{
    QNetworkConfigurationManager *ncm = new QNetworkConfigurationManager();
    if (!ncm->isOnline()) {
         qDebug() << "Device is NOT online" << endl;
         delete ncm;
         return -1;
    }

    QList<QNetworkConfiguration> netConfs;
    netConfs = ncm->allConfigurations();
    qDebug() << "Got "<< netConfs.size() << " active configurations" << endl;
    for (int i = 0; i < netConfs.size(); i++) {
        QNetworkConfiguration nc = netConfs.at(i);
        qDebug() << nc.identifier() << "of type" << nc.bearerType() << nc.bearerTypeName() << " name" << nc.name() <<
                  " state" << (0xe == nc.state()? " active" : " not active");
    }
}
