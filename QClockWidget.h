#ifndef QCLOCKWIDGET_H
#define QCLOCKWIDGET_H

#include <QLCDNumber>

class QClockWidget : public QLCDNumber
{
  Q_OBJECT

public:
    QClockWidget(QWidget *parent = Q_NULLPTR);

public Q_SLOTS:
    void showTime();


};

#endif // QCLOCKWIDGET_H
