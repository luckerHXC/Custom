#include "controlproperty.h"
#include "draggablebutton.h"
#include <QDebug>
#include <QLayout>
#include <QMenu>
#include <QtXml>
#include <QSlider>
#include <QScrollBar>
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QThread>
#include <QPushButton>
#include <QCoreApplication>
#include <QMenuBar>
#include <QMenu>
#include <QMainWindow>
#include "ui_mainwindow.h"
#include "draggableslider.h"
/*控件属性框相关*/
QStandardItemModel* controlproperty::model;
QString controlproperty::newText;
QString controlproperty::firstColumnText;
controlproperty::controlproperty(MainWindow * parent)
{
    this->mainwindow=parent;
    model = new QStandardItemModel(mainwindow->ui->treeView);
    mainwindow->ui->treeView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
    connect(mainwindow->ui->treeView->itemDelegate(), &QAbstractItemDelegate::closeEditor, this, &controlproperty::handleTreeViewEditingFinished);



}

//把控件的数据放到属性框上
void controlproperty::InitTree()
{
    // 给 QTreeView 应用 model
    mainwindow->ui->treeView->setModel(model);
    // 获取水平标头
    QHeaderView *horizontalHeader = mainwindow->ui->treeView->header();
    if (horizontalHeader) {
        // 使用样式表设置水平标头的样式
        horizontalHeader->setStyleSheet("QHeaderView::section { background-color: lightgrey; color: black; }");
    }
    QModelIndex parentIndex = model->index(0, 0);
    QModelIndex parentIndex1 = model->index(1, 0);

    for (int row = 0; row < 20; ++row) {
        for (int col = 0; col < 20; ++col) {
            QModelIndex index = model->index(row, col,parentIndex );
              if (index.isValid()) {
                 QVariant data = index.data(Qt::DisplayRole);
                 QModelIndex newIndex = index.sibling(index.row(), index.column() + 1);
                 if (data.isValid() && data.toString() == "添加图片") {
                     image_button = new QPushButton("...");
                     image_button->setStyleSheet("color:white;");
                     image_button->setFixedSize(40, 20);
                     // 将按钮添加到属性树
                     mainwindow->ui->treeView->setIndexWidget(newIndex, image_button);
                     // 连接按钮的点击信号到槽函数
                     connect(image_button, &QPushButton::clicked, this, &controlproperty::changimage);
                 }else if(data.isValid() && data.toString() == "添加背景图片"){
                     imagebackground_button = new QPushButton("...");
                     imagebackground_button->setStyleSheet("color:white;");
                     imagebackground_button->setFixedSize(40, 20);
                     // 将按钮添加到属性树
                     mainwindow->ui->treeView->setIndexWidget(newIndex, imagebackground_button);
                     // 连接按钮的点击信号到槽函数
                     connect(imagebackground_button, &QPushButton::clicked, this, &controlproperty::changbackgroundimage);
                 }else if(data.isValid() && data.toString() == "背景颜色"){
                     backgroundcolor_combobox = new ClickableCombobox();
                     backgroundcolor_combobox->setFixedSize(80, 20);
                     backgroundcolor_combobox->addItem(backgroundcolor);
                     QColor color(backgroundcolor);
                     QString styleSheet = QString("QComboBox { color: %1; } QComboBox QAbstractItemView { background-color: white; }").arg(color.name());
                     backgroundcolor_combobox->setStyleSheet(styleSheet);
                     connect(backgroundcolor_combobox, &ClickableCombobox::clicked, this, &controlproperty::changbackgroundcolor);

                     // 将按钮添加到属性树
                     mainwindow->ui->treeView->setIndexWidget(newIndex, backgroundcolor_combobox);
                     // 连接按钮的点击信号到槽函数

                 }
             }
         }
     }
    for (int row = 0; row < 10; ++row) {
        for (int col = 0; col < 10; ++col) {
            QModelIndex index = model->index(row, col,parentIndex1 );
              if (index.isValid()) {
                 QVariant data = index.data(Qt::DisplayRole);
                 QModelIndex newIndex = index.sibling(index.row(), index.column() + 1);
                 if (data.isValid() && data.toString() == "滑槽颜色") {
                     color_button = new QPushButton("添加颜色");
                     color_button->setFixedSize(60, 20);
                     // 将按钮添加到属性树
                     mainwindow->ui->treeView->setIndexWidget(newIndex, color_button);
                     // 连接按钮的点击信号到槽函数
                     connect(color_button, &QPushButton::clicked, slider, &DraggableSlider::changcolor);
                 }
             }
         }
     }

}

//当编辑完毕后触发此函数，将数值还给控件以进行调整
void controlproperty::handleTreeViewEditingFinished(QWidget* editor, QAbstractItemDelegate::EndEditHint hint)
{
    Q_UNUSED(hint);

    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
    if (lineEdit){
       newText = lineEdit->text(); // 获取编辑后的文本
       QModelIndex index = mainwindow->ui->treeView->currentIndex();
       QModelIndex firstColumnIndex = index.sibling(index.row(), 0);
       //firstColumnText要还给控件的数值
       firstColumnText = firstColumnIndex.data(Qt::DisplayRole).toString();
       emit editfinish();
    }
}
//改变图片
void controlproperty::changimage(){
    //此信号会被generate和printcontroll里面控件接收
    emit image();
}
//改变背景图片
void controlproperty::changbackgroundimage(){
    //此信号会被generate和printcontroll里面控件接收
    emit backgroundimage();
}

void controlproperty::changbackgroundcolor(){
    // 创建一个调色板
    QColorDialog colorDialog;

    // 弹出调色板对话框
    QColor selectedColor = colorDialog.getColor();

    if (selectedColor.isValid())
    {
        QString colorStr = selectedColor.name();
        qDebug() << "选择的颜色：" << colorStr;
        QString setStyleSheet("background-color: " + colorStr + ";");
        QString currentStyleSheet = widget->styleSheet(); // 获取当前的样式表
        currentStyleSheet += "\n" + setStyleSheet; // 将新的样式表拼接到原来的样式表后面
        widget->setStyleSheet(currentStyleSheet);
    }
}

