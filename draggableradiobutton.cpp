#include "draggablecheckbox.h"
#include "draggableradiobutton.h"
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
QList<DraggableRadioButton*> DraggableRadioButton::generateRadioButton;
DraggableRadioButton::DraggableRadioButton(QWidget* parent)
    : QRadioButton(parent)
{
    setMouseTracking(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &DraggableRadioButton::customContextMenuRequested, this, &DraggableRadioButton::showContextMenu);
    setStyleSheet("color: white;background-color: transparent;");
}
//鼠标点击
void DraggableRadioButton::mousePressEvent(QMouseEvent* event)
{
    WidgetHelper::mousePressEvent(event,this);
}
//鼠标移动
void DraggableRadioButton::mouseMoveEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseMoveEvent(event,this);
    }
}
//鼠标释放
void DraggableRadioButton::mouseReleaseEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseReleaseEvent(event,this);
    this->Tree();
    emit clicked();
    setIconSize(size()); // 设置图标大小为按钮大小
    }
}
//移除（此函数暂时没使用)
void DraggableRadioButton::remove(){
    generateRadioButton.removeOne(this);
    deleteLater();
}

//呼出菜单
void DraggableRadioButton::showContextMenu(const QPoint& pos)
{
    WidgetHelper::enable=lock;
    WidgetHelper::showContextMenu(pos,this);
}

//控件放大
void DraggableRadioButton::wheelEvent(QWheelEvent* event){
    if(lock==true){
    WidgetHelper::wheelEvent(event,this);
    }
}
//将控件属性打印到树上
void DraggableRadioButton::Tree(){
    WidgetHelper::Text=this->text();
    WidgetHelper::name=this->name;
    WidgetHelper::tree(this);
}
//通过属性列表改变值
void DraggableRadioButton::changeproperty(){
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
void DraggableRadioButton::clicktree() {
    WidgetHelper::clicktree();
    //signalEmitte标记为true才能触发changeproperty
    this->signalEmitted = true;
}

int DraggableRadioButton::getmatchID(){
    return this->matchId;
}

void DraggableRadioButton::setMatchId(int id) {
       matchId = id;
}
//属性编辑框的更改图片的信号发射后会由此函数接收
void DraggableRadioButton::changimage(){
    if(this->signalEmitted == true){
        imagePath = QFileDialog::getOpenFileName(this, tr("选择图片"), "", tr("图片文件 (*.png *.jpg *.jpeg)"));
        if (!imagePath.isEmpty()) {
            QPixmap pixmap(imagePath);
            if (!pixmap.isNull()) {
               QIcon icon(pixmap);
               setIcon(icon);//设置图像（核心语句）
               setIconSize(size()); // 设置图标大小为按钮大小
               this->setStyleSheet("QRadioButton { border: none; }");
               qDebug()<<pixmap;
               savePath = "C:\\Users\\RDSE\\Documents\\Custom_data\\"+QString::number(imageCounter)+".jpg";  // 设置保存图片的路径和文件名
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
void DraggableRadioButton::customcontrol(){
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
           QIcon icon(pixmap);
           setIcon(icon);//设置图像（核心语句）
           setIconSize(size()); // 设置图标大小为按钮大小
           this->setStyleSheet("QCheckBox { border: none; }");
           qDebug()<<pixmap;
           savePath = "C:\\Users\\RDSE\\Documents\\Custom_data\\"+QString::number(imageCounter)+".jpg";  // 设置保存图片的路径和文件名
           if (pixmap.save(savePath)) {
               qDebug() << "图片保存成功：" << savePath;
           } else {
               qDebug() << "图片保存失败：" << savePath;
           }

        }

    }
    if (!backsavePath.isEmpty()) {
        QString styleSheet = QString("QRadioButton {background-image: url(%1);}").arg(backsavePath);
        this->setStyleSheet(styleSheet);
    }
    if(control){
    QPixmap pixmap(imagePath);
    QIcon icon(pixmap);
    control->setIcon(icon);
    control->setIconSize(control->size()); // 设置图标大小为按钮大小
      if(!backsavePath.isEmpty()){
      QString styleSheet = QString("QRadioButton {background-image: url(%1);}").arg(backsavePath);
      control->setStyleSheet(styleSheet);
      }
    }
}
//属性编辑框的更改背景图片的信号发射后会由此函数接收
void DraggableRadioButton::changbackgroundimage(){
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
        QString styleSheet = QString("QRadioButton {background-image: url(%1);}").arg(backsavePath);
        //currentStyleSheet += "\n" + styleSheet; // 将新的样式表拼接到原来的样式表后面
        this->setStyleSheet(styleSheet);
    }

  }

}

void DraggableRadioButton::copy(DraggableRadioButton *radiobutton){
    radiobutton->setText(text());
    radiobutton->matchId=matchId;
    radiobutton->setFixedSize(size());
    radiobutton->setStyleSheet(styleSheet());
    radiobutton->setFont(font());
    radiobutton->savePath=savePath;
    radiobutton->imagePath=imagePath;
    radiobutton->backsavePath=backimagePath;
    radiobutton->backimagePath=backimagePath;
    radiobutton->control=nullptr;

    radiobutton->customcontrol();
    connect(radiobutton, &DraggableRadioButton::clicked,WidgetHelper::mainwindow->cp, &controlproperty::InitTree);
    connect(WidgetHelper::mainwindow->cp, &controlproperty::editfinish,radiobutton, &DraggableRadioButton::changeproperty);
    connect(radiobutton,&DraggableRadioButton::clicked,radiobutton,&DraggableRadioButton::clicktree);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::image,radiobutton,&DraggableRadioButton::changimage);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::backgroundimage,radiobutton,&DraggableRadioButton::changbackgroundimage);
    generateRadioButton.append(radiobutton);
}

void DraggableRadioButton::paintEvent(QPaintEvent* event)
{
    QRadioButton::paintEvent(event);

    QPainter painter(this);
    if(drawblue==true){
    painter.setPen(QPen(Qt::blue, 4)); // 蓝色边框，线宽为2像素
    QRect rect = this->rect(); // 获取自定义小部件的矩形区域
    QPoint point = rect.topLeft(); // 获取矩形区域的左上角坐标
    painter.drawRect(QRect(point, rect.size())); // 绘制一个蓝色矩形边框，以左下角为原点
    }
}
