#include "weatheritemdelegate.h"
#include <QDebug>
#include <QRect>
#include <QPainter>
#include <QColor>

WeatherItemDelegate::WeatherItemDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
}

void WeatherItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->setBrush(index.row() % 2 ? Qt::gray : Qt::lightGray);
    painter->setPen(Qt::NoPen);
    painter->drawRect(option.rect);


    painter->setPen(Qt::SolidLine);
    QString main = index.data(Qt::DisplayRole).toString();
    QRect r = option.rect.adjusted(100, 0, -10, 0);
    painter->drawText(r, main);

}

QSize WeatherItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    //size.setHeight(size.height() + 20);

    return size;
}
