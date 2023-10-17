#include "userdial.h"
#include "draggabledial.h"
#include "mainwindow.h"
#include "controlproperty.h"
#include <QDebug>
#include <QLayout>
#include <QMenu>
#include <QDomDocument>
#include <QTimer>
#include <QApplication>
#include <QPushButton>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QMimeData>
#include <QDrag>
#include "widgethelper.h"
UserDial::UserDial(QWidget* parent):QDial(parent)
{

}


void UserDial::paintEvent(QPaintEvent *event)
{
    QDial::paintEvent(event); // 首先调用基类的绘制

    int width = this->width();
    int height = this->height();
    int side = qMin(width, height);
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);//渲染提示，启用抗锯齿和文本抗锯齿
    painter.translate(width / 2, height / 2);//坐标原点平移到控件的中心(默认在左上角)
    painter.scale(side / 200.0, -side / 200.0);//对绘图坐标系统进行缩放变换

    // 绘制调音旋钮的外观
    drawTunerKnob(&painter);

    // 绘制当前值
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 12));
    QString valueText = QString::number(value());

    // 计算文本的矩形框，增大框的大小
    QFontMetrics fm(painter.font());
    QRect textRect = fm.boundingRect(valueText);
    textRect.setWidth(textRect.width() + 50); // 增大宽度
    textRect.setHeight(textRect.height() + 10); // 增大高度

    // 将文本框移动到中心
    textRect.moveCenter(QPoint(0, 0));

    // 绘制文本框
    painter.setPen(QPen(Qt::black, 2));
    painter.drawRect(textRect);

    // 绘制当前值，但将Y坐标翻转
    painter.scale(1, -1); // 仅翻转Y轴
    painter.drawText(textRect, Qt::AlignCenter, valueText);
    painter.scale(1, -1); // 恢复Y轴翻转
}

void UserDial::drawTunerKnob(QPainter *painter)
{
    int radius = 80; // 调音旋钮的半径
    painter->save();

    // 绘制调音旋钮的底座
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(100, 100, 100)); // 底座的颜色
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);

    // 绘制调音旋钮的刻度
    int numTicks = 12; // 刻度的数量
    double angleStep = 360.0 / numTicks;
    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine));
    for (int i = 0; i < numTicks; ++i) {
        painter->drawLine(0, -radius + 5, 0, -radius + 15); // 调音旋钮上的刻度线
        painter->rotate(angleStep);
    }

    // 绘制调音旋钮的指针
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(255, 0, 0)); // 指针的颜色
    painter->rotate(-value() * (360.0 /(maximum()-minimum()))); // 根据当前值旋转指针
    painter->drawPolygon(QPolygonF() << QPointF(0, -radius + 20) << QPointF(-5, -radius + 35) << QPointF(5, -radius + 35));

    painter->restore();
}
