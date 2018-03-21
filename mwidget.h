#ifndef MWIDGET_H
#define MWIDGET_H

#include <QWidget>

class MWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // MWIDGET_H
