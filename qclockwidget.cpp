#include "qclockwidget.h"
#include <QtWidgets>

QClockWidget::QClockWidget(QWidget *parent)
  : QLCDNumber(parent)
{
  QTimer *timer = new QTimer(parent);
  connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
  timer->start(1000);
}

void QClockWidget::showTime()
{
  QTime time = QTime::currentTime();
  QString text = time.toString("hh:mm");
  if (time.second() % 2 )
      text[2] = ' ';
  display(text);
}
