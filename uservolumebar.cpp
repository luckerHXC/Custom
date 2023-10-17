#include "uservolumebar.h"
#include <QApplication>
#include <QSlider>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>
#include "QDebug"
#include <QLayout>
#include <QMenu>
#include <QDrag>
#include <QFileDialog>
#include "draggableslider.h"
#include "widgethelper.h"
#include "connectsl.h"
UserVolumeBar::UserVolumeBar(Qt::Orientation orientation, QWidget* parent)
    :QSlider(orientation, parent)
{
    setRange(-100, 0);
}

//同步值
void UserVolumeBar::setvalue(const QString& text){
    this->setValue(text.toInt());
}


void UserVolumeBar::paintEvent(QPaintEvent *event)
{
    QSlider::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 获取滑槽的外观样式
    QStyleOptionSlider opt;
    initStyleOption(&opt);

    QRect grooveRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);

    // 获取滑块的位置和范围
    int minVal = minimum();
    int maxVal = maximum();
    int val = value();

    // 计算填充部分的矩形
    int fillHeight = (grooveRect.height() * (val - minVal)) / (maxVal - minVal);
    QRect fillRect = grooveRect.adjusted(0, grooveRect.height() - fillHeight, 0, 0);

    // 填充低于当前值的部分为蓝色
    painter.fillRect(grooveRect, Qt::white);
    painter.fillRect(fillRect, Qt::blue);
}
