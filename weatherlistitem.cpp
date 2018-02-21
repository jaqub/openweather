#include "weatherlistitem.h"
#include <QNetworkReply>

WeatherListItem::WeatherListItem(QListWidget *parent):
    QListWidgetItem(parent)
{

}

void WeatherListItem::onIconDownload()
{
    QNetworkReply *rpl = qobject_cast<QNetworkReply*>(sender());
    Q_CHECK_PTR(rpl);

    if (rpl == nullptr)
        return;

    if (rpl->error()) {
        qDebug() << "Request finished with error:" << rpl->errorString();
        rpl->deleteLater();
        return;
    }

    QPixmap pixmap;
    pixmap.loadFromData(rpl->readAll());
    setIcon(QIcon(pixmap));

    qDebug() << "got Icon";

    rpl->deleteLater();
}
