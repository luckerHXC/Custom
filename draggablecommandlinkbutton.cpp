#include "draggablecommandlinkbutton.h"
#include "draggablebutton.h"
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
#include "generate.h"
QList<DraggableCommandLinkButton*> DraggableCommandLinkButton::generatedCommandLinkButton;
DraggableCommandLinkButton::DraggableCommandLinkButton(QWidget* parent)
  : QCommandLinkButton(parent)
{
    setMouseTracking(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &DraggableCommandLinkButton::customContextMenuRequested, this, &DraggableCommandLinkButton::showContextMenu);
}
//鼠标点击
void DraggableCommandLinkButton::mousePressEvent(QMouseEvent* event)
{
    WidgetHelper::mousePressEvent(event,this);
}
//鼠标移动
void DraggableCommandLinkButton::mouseMoveEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseMoveEvent(event,this);
    }
}
//鼠标释放
void DraggableCommandLinkButton::mouseReleaseEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseReleaseEvent(event,this);
    this->Tree();
    emit clicked();
    setIconSize(size());
    }
}
//移除
void DraggableCommandLinkButton::remove(){
    generatedCommandLinkButton.removeOne(this);
    deleteLater();
}
//呼出菜单
void DraggableCommandLinkButton::showContextMenu(const QPoint& pos)
{
    WidgetHelper::showContextMenu(pos,this);
    if(WidgetHelper::delet==true){
        remove();
        WidgetHelper::delet=false;
    }
    if(WidgetHelper::enable==false){
        lock=false;
    }else if(WidgetHelper::enable==true){
        lock=true;
    }
}
//控件放大
void DraggableCommandLinkButton::wheelEvent(QWheelEvent* event){
    if(lock==true){
    WidgetHelper::wheelEvent(event,this);
    }
}
//将控件属性打印到树上
void DraggableCommandLinkButton::Tree(){
    WidgetHelper::Text=this->text();
    WidgetHelper::name=this->name;
    WidgetHelper::tree(this);
}
//通过属性列表改变值
void DraggableCommandLinkButton::changeproperty(){
    if(lock==true){
    if(this->signalEmitted == true) {
    WidgetHelper::changeproperty(this);
    if(controlproperty::firstColumnText=="文本"){
        this->setText(controlproperty::newText);
    }
    if(controlproperty::firstColumnText=="名字"){
        this->name=controlproperty::newText;
    }
   }
 }

}

//改变对象标记，决定属性表可以改变哪些控件
void DraggableCommandLinkButton::clicktree() {
    WidgetHelper::clicktree();
    //signalEmitte标记为true才能触发changeproperty
    this->signalEmitted = true;
}

int DraggableCommandLinkButton::getmatchID(){
    return this->matchId;
}

void DraggableCommandLinkButton::setMatchId(int id) {
    matchId = id;
}

void DraggableCommandLinkButton::changecolor(){
    qDebug()<<"";
}
//属性编辑框的更改图片的信号发射后会由此函数接收
void DraggableCommandLinkButton::changimage(){
    if(this->signalEmitted == true){
        imagePath = QFileDialog::getOpenFileName(this, tr("选择图片"), "", tr("图片文件 (*.png *.jpg *.jpeg)"));
        if (!imagePath.isEmpty()) {
            QPixmap pixmap(imagePath);
            if (!pixmap.isNull()) {
               QIcon icon(pixmap);
               setIcon(icon);//设置图像（核心语句）
               setIconSize(size()); // 设置图标大小为按钮大小
               this->setStyleSheet("QPushbutton { border: none; }");
               qDebug()<<pixmap;
               savePath = "D:\\Custom\\"+QString::number(imageCounter)+".jpg";  // 设置保存图片的路径和文件名
               if (pixmap.save(savePath)) {
                   qDebug() << "图片保存成功：" << savePath;
               } else {
                   qDebug() << "图片保存失败：" << savePath;
               }

            }

        }
    }

}
//打印已有控件用此函数添加图片
void DraggableCommandLinkButton::customcontrol(){
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
           QIcon icon(pixmap);
           setIcon(icon);//设置图像（核心语句）
           setIconSize(size()); // 设置图标大小为按钮大小
           this->setStyleSheet("QCommandLinkButton { border: none; }");
           qDebug()<<pixmap;
           savePath = "D:\\Custom\\"+QString::number(imageCounter)+".jpg";  // 设置保存图片的路径和文件名
           if (pixmap.save(savePath)) {
               qDebug() << "图片保存成功：" << savePath;
           } else {
               qDebug() << "图片保存失败：" << savePath;
           }

        }

    }
    if (!backsavePath.isEmpty()) {
        QString styleSheet = QString("QCommandLinkButton {background-image: url(%1);}").arg(backsavePath);
        this->setStyleSheet(styleSheet);
    }
    if(control){
    if(!imagePath.isEmpty()){
    QPixmap pixmap(imagePath);
    QIcon icon(pixmap);
    control->setIcon(icon);
    control->setIconSize(control->size()); // 设置图标大小为按钮大小
    }

    if(!backsavePath.isEmpty()){
    QString styleSheet = QString("QCommandLinkButton {background-image: url(%1);}").arg(backsavePath);
    control->setStyleSheet(styleSheet);
    }
  }
}
//属性编辑框的更改背景图片的信号发射后会由此函数接收
void DraggableCommandLinkButton::changbackgroundimage(){
    if(this->signalEmitted == true){
    backimagePath = QFileDialog::getOpenFileName(this, tr("选择图片"), "", tr("图片文件 (*.png *.jpg *.jpeg)"));
    if (!backimagePath.isEmpty()) {
        QPixmap pixmap(backimagePath);
        backsavePath = "D://Custom//"+QString::number(imageCounter+1)+".jpg";  // 设置保存图片的路径和文件名
        if (pixmap.save(backsavePath)) {
            qDebug() << "图片保存成功：" << backsavePath;
        } else {
            qDebug() << "图片保存失败：" << backsavePath;
        }
        //QString currentStyleSheet = this->styleSheet(); // 获取当前的样式表
        QString styleSheet = QString("QCommandLinkButton{background-image: url(%1);}").arg(backsavePath);
        //currentStyleSheet += "\n" + styleSheet; // 将新的样式表拼接到原来的样式表后面
        this->setStyleSheet(styleSheet);
    }
  }
}

void DraggableCommandLinkButton::copy(DraggableCommandLinkButton * commandlinkbutton){
    commandlinkbutton->setText(text());
    commandlinkbutton->matchId=matchId;
    commandlinkbutton->setFixedSize(size());
    commandlinkbutton->setStyleSheet(styleSheet());
    commandlinkbutton->setFont(font());
    commandlinkbutton->savePath=savePath;
    commandlinkbutton->imagePath=imagePath;
    commandlinkbutton->backsavePath=backimagePath;
    commandlinkbutton->backimagePath=backimagePath;

    commandlinkbutton->customcontrol();
    connect(commandlinkbutton, &DraggableCommandLinkButton::clicked,WidgetHelper::mainwindow->cp, &controlproperty::InitTree);
    connect(WidgetHelper::mainwindow->cp, &controlproperty::editfinish, commandlinkbutton, &DraggableCommandLinkButton::changeproperty);
    connect(commandlinkbutton,&DraggableCommandLinkButton::clicked,commandlinkbutton,&DraggableCommandLinkButton::clicktree);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::image,commandlinkbutton,&DraggableCommandLinkButton::changimage);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::backgroundimage,commandlinkbutton,&DraggableCommandLinkButton::changbackgroundimage);
    generatedCommandLinkButton.append(commandlinkbutton);

}

void DraggableCommandLinkButton::paintEvent(QPaintEvent* event)
{
    QCommandLinkButton::paintEvent(event);

    QPainter painter(this);
    if(drawblue==true){
    painter.setPen(QPen(Qt::blue, 4)); // 蓝色边框，线宽为2像素
    QRect rect = this->rect(); // 获取自定义小部件的矩形区域
    QPoint point = rect.topLeft(); // 获取矩形区域的左上角坐标
    painter.drawRect(QRect(point, rect.size())); // 绘制一个蓝色矩形边框，以左下角为原点
    }
}
