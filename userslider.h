#ifndef USERSLIDER_H
#define USERSLIDER_H
#include <QApplication>
#include <QSlider>
#include <QMouseEvent>
#include <QPainter>

class UserSlider: public QSlider
{
    Q_OBJECT
public:
    explicit UserSlider(Qt::Orientation orientation,QWidget* parent = nullptr);
    void setvalue(const QString& text);
    void paintEvent(QPaintEvent *event);
    QString savePath;

};

#endif // USERSLIDER_H
