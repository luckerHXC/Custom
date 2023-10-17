#ifndef USERVOLUMEBAR_H
#define USERVOLUMEBAR_H
#include <QApplication>
#include <QSlider>
#include <QMouseEvent>
#include <QPainter>

class UserVolumeBar: public QSlider
{
    Q_OBJECT
public:
    explicit UserVolumeBar(Qt::Orientation orientation,QWidget* parent = nullptr);
    void setvalue(const QString& text);
    void paintEvent(QPaintEvent *event);
    QString savePath;
};

#endif // USERVOLUMEBAR_H
