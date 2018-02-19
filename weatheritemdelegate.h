#ifndef WEATHERITEMDELEGATE_H
#define WEATHERITEMDELEGATE_H

#include <QStyledItemDelegate>

class WeatherItemDelegate : public QStyledItemDelegate
{
public:
    WeatherItemDelegate(QWidget *parent);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // WEATHERITEMDELEGATE_H
