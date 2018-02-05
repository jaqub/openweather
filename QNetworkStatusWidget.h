#ifndef QNETWORKSTATUSWIDGET_H
#define QNETWORKSTATUSWIDGET_H

#include <QLabel>
#include <QNetworkConfigurationManager>

class QPixmap;

class QNetworkStatusWidget : public QLabel
{
    Q_OBJECT

public:
    QNetworkStatusWidget(QWidget *parent = Q_NULLPTR);

    bool isOnline(void) { return networkMgr->isOnline(); }

private slots:
    void onNetworkStatusChange(bool isActive);

private:
    QNetworkConfigurationManager *networkMgr;
};

#endif // QNETWORKSTATUSWIDGET_H
