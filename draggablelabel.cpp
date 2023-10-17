#include "draggablelabel.h"
#include "mainwindow.h"
#include "controlproperty.h"
#include <QDebug>
#include <QLayout>
#include <QMenu>
#include <QDomDocument>
#include <QTimer>
#include <QApplication>
#include <QLabel>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QStandardItemModel>
#include <QMimeData>
#include <QDrag>
#include "widgethelper.h"
#include "QLineEdit"
QList<DraggableLabel*> DraggableLabel::generatedLabels;
DraggableLabel::DraggableLabel(QWidget* parent)
    : QLabel(parent)
{
    setMouseTracking(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &DraggableLabel::customContextMenuRequested, this, &DraggableLabel::showContextMenu);
    setStyleSheet("color: white;background-color: transparent;");
}
//鼠标点击
void DraggableLabel::mousePressEvent(QMouseEvent* event)
{
    WidgetHelper::mousePressEvent(event,this);
}
//鼠标移动
void DraggableLabel::mouseMoveEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseMoveEvent(event,this);
    }
}
//鼠标释放
void DraggableLabel::mouseReleaseEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseReleaseEvent(event,this);
    this->Tree();
    emit clicked();
    }
}
//移除（此函数暂时没使用)
void DraggableLabel::remove(){
    generatedLabels.removeOne(this);
    deleteLater();
}

//呼出菜单
void DraggableLabel::showContextMenu(const QPoint& pos)
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
void DraggableLabel::wheelEvent(QWheelEvent* event){
    if(lock==true){
    WidgetHelper::wheelEvent(event,this);
    }
}
//将控件属性打印到树上
void DraggableLabel::Tree(){
    WidgetHelper::Text=this->text();
    WidgetHelper::name=this->name;
    WidgetHelper::tree(this);
}
//通过属性列表改变值
void DraggableLabel::changeproperty(){
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
void DraggableLabel::clicktree() {
    WidgetHelper::clicktree();
    //signalEmitte标记为true才能触发changeproperty
    this->signalEmitted = true;
}
//可编辑标签
void DraggableLabel::setEditable(bool editable) {
    if (editable) {
        // 创建一个单行文本编辑框，将其设置为当前文本内容，并显示在标签的位置

        lineEdit = new QLineEdit(this);
        lineEdit->setText(text());
        lineEdit->setGeometry(rect());
        lineEdit->selectAll();
        lineEdit->setFocus();
        lineEdit->show();
        this->setText("");
        connect(lineEdit, &QLineEdit::editingFinished, this, &DraggableLabel::finishEditing);
        // 设置编辑框字体大小与编辑框大小一致
                QFont font = lineEdit->font();
                QFontMetrics fontMetrics(font);
                int fontSize = fontMetrics.height(); // 使用编辑框的高度作为字体大小
                font.setPointSize(fontSize);
                lineEdit->setFont(font);
    } else {
        // 退出编辑模式，获取编辑框的文本并更新标签的文本内容
        if (lineEdit) {
            setText(lineEdit->text());
            lineEdit->deleteLater();
            lineEdit = nullptr;
        }
    }
}
//双击鼠标进入编辑状态
void DraggableLabel::mouseDoubleClickEvent(QMouseEvent* event){
        if(lock==true){
        if (event->button() == Qt::LeftButton) {
            setEditable(true);  // 进入编辑模式
        }
       }
}
//完成编辑
void DraggableLabel::finishEditing()
{
        setEditable(false);  // 结束编辑模式
}

int DraggableLabel::getmatchID(){
    return this->matchId;
}

void DraggableLabel::setMatchId(int id) {
       matchId = id;
}

//占位
void DraggableLabel::changimage(){
    if(this->signalEmitted == true){
    }
}
//占位
void DraggableLabel::customcontrol(){

}
//占位
void DraggableLabel::changbackgroundimage(){
    if(this->signalEmitted == true){
  }
}

void DraggableLabel::copy(DraggableLabel *label){
    label->setText(text());
    label->matchId=matchId;
    label->setFixedSize(size());
    label->setStyleSheet(styleSheet());
    label->setFont(font());
    label->savePath=savePath;
    label->imagePath=imagePath;
    label->backsavePath=backimagePath;
    label->backimagePath=backimagePath;

    connect(label, &DraggableLabel::clicked,WidgetHelper::mainwindow->cp, &controlproperty::InitTree);
    connect(WidgetHelper::mainwindow->cp, &controlproperty::editfinish, label, &DraggableLabel::changeproperty);
    connect(label,&DraggableLabel::clicked,label,&DraggableLabel::clicktree);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::image,label,&DraggableLabel::changimage);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::backgroundimage,label,&DraggableLabel::changbackgroundimage);
    generatedLabels.append(label);
}

void DraggableLabel::paintEvent(QPaintEvent* event)
{
    QLabel::paintEvent(event);

    QPainter painter(this);
    if(drawblue==true){
    painter.setPen(QPen(Qt::blue, 4)); // 蓝色边框，线宽为2像素
    QRect rect = this->rect(); // 获取自定义小部件的矩形区域
    QPoint point = rect.topLeft(); // 获取矩形区域的左上角坐标
    painter.drawRect(QRect(point, rect.size())); // 绘制一个蓝色矩形边框，以左下角为原点
    }
}
