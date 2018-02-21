#ifndef WEATHERLISTITEM_H
#define WEATHERLISTITEM_H

#include <QObject>
#include <QListWidgetItem>

class WeatherListItem : public QObject,
                        public QListWidgetItem
{
    Q_OBJECT

public:
    WeatherListItem(QListWidget *parent = 0);

public slots:
    void onIconDownload();
};

#endif // WEATHERLISTITEM_H
