#include "userslider.h"
#include <QApplication>
#include <QSlider>
#include <QMouseEvent>
#include <QPainter>
#include <QStyle>
#include "QDebug"
//用户界面的滑动槽
UserSlider::UserSlider(Qt::Orientation orientation, QWidget* parent)
    :QSlider(orientation, parent)
{
    setRange(-72, 12);
    this->setTickPosition(QSlider::TicksLeft);
    int tickInterval = 12; // 设置刻度间隔为12
    this->setTickInterval(tickInterval);
    // 将样式表字符串应用于垂直方向的QSlider控件
    /*this->setStyleSheet(
    "QSlider::sub-page:vertical { background: rgb(90,49,255);border: 1px solid #777;border-radius: 2px; margin-left: 9px; margin-right: 9px; }"
    "QSlider::handle:vertical {background:rgb(0, 85, 255);}"
    );
    QString styleSheet = "QSlider::add-page { background: yellow; }";
    this->setStyleSheet(styleSheet);*/


}

//同步值
void UserSlider::setvalue(const QString& text){
    this->setValue(text.toInt());
}


void UserSlider::paintEvent(QPaintEvent *event)
{
    QSlider::paintEvent(event); // 调用基类的绘制
    if(savePath==""){

    }else{
    QString currentStyleSheet = this->styleSheet(); // 获取当前的样式表
    QString styleSheet=QString("QSlider::handle { background: rgba(0, 0, 0, 0); }");
    currentStyleSheet += "\n" + styleSheet; // 将新的样式表拼接到原来的样式表后面
    this->setStyleSheet(currentStyleSheet);
    QPixmap pixmap(savePath);
    QPainter painter(this);
    QRect handleRect = QRect();

    if (orientation() == Qt::Vertical) {
        int sliderWidth = width(); // 获取滑块宽度
        int sliderHeight = height(); // 获取滑块高度
        int handleHeight = sliderHeight / 7; // 计算滑块高度，占用整个 slider 高度的七分之一
        int handleWidth = sliderWidth / 1.8; // 计算滑块宽度，占用整个 slider 宽度的二分之一

        int handleY = (sliderHeight - handleHeight) * (maximum() - value()) / (maximum() - minimum()); // 计算滑块位置
        int handleX = (sliderWidth) * 7 / 18; // 计算滑块的横坐标，位于整个 slider 宽度的三分之一位置
        handleRect = QRect(handleX, handleY, handleWidth, handleHeight); // 创建滑块的 QRect

        // 调整图片大小以适应滑块
        pixmap = pixmap.scaled(handleHeight, handleHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    } else if (orientation() == Qt::Horizontal) {
        int sliderWidth = width(); // 获取滑块宽度
        int sliderHeight = height(); // 获取滑块高度
        int handleWidth = sliderWidth / 7; // 计算滑块宽度，占用整个 slider 宽度的七分之一
        int handleHeight = sliderHeight / 1.8; // 计算滑块高度，占用整个 slider 高度的七分之一
        int handleX = (sliderWidth - handleWidth) * (value() - minimum()) / (maximum() - minimum()); // 计算滑块位置
        int handleY = (sliderHeight) * 7 / 18; // 计算滑块的纵坐标，位于整个 slider 宽度的三分之二位置
        handleRect = QRect(handleX, handleY, handleWidth, handleHeight); // 创建滑块的 QRect

        // 调整图片大小以适应滑块
        pixmap = pixmap.scaled(handleWidth, sliderHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation).transformed(QTransform().rotate(270));
    }

    painter.drawPixmap(handleRect, pixmap);
    }
}

