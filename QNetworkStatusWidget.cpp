#include "QNetworkStatusWidget.h"
#include <QNetworkConfigurationManager>
#include <QPixmap>

QNetworkStatusWidget::QNetworkStatusWidget(QWidget *parent)
    : QLabel(parent)
{
    networkMgr = new QNetworkConfigurationManager(this);
    Q_CHECK_PTR(networkMgr);

    setPixmap(QPixmap(":/icons/loading.png"));

    if (networkMgr->isOnline())
        setPixmap(QPixmap(":/icons/connected.png"));

    connect(networkMgr, &QNetworkConfigurationManager::onlineStateChanged,
            this, &QNetworkStatusWidget::onNetworkStatusChange);
}

void QNetworkStatusWidget::onNetworkStatusChange(bool isConnected)
{
    if (isConnected)
        setPixmap(QPixmap(":/icons/connected.png"));
    else
        setPixmap(QPixmap(":/icons/loading.png"));
}
