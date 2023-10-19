#include "draggabletoolbutton.h"
#include <QLayout>
#include <QMenu>
#include <QDrag>
#include <QApplication>
#include <QPainter>
#include <QFileDialog>
#include "widgethelper.h"
#include "controlproperty.h"
#include "QIcon"
QList<DraggableToolButton*> DraggableToolButton::generatedToolButton;
DraggableToolButton::DraggableToolButton(QWidget* parent)
    :  QToolButton(parent)
{
    setMouseTracking(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &DraggableToolButton::customContextMenuRequested, this, &DraggableToolButton::showContextMenu);
}
//鼠标点击
void DraggableToolButton::mousePressEvent(QMouseEvent* event)
{
    WidgetHelper::mousePressEvent(event,this);
}
//鼠标移动
void DraggableToolButton::mouseMoveEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseMoveEvent(event,this);
    }
}
//鼠标释放
void DraggableToolButton::mouseReleaseEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseReleaseEvent(event,this);
    this->Tree();
    emit clicked();
    setIconSize(size());
    }
}
//移除
void DraggableToolButton::remove(){
    generatedToolButton.removeOne(this);
    deleteLater();
}
//呼出菜单
void DraggableToolButton::showContextMenu(const QPoint& pos)
{
    WidgetHelper::enable=lock;
    WidgetHelper::showContextMenu(pos,this);
}
//加载图片
void DraggableToolButton::addImage()
{
    imagePath = QFileDialog::getOpenFileName(this, tr("选择图片"), "", tr("图片文件 (*.png *.jpg *.jpeg)"));
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
           QIcon icon(pixmap);
           setIcon(icon);//设置图像（核心语句）
           setIconSize(size()); // 设置图标大小为按钮大小
           this->setStyleSheet("QToolButton { border: none; }");
           this->borderstatus=0;
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
//设置图片路径
void DraggableToolButton::setpath(QString imagepath){
    this->savePath=imagepath;
}

//控件放大
void DraggableToolButton::wheelEvent(QWheelEvent* event){
    if(lock==true){
    WidgetHelper::wheelEvent(event,this);
    }
}
//将控件属性打印到树上
void DraggableToolButton::Tree(){
    WidgetHelper::name=this->name;
    WidgetHelper::tree(this);
}
//通过属性列表改变值
void DraggableToolButton::changeproperty(){
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
void DraggableToolButton::clicktree() {
    WidgetHelper::clicktree();
    //signalEmitte标记为true才能触发changeproperty
    this->signalEmitted = true;
}

int DraggableToolButton::getmatchID(){
    return this->matchId;
}

void DraggableToolButton::setMatchId(int id) {
       matchId = id;
}
//占位
void DraggableToolButton::changimage(){
    if(this->signalEmitted == true){
    addImage();
    }
}
//占位
void DraggableToolButton::customcontrol(){
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
           QIcon icon(pixmap);
           setIcon(icon);//设置图像（核心语句）
           setIconSize(size()); // 设置图标大小为按钮大小
           //this->setStyleSheet("QToolButton { border: none; }");
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
        QString styleSheet = QString("QToolButton {background-image: url(%1);}").arg(backsavePath);
        QString currentStyleSheet = this->styleSheet(); // 获取当前的样式表
        currentStyleSheet += "\n" + styleSheet;
        this->setStyleSheet(currentStyleSheet);
    }

    if(control){
        QString styleSheet = QString("QToolButton {background-image: url(%1);}").arg(backsavePath);
        QString currentStyleSheet = control->styleSheet(); // 获取当前的样式表
        currentStyleSheet += "\n" + styleSheet;
        control->setStyleSheet(currentStyleSheet);
    }
}
//占位
void DraggableToolButton::changbackgroundimage(){
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
            QString styleSheet = QString("QToolButton {background-image: url(%1);}").arg(backsavePath);
            //currentStyleSheet += "\n" + styleSheet; // 将新的样式表拼接到原来的样式表后面
            this->setStyleSheet(styleSheet);
        }
    }
}

void DraggableToolButton::copy(DraggableToolButton *toolbutton){
    toolbutton->name=name;
    toolbutton->setMatchId(matchId);
    toolbutton->setFixedSize(size());
    toolbutton->setStyleSheet(styleSheet());
    toolbutton->savePath=savePath;
    toolbutton->imagePath=imagePath;
    toolbutton->backsavePath=backsavePath;
    toolbutton->backimagePath=backimagePath;
    toolbutton->customcontrol();

    connect(toolbutton, &DraggableToolButton::clicked,WidgetHelper::mainwindow->cp, &controlproperty::InitTree);
    connect(WidgetHelper::mainwindow->cp, &controlproperty::editfinish, toolbutton, &DraggableToolButton::changeproperty);
    connect(toolbutton,&DraggableToolButton::clicked,toolbutton,&DraggableToolButton::clicktree);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::image,toolbutton,&DraggableToolButton::changimage);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::backgroundimage,toolbutton,&DraggableToolButton::changbackgroundimage);
    generatedToolButton.append(toolbutton);
}

void DraggableToolButton::paintEvent(QPaintEvent* event)
{
    QToolButton::paintEvent(event);

    QPainter painter(this);
    if(drawblue==true){
    painter.setPen(QPen(Qt::blue, 4)); // 蓝色边框，线宽为2像素
    QRect rect = this->rect(); // 获取自定义小部件的矩形区域
    QPoint point = rect.topLeft(); // 获取矩形区域的左上角坐标
    painter.drawRect(QRect(point, rect.size())); // 绘制一个蓝色矩形边框，以左下角为原点
    }
}
