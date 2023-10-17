#include "draggablecheckbox.h"
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
QList<DraggableCheckBox*> DraggableCheckBox::generatedCheckBox;
DraggableCheckBox::DraggableCheckBox(QWidget* parent)
    : QCheckBox(parent)
{
    setMouseTracking(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &DraggableCheckBox::customContextMenuRequested, this, &DraggableCheckBox::showContextMenu);
    setStyleSheet("color: white;background-color: transparent;");
}
//鼠标点击
void DraggableCheckBox::mousePressEvent(QMouseEvent* event)
{
    WidgetHelper::mousePressEvent(event,this);
}
//鼠标移动
void DraggableCheckBox::mouseMoveEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseMoveEvent(event,this);
    }
}
//鼠标释放
void DraggableCheckBox::mouseReleaseEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseReleaseEvent(event,this);
    this->Tree();
    emit clicked();
    setIconSize(size()); // 设置图标大小为按钮大小
    }
}
//移除
void DraggableCheckBox::remove(){
    generatedCheckBox.removeOne(this);
    deleteLater();
}

//呼出菜单
void DraggableCheckBox::showContextMenu(const QPoint& pos)
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
void DraggableCheckBox::wheelEvent(QWheelEvent* event){
    if(WidgetHelper::enable==true){
    WidgetHelper::wheelEvent(event,this);
    }
}
//将控件属性打印到树上
void DraggableCheckBox::Tree(){
    WidgetHelper::Text=this->text();
    WidgetHelper::name=this->name;
    WidgetHelper::tree(this);
}
//通过属性列表改变值
void DraggableCheckBox::changeproperty(){
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
void DraggableCheckBox::clicktree() {
    WidgetHelper::clicktree();
    //signalEmitte标记为true才能触发changeproperty
    this->signalEmitted = true;
}

int DraggableCheckBox::getmatchID(){
    return this->matchId;
}

void DraggableCheckBox::setMatchId(int id) {
       matchId = id;
}
//属性编辑框的更改图片的信号发射后会由此函数接收
void DraggableCheckBox::changimage(){
    if(this->signalEmitted == true){
    if(lock == true){
        imagePath = QFileDialog::getOpenFileName(this, tr("选择图片"), "", tr("图片文件 (*.png *.jpg *.jpeg)"));
        if (!imagePath.isEmpty()) {
            QPixmap pixmap(imagePath);
            if (!pixmap.isNull()) {
               QIcon icon(pixmap);
               setIcon(icon);//设置图像（核心语句）
               setIconSize(size()); // 设置图标大小为按钮大小
               this->setStyleSheet("QCheckBox { border: none; }");
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

}
//打印已有控件用此函数添加图片
void DraggableCheckBox::customcontrol(){
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
           QIcon icon(pixmap);
           setIcon(icon);//设置图像（核心语句）
           setIconSize(size()); // 设置图标大小为按钮大小
           this->setStyleSheet("QCheckBox { border: none; }");
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
        QString styleSheet = QString("QCheckBox {background-image: url(%1);}").arg(backsavePath);
        this->setStyleSheet(styleSheet);
    }

    if(control){
    QPixmap pixmap(imagePath);
    QIcon icon(pixmap);
    control->setIcon(icon);
    control->setIconSize(control->size()); // 设置图标大小为按钮大小
      if (!backsavePath.isEmpty()){
      QString styleSheet = QString("QCheckBox {background-image: url(%1);}").arg(backsavePath);
      control->setStyleSheet(styleSheet);
      }
    }
}
//属性编辑框的更改背景图片的信号发射后会由此函数接收
void DraggableCheckBox::changbackgroundimage(){
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
        QString styleSheet = QString("QCheckBox {background-image: url(%1);}").arg(backsavePath);
        //currentStyleSheet += "\n" + styleSheet; // 将新的样式表拼接到原来的样式表后面
        this->setStyleSheet(styleSheet);
    }
  }
}

void DraggableCheckBox::copy(DraggableCheckBox *checkbox){
    checkbox->setText(text());
    checkbox->matchId=matchId;
    checkbox->setFixedSize(size());
    checkbox->setStyleSheet(styleSheet());
    checkbox->setFont(font());
    checkbox->savePath=savePath;
    checkbox->imagePath=imagePath;
    checkbox->backsavePath=backimagePath;
    checkbox->backimagePath=backimagePath;
    checkbox->control=nullptr;

    checkbox->customcontrol();
    connect(checkbox, &DraggableCheckBox::clicked,WidgetHelper::mainwindow->cp, &controlproperty::InitTree);
    connect(WidgetHelper::mainwindow->cp, &controlproperty::editfinish, checkbox, &DraggableCheckBox::changeproperty);
    connect(checkbox,&DraggableCheckBox::clicked,checkbox,&DraggableCheckBox::clicktree);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::image,checkbox,&DraggableCheckBox::changimage);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::backgroundimage,checkbox,&DraggableCheckBox::changbackgroundimage);
    generatedCheckBox.append(checkbox);
}

void DraggableCheckBox::paintEvent(QPaintEvent* event)
{
    QCheckBox::paintEvent(event);

    QPainter painter(this);
    if(drawblue==true){
    painter.setPen(QPen(Qt::blue, 4)); // 蓝色边框，线宽为2像素
    QRect rect = this->rect(); // 获取自定义小部件的矩形区域
    QPoint point = rect.topLeft(); // 获取矩形区域的左上角坐标
    painter.drawRect(QRect(point, rect.size())); // 绘制一个蓝色矩形边框，以左下角为原点
    }
}
