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
QList<DraggableDial*> DraggableDial::generatedDials;
DraggableDial::DraggableDial(QWidget * parent) : QDial(parent)
{
    setMouseTracking(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &DraggableDial::customContextMenuRequested, this, &DraggableDial::showContextMenu);

}
//鼠标点击
void DraggableDial::mousePressEvent(QMouseEvent* event)
{
    WidgetHelper::mousePressEvent(event,this);
}
//鼠标移动
void DraggableDial::mouseMoveEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseMoveEvent(event,this);
    }
}
//鼠标释放
void DraggableDial::mouseReleaseEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseReleaseEvent(event,this);
    this->Tree();
    emit clicked();
    }
}
//移除
void DraggableDial::remove(){
    generatedDials.removeOne(this);
    deleteLater();
}

//呼出菜单
void DraggableDial::showContextMenu(const QPoint& pos)
{
    WidgetHelper::enable=lock;
    WidgetHelper::showContextMenu(pos,this);
}

//控件放大
void DraggableDial::wheelEvent(QWheelEvent* event){
    if(lock==true){
    WidgetHelper::wheelEvent(event,this);
    }
}
//将控件属性打印到树上
void DraggableDial::Tree(){
    WidgetHelper::name=this->name;
    WidgetHelper::tree(this);
    for(int i=1;i<2;i++)
    {
        //一级节点，加入mModel
        QList<QStandardItem*> items1;
        QStandardItem* item1 = new QStandardItem(QString::number(i));

        if(i==1){
            item1->setText("表盘");
            for(int j=1;j<4;j++)
            {
                //二级节点,加入第1个一级节点
                QList<QStandardItem*> items2;
                QStandardItem* item2 = new QStandardItem(QString::number(j));
                QStandardItem* item3 = new QStandardItem(QStringLiteral("二级节点"));
                if(j==1){
                    item2->setText("最大值");
                    item3->setText(QString::number(this->maximum()));
                }else if(j==2){
                    item2->setText("最小值");
                    item3->setText(QString::number(this->minimum()));
                }else{
                    item2->setText("刻度大小");
                    // 获取刻度范围
                    int min = this->minimum(); // 最小刻度值
                    int max = this->maximum(); // 最大刻度值
                    // 获取刻度总数
                    int numTicks = max - min + 1;
                    // 计算每个刻度的大小
                    angleStep = 360.0 / numTicks;
                    item3->setText(QString::number(angleStep));
                }
                items2.append(item2);
                items2.append(item3);
                item1->appendRow(items2);
            }
        }
        items1.append(item1);
        controlproperty::model->appendRow(items1);

       }
}
//通过属性列表改变值
void DraggableDial::changeproperty(){
    if(lock==true){
    if(this->signalEmitted == true){
    WidgetHelper::changeproperty(this);
    if(controlproperty::firstColumnText=="名字"){
        this->name=controlproperty::newText;
    }
    if(controlproperty::firstColumnText=="最大值"){
        int max =controlproperty::newText.toInt();
        this->setMaximum(max);
        if(control!=nullptr){
        control->setMaximum(max);
        }
     }
    if(controlproperty::firstColumnText=="最小值"){
        int min =controlproperty::newText.toInt();
        this->setMinimum(min);
        if(control!=nullptr){
        control->setMinimum(min);
       }
    }
    if(controlproperty::firstColumnText=="刻度大小"){

        int interval =controlproperty::newText.toInt();
        setSingleStep(interval);
        if(control!=nullptr){
        control->setSingleStep(interval);
       }

    }
  }
 }
}
//改变对象标记，决定属性表可以改变哪些控件
void DraggableDial::clicktree() {
    WidgetHelper::clicktree();
    //signalEmitte标记为true才能触发changeproperty
    this->signalEmitted = true;
}

int DraggableDial::getmatchID(){
    return this->matchId;
}

void DraggableDial::setMatchId(int id) {
       matchId = id;
}

void DraggableDial::changimage(){
    if(this->signalEmitted == true){
    qDebug()<<"yata";
    }
}

void DraggableDial::customcontrol(){

}

void DraggableDial::changbackgroundimage(){
    if(this->signalEmitted == true){
    backimagePath = QFileDialog::getOpenFileName(this, tr("选择图片"), "", tr("图片文件 (*.png *.jpg *.jpeg)"));
    if (!backimagePath.isEmpty()) {
        QPixmap pixmap(backimagePath);
        backsavePath = "C://Users//RDSE//Documents//Custom_data//"+QString::number(imageCounter+1)+".jpg";  // 设置保存图片的路径和文件名
        if (pixmap.save(backsavePath)) {
            qDebug() << "图片保存成功：" << backsavePath;
        } else {
            qDebug() << "图片保存失败：" << backsavePath;
        }
        //QString currentStyleSheet = this->styleSheet(); // 获取当前的样式表
        QString styleSheet = QString("QDial {background-image: url(%1);}").arg(backsavePath);
        //currentStyleSheet += "\n" + styleSheet; // 将新的样式表拼接到原来的样式表后面
        this->setStyleSheet(styleSheet);
    }

  }

}

QString DraggableDial::styleSheet(){
    return m_dialStyleSheet;
}

void DraggableDial::setstyleSheet(const QString& styleSheet) {
    m_dialStyleSheet = styleSheet;
    update(); // 重新绘制以应用新的样式表
}


void DraggableDial::paintEvent(QPaintEvent *event)
{

    QDial::paintEvent(event); // 首先调用基类的绘制

    int width = this->width();
    int height = this->height();
    int side = qMin(width, height);


    QPainter painter(this);
    if(drawblue==true){
    painter.setPen(QPen(Qt::blue, 4)); // 蓝色边框，线宽为2像素
    QRect rect = this->rect(); // 获取自定义小部件的矩形区域
    QPoint point = rect.topLeft(); // 获取矩形区域的左上角坐标
    painter.drawRect(QRect(point, rect.size())); // 绘制一个蓝色矩形边框，以左下角为原点
    }

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

    // 绘制当前值
    painter.drawText(textRect, Qt::AlignCenter, valueText);
}

void DraggableDial::drawTunerKnob(QPainter *painter)
{
    int radius = 80; // 调音旋钮的半径
    painter->save();

    // 绘制调音旋钮的底座
    QBrush brush(QColor(100, 100, 100)); // 默认颜色
    painter->setPen(Qt::NoPen);
    painter->setBrush(brush); // 底座的颜色
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
    painter->rotate((value() - minimum())/(maximum() - minimum())* 360.0); // 根据当前值旋转指针
    painter->drawPolygon(QPolygonF() << QPointF(0, -radius + 20) << QPointF(-5, -radius + 35) << QPointF(5, -radius + 35));

    painter->restore();
}


void DraggableDial::copy(DraggableDial * dial){
    dial->name=name;
    dial->setMatchId(matchId);
    dial->setFixedSize(size());
    dial->setStyleSheet(styleSheet());
    dial->savePath=savePath;
    dial->imagePath=imagePath;
    dial->backsavePath=backsavePath;
    dial->backimagePath=backimagePath;
    dial->setMaximum(maximum());
    dial->setMinimum(minimum());
    dial->setSingleStep(this->singleStep());

    connect(dial, &DraggableDial::clicked,WidgetHelper::mainwindow->cp, &controlproperty::InitTree);
    connect(WidgetHelper::mainwindow->cp, &controlproperty::editfinish, dial, &DraggableDial::changeproperty);
    connect(dial,&DraggableDial::clicked,dial,&DraggableDial::clicktree);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::image,dial,&DraggableDial::changimage);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::backgroundimage,dial,&DraggableDial::changbackgroundimage);
    generatedDials.append(dial);
}






