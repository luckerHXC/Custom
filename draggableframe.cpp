#include "draggableframe.h"
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
#include <QAbstractButton>
QList<DraggableFrame*> DraggableFrame::generatedFrame;
DraggableFrame::DraggableFrame(QWidget* parent)
    :  QFrame(parent)
{
    setMouseTracking(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &DraggableFrame::customContextMenuRequested, this, &DraggableFrame::showContextMenu);
    setStyleSheet("QFrame { background-color: lightgray; border: 2px solid gray; }");
}
//鼠标点击
void DraggableFrame::mousePressEvent(QMouseEvent* event)
{
    WidgetHelper::mousePressEvent(event,this);
}
//鼠标移动
void DraggableFrame::mouseMoveEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseMoveEvent(event,this);
    }
}
//鼠标释放
void DraggableFrame::mouseReleaseEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseReleaseEvent(event,this);
    this->Tree();
    emit clicked();
    setIconSize(size());
    }
}
//移除
void DraggableFrame::remove(){
    generatedFrame.removeOne(this);
    deleteLater();
}
//呼出菜单
void DraggableFrame::showContextMenu(const QPoint& pos)
{
    WidgetHelper::enable=lock;
    WidgetHelper::showContextMenu(pos,this);
}
//加载图片
void DraggableFrame::addImage()
{
    imagePath = QFileDialog::getOpenFileName(this, tr("选择图片"), "", tr("图片文件 (*.png *.jpg *.jpeg)"));
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
           QIcon icon(pixmap);
           setIcon(icon);
           setIconSize(size()); // 设置图标大小为按钮大小
           this->setStyleSheet("QToolButton { border: none; }");
           this->borderstatus=0;
           qDebug()<<pixmap;
           savePath = "C:\\Users\\RDSE\\Documents\\Custom_data\\"+QString::number(imageCounter)+".jpg";  // 设置保存图片的路径和文件名
           if (pixmap.save(savePath)) {
               qDebug() << "图片保存成功：" << savePath;
               imageCounter++;
           } else {
               qDebug() << "图片保存失败：" << savePath;
           }
        }
           // 如果需要，你还可以调整图标的大小：
           // setIconSize(QSize(width, height));
    }
}
//设置图片保存路径
void DraggableFrame::setpath(QString imagepath){
    this->savePath=imagepath;
}

//控件放大
void DraggableFrame::wheelEvent(QWheelEvent* event){
    if(lock==true){
    WidgetHelper::wheelEvent(event,this);
    }
}
//将控件属性打印到树上
void DraggableFrame::Tree(){
    WidgetHelper::name=this->name;
    WidgetHelper::tree(this);
}
//通过属性列表改变值
void DraggableFrame::changeproperty(){
    if(lock==true){
    if(this->signalEmitted == true) {
    WidgetHelper::changeproperty(this);
    if(controlproperty::firstColumnText=="名字"){
        this->name=controlproperty::newText;
    }
  }
 }
}
//改变对象标记，决定属性表可以改变哪些控件
void DraggableFrame::clicktree() {
    WidgetHelper::clicktree();
    //signalEmitte标记为true才能触发changeproperty
    this->signalEmitted = true;
}
//把图片打印到控件上去
void DraggableFrame::paintEvent(QPaintEvent* event)
{
    QFrame::paintEvent(event);

    if (!icon.isNull())
    {
        QPainter painter(this);
        painter.drawPixmap(5, 5, icon.pixmap(iconSize));
    }

    QPainter painter(this);
    if(drawblue==true){
    painter.setPen(QPen(Qt::blue, 4)); // 蓝色边框，线宽为2像素
    QRect rect = this->rect(); // 获取自定义小部件的矩形区域
    QPoint point = rect.topLeft(); // 获取矩形区域的左上角坐标
    painter.drawRect(QRect(point, rect.size())); // 绘制一个蓝色矩形边框，以左下角为原点
    }
}
//设置图片
void DraggableFrame::setIcon(const QIcon& icon)
{
    this->icon = icon;
    update();
}
//设置图片大小
void DraggableFrame::setIconSize(const QSize& size)
{
    iconSize = size;
    update();
}

int DraggableFrame::getmatchID(){
    return this->matchId;
}

void DraggableFrame::setMatchId(int id) {
       matchId = id;
}

void DraggableFrame::changimage(){
    if(this->signalEmitted == true){
    addImage();
    }
}
//打印已有控件用此函数添加图片
void DraggableFrame::customcontrol(){
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        if (!pixmap.isNull()) {
           QIcon icon(pixmap);
           setIcon(icon);//设置图像（核心语句）
           setIconSize(size()); // 设置图标大小为按钮大小
           //this->setStyleSheet("QFrame { border: none; }");
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
        QString currentStyleSheet = control->styleSheet(); // 获取当前的样式表
        QString styleSheet = QString("QFrame {background-image: url(%1);}").arg(backsavePath);
        currentStyleSheet += "\n" + styleSheet;
        this->setStyleSheet(currentStyleSheet);
    }

    if(control){
        QString currentStyleSheet = control->styleSheet(); // 获取当前的样式表
        QString styleSheet = QString("QFrame {background-image: url(%1);}").arg(backsavePath);
        currentStyleSheet += "\n" + styleSheet;
        control->setStyleSheet(currentStyleSheet);
    }

}
//属性编辑框的更改背景图片的信号发射后会由此函数接收
void DraggableFrame::changbackgroundimage(){
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
        QString styleSheet = QString("QFrame {background-image: url(%1);}").arg(backsavePath);
        //currentStyleSheet += "\n" + styleSheet; // 将新的样式表拼接到原来的样式表后面
        this->setStyleSheet(styleSheet);
    }

  }

}

void DraggableFrame::copy(DraggableFrame *Frame){
    Frame->name=name;
    Frame->setMatchId(matchId);
    Frame->setFixedSize(size());
    Frame->setStyleSheet(styleSheet());
    Frame->savePath=savePath;
    Frame->imagePath=imagePath;
    Frame->backsavePath=backsavePath;
    Frame->backimagePath=backimagePath;
    Frame->control=nullptr;
    Frame->customcontrol();

    connect(Frame, &DraggableFrame::clicked,WidgetHelper::mainwindow->cp, &controlproperty::InitTree);
    connect(WidgetHelper::mainwindow->cp, &controlproperty::editfinish, Frame, &DraggableFrame::changeproperty);
    connect(Frame,&DraggableFrame::clicked,Frame,&DraggableFrame::clicktree);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::image,Frame,&DraggableFrame::changimage);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::backgroundimage,Frame,&DraggableFrame::changbackgroundimage);
    generatedFrame.append(Frame);
}
