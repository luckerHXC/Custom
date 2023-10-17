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
#include "connectsl.h"
QList<DraggableLineEdit*> DraggableLineEdit::generatedLineEdit;
DraggableLineEdit::DraggableLineEdit(QWidget * parent) : QLineEdit(parent)
{
    setMouseTracking(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &DraggableLineEdit::customContextMenuRequested, this, &DraggableLineEdit::showContextMenu);
    this->setText("");
}
//鼠标点击
void DraggableLineEdit::mousePressEvent(QMouseEvent* event)
{
    WidgetHelper::mousePressEvent(event,this);
}
//鼠标移动
void DraggableLineEdit::mouseMoveEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseMoveEvent(event,this);
    }
}
//鼠标释放
void DraggableLineEdit::mouseReleaseEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseReleaseEvent(event,this);
    this->Tree();
    emit clicked();
    }
}
//移除
void DraggableLineEdit::remove(){
    generatedLineEdit.removeOne(this);
    deleteLater();
}

//呼出菜单
void DraggableLineEdit::showContextMenu(const QPoint& pos)
{
    WidgetHelper::showContextMenu(pos,this);
    if(WidgetHelper::delet==true){
        remove();
        WidgetHelper::delet=false;
    }
    if(WidgetHelper::enable==false){
        lock=false;
        this->setReadOnly(true);
    }else if(WidgetHelper::enable==true){
        lock=true;
        this->setReadOnly(false);
    }
}

//控件放大
void DraggableLineEdit::wheelEvent(QWheelEvent* event){
    if(lock==true){
    WidgetHelper::wheelEvent(event,this);
    }
}
//将控件属性打印到树上
void DraggableLineEdit::Tree(){
    WidgetHelper::Text=this->text();
    WidgetHelper::name=this->name;
    WidgetHelper::tree(this);
}
//通过属性列表改变值
void DraggableLineEdit::changeproperty(){
    if(lock==true){
    if(this->signalEmitted == true){
    WidgetHelper::changeproperty(this);
    if(controlproperty::firstColumnText=="名字"){
        this->name=controlproperty::newText;
    }
  }
 }
}
//改变对象标记，决定属性表可以改变哪些控件
void DraggableLineEdit::clicktree() {
    WidgetHelper::clicktree();
    //signalEmitte标记为true才能触发changeproperty
    this->signalEmitted = true;
}
//设置匹配id
void DraggableLineEdit::setMatchId(int id) {
    matchId = id;
}
//获得唯一id
int DraggableLineEdit::getmatchID(){
    return this->matchId;
}
//改变图片
void DraggableLineEdit::changimage(){
    if(this->signalEmitted == true){
    qDebug()<<"yata";
    }
}
//打印已有控件用此函数添加图片
void DraggableLineEdit::customcontrol(){

    if (!backsavePath.isEmpty()) {
        QString styleSheet = QString("QLineEdit {background-image: url(%1);}").arg(backsavePath);
        this->setStyleSheet(styleSheet);
    }

    if(lineEdit){
        QString currentStyleSheet = lineEdit->styleSheet(); // 获取当前的样式表
        QString styleSheet = QString("QLineEdit {background-image: url(%1);}").arg(backsavePath);
        currentStyleSheet += "\n" + styleSheet;
        lineEdit->setStyleSheet(currentStyleSheet);
    }
}
//属性编辑框的更改背景图片的信号发射后会由此函数接收
void DraggableLineEdit::changbackgroundimage(){
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
        QString currentStyleSheet = this->styleSheet(); // 获取当前的样式表
        QString styleSheet = QString("QLineEdit {background-image: url(%1);}").arg(backsavePath);
        currentStyleSheet += "\n" + styleSheet; // 将新的样式表拼接到原来的样式表后面
        this->setStyleSheet(currentStyleSheet);
    }
  }
}

void DraggableLineEdit::copy(DraggableLineEdit *LineEdit){
    LineEdit->setText(text());
    LineEdit->matchId=matchId;
    LineEdit->setFixedSize(size());
    LineEdit->setStyleSheet(styleSheet());
    LineEdit->setFont(font());
    LineEdit->savePath=savePath;
    LineEdit->imagePath=imagePath;
    LineEdit->backsavePath=backimagePath;
    LineEdit->backimagePath=backimagePath;

    connect(LineEdit, &DraggableLineEdit::clicked,WidgetHelper::mainwindow->cp, &controlproperty::InitTree);
    connect(WidgetHelper::mainwindow->cp, &controlproperty::editfinish, LineEdit, &DraggableLineEdit::changeproperty);
    connect(LineEdit,&DraggableLineEdit::clicked,LineEdit,&DraggableLineEdit::clicktree);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::image,LineEdit,&DraggableLineEdit::changimage);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::backgroundimage,LineEdit,&DraggableLineEdit::changbackgroundimage);
    generatedLineEdit.append(LineEdit);
}

void DraggableLineEdit::paintEvent(QPaintEvent* event)
{
    QLineEdit::paintEvent(event);

    QPainter painter(this);
    if(drawblue==true){
    painter.setPen(QPen(Qt::blue, 4)); // 蓝色边框，线宽为2像素
    QRect rect = this->rect(); // 获取自定义小部件的矩形区域
    QPoint point = rect.topLeft(); // 获取矩形区域的左上角坐标
    painter.drawRect(QRect(point, rect.size())); // 绘制一个蓝色矩形边框，以左下角为原点
    }
}
