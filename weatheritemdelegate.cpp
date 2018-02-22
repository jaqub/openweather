#include "weatheritemdelegate.h"
#include <QDebug>
#include <QRect>
#include <QPainter>
#include <QLinearGradient>
#include <QColor>

WeatherItemDelegate::WeatherItemDelegate(QWidget *parent) :
    QStyledItemDelegate(parent)
{
}

void WeatherItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect r;
    QLinearGradient gradient(option.rect.x(), option.rect.y(),
                             option.rect.x(), option.rect.y() + option.rect.height());
    gradient.setColorAt(0.0, Qt::lightGray);
    gradient.setColorAt(0.9, Qt::gray);
    painter->setBrush(gradient);
    painter->setPen(Qt::NoPen);
    painter->drawRect(option.rect);

    QIcon ic = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    r = option.rect.adjusted(0, 0, -option.rect.width()+100, 0);
    ic.paint(painter, r);

    QString main = index.data(Qt::DisplayRole).toString();
    r = option.rect.adjusted(100, 0, 0, 0);
    painter->setPen(Qt::SolidLine);
    painter->drawText(r, main);

}

QSize WeatherItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(size.height() + 20);

    return size;
}
