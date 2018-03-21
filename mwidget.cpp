#include "mwidget.h"
#include <QStyleOption>
#include <QWidget>
#include <QPainter>

MWidget::MWidget(QWidget *parent) : QWidget(parent)
{

}

void MWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}
