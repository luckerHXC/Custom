#include "mainwindow.h"
#include "ui_mainwindow.h"
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
#include "generate.h"
#include "savebutton.h"
#include "printcontroll.h"
#include "controlproperty.h"
#include "clickablelineedit.h"
#include "clickableframe.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //滚屏实现
    connect(ui->verticalScrollBar_2, SIGNAL(valueChanged(int)), this, SLOT(onScrollValueChanged(int)));

    WidgetHelper::mainwindow=this;//让widgethelper类可操作mainwindow的ui

    cp=new controlproperty(this);//控件属性展示与修改对象

    //把保存在xml上的数据读取并打印到界面上（程序启动默认打印)
    printcontroll *pt=new printcontroll(this);
    pt->printall();
    //点击保存按钮把设计界面更新到用户界面
    connect(ui->pushButton_5,&savebutton::clearcontrol, this, &MainWindow::restorecontrol);
    connect(ui->pushButton_5,&savebutton::updateclick, pt, &printcontroll::updatecontroll);

    //生成控件
    gen=new generate(this);
    connect(ui->frame,&ClickableFrame::released,gen,&generate::generateButton);//生成按钮
    connect(ui->frame_3,&ClickableFrame::released,gen,&generate::generateDial);//生成按钮
    connect(ui->frame_6,&ClickableFrame::released,gen,&generate::generateLabel);
    connect(ui->frame_8,&ClickableFrame::released,gen,&generate::generateLineEdit);
    connect(ui->frame_5,&ClickableFrame::released,gen,&generate::generateCheckBox);
    connect(ui->frame_4,&ClickableFrame::released,gen,&generate::generateRadioButton);
    connect(ui->frame_1,&ClickableFrame::released,gen,&generate::generatedhorizontalSlider);
    connect(ui->frame_2,&ClickableFrame::released,gen,&generate::generatedverticalSlider);
    connect(ui->frame_7,&ClickableFrame::released,gen,&generate::generatedToolButton);
    connect(ui->frame_9,&ClickableFrame::released,gen,&generate::generatedFrame);
    connect(ui->frame_10,&ClickableFrame::released,gen,&generate::generatedCommandLinkButton);
    connect(ui->frame_11,&ClickableFrame::released,gen,&generate::generatedVolumeBar);
    //切换用户界面禁用保存按钮
    connect(ui->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);

    // 设置事件过滤器，监听鼠标点击事件
    installEventFilter(this);

    //匹配
    matchSL();
    connect(ui->pushButton_5, &savebutton::updatematch,this, &MainWindow::matchSL);

    // 从文件中读取上次的nextMatchId值
    QFile file("match_id.txt");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString lastMatchId = in.readLine();
        nextMatchId = lastMatchId.toInt();
        file.close();
    }


    m_isDrawing = false; // 初始状态为未绘制

    // 在 tab_3 上安装事件过滤器
    ui->tab_3->installEventFilter(this);


    QString currentStyleSheet = ui->tabWidget->styleSheet(); // 获取当前的样式表
    // 使用样式表设置选项卡按钮的背景颜色和文本颜色
    QString styleSheet=QString("QTabBar::tab:selected { background-color: blue; color: white; }"
                               "QTabWidget::pane { border: 1px solid blue; }");
    currentStyleSheet += "\n" + styleSheet; // 将新的样式表拼接到原来的样式表后面
    ui->tabWidget->setStyleSheet(currentStyleSheet);

    ui->menuBar->setStyleSheet("#menuBar{background-image:url(:/image/grey.png);color:white;}");

    connect(ui->actionDelete, &QAction::triggered, this, [this]() {actionSelectedWidget(ui->actionDelete);});
    connect(ui->actionCopy, &QAction::triggered, this, [this]() {actionSelectedWidget(ui->actionCopy);});
    connect(ui->actionLock, &QAction::triggered, this, [this]() {actionSelectedWidget(ui->actionLock);});
    connect(ui->actionUnlock, &QAction::triggered, this, [this]() {actionSelectedWidget(ui->actionUnlock);});
    connect(ui->actionTop, &QAction::triggered, this, [this]() {actionSelectedWidget(ui->actionTop);});
    connect(ui->actionBottom, &QAction::triggered, this, [this]() {actionSelectedWidget(ui->actionBottom);});

}

MainWindow::~MainWindow()
{
    delete ui;
}
//切换界面禁用按钮
void MainWindow::onTabChanged(int index) {
    if (index == ui->tabWidget->indexOf(ui->tab_3)) {
        // 当切换到 tab_3 页面时，激活按钮
        ui->pushButton_5->setEnabled(true);
        connect(ui->frame,&ClickableFrame::released,gen,&generate::generateButton);//生成按钮
        connect(ui->frame_3,&ClickableFrame::released,gen,&generate::generateDial);//生成按钮
        connect(ui->frame_6,&ClickableFrame::released,gen,&generate::generateLabel);
        connect(ui->frame_8,&ClickableFrame::released,gen,&generate::generateLineEdit);
        connect(ui->frame_5,&ClickableFrame::released,gen,&generate::generateCheckBox);
        connect(ui->frame_4,&ClickableFrame::released,gen,&generate::generateRadioButton);
        connect(ui->frame_1,&ClickableFrame::released,gen,&generate::generatedhorizontalSlider);
        connect(ui->frame_2,&ClickableFrame::released,gen,&generate::generatedverticalSlider);
        connect(ui->frame_7,&ClickableFrame::released,gen,&generate::generatedToolButton);
        connect(ui->frame_9,&ClickableFrame::released,gen,&generate::generatedFrame);
        connect(ui->frame_10,&ClickableFrame::released,gen,&generate::generatedCommandLinkButton);
        connect(ui->frame_11,&ClickableFrame::released,gen,&generate::generatedVolumeBar);
    } else if (index == ui->tabWidget->indexOf(ui->tab_4)) {
        // 清除 QTreeView 内容
        QStandardItemModel* model = new QStandardItemModel();
        ui->treeView->setModel(model);

        // 通知 QTreeView 更新
        ui->treeView->update();
        // 当切换到 tab_4 页面时，失效按钮
        ui->pushButton_5->setEnabled(false);
        disconnect(ui->frame,&ClickableFrame::released,gen,&generate::generateButton);//生成按钮
        disconnect(ui->frame_3,&ClickableFrame::released,gen,&generate::generateDial);//生成按钮
        disconnect(ui->frame_6,&ClickableFrame::released,gen,&generate::generateLabel);
        disconnect(ui->frame_8,&ClickableFrame::released,gen,&generate::generateLineEdit);
        disconnect(ui->frame_5,&ClickableFrame::released,gen,&generate::generateCheckBox);
        disconnect(ui->frame_4,&ClickableFrame::released,gen,&generate::generateRadioButton);
        disconnect(ui->frame_1,&ClickableFrame::released,gen,&generate::generatedhorizontalSlider);
        disconnect(ui->frame_2,&ClickableFrame::released,gen,&generate::generatedverticalSlider);
        disconnect(ui->frame_7,&ClickableFrame::released,gen,&generate::generatedToolButton);
        disconnect(ui->frame_9,&ClickableFrame::released,gen,&generate::generatedFrame);
        disconnect(ui->frame_10,&ClickableFrame::released,gen,&generate::generatedCommandLinkButton);
        disconnect(ui->frame_11,&ClickableFrame::released,gen,&generate::generatedVolumeBar);
    }
}



//滚屏函数
void MainWindow::onScrollValueChanged(int value)
{
    // 获取滚动条的值范围
    int minValue = ui->verticalScrollBar_2->minimum();
    int maxValue = ui->verticalScrollBar_2->maximum();

    // 获取滚动内容的高度
    int contentHeight = ui->scrollContent_2->height();

    // 获取滚动区域的高度
    int scrollAreaHeight = ui->scrollArea_2->viewport()->height();

    // 计算滚动内容的垂直偏移量
    int verticalOffset = (contentHeight - scrollAreaHeight) * (value - minValue) / (maxValue - minValue);

    if (ui->scrollArea_2 && ui->scrollContent_2){
        // 执行滚动内容位置的更新操作
        ui->scrollContent_2->move(ui->scrollContent_2->x(), -verticalOffset);
    } else {
        qDebug() << "scrollArea 或 scrollContent 为空指针";
    }

}
//事件过滤，以下用于滑槽与编辑框的匹配以及框选控件边框变蓝
bool MainWindow::eventFilter(QObject* obj, QEvent* event){
      QWidget* widget = qobject_cast<QWidget*>(obj);

      if (obj != ui->tab_3 && dynamic_cast<MainWindow*>(obj) == nullptr) {
         QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
         if (event->type() == QEvent::Enter|| event->type() == QEvent::MouseMove) {
             if (QWidget* widget = qobject_cast<QWidget*>(obj)) {
                 // 鼠标进入控件，设置指针形状为 SizeAllCursor
                 setCursor(Qt::SizeAllCursor);
                 QPoint mousePos = widget->mapFromGlobal(QCursor::pos());
                 QRect widgetRect = widget->rect();
                 int cornerSize = 10; // 定义角的小范围大小
                 if (mousePos.x() <= cornerSize && mousePos.y() <= cornerSize) {
                     // 鼠标在左上角，设置指针形状为 SizeFDiagCursor
                     setCursor(Qt::SizeFDiagCursor);
                 } else if (mousePos.x() <= cornerSize && mousePos.y() >= widgetRect.height() - cornerSize) {
                     // 鼠标在左下角，设置指针形状为 SizeBDiagCursor
                     setCursor(Qt::SizeBDiagCursor);
                 } else if (mousePos.x() >= widgetRect.width() - cornerSize && mousePos.y() <= cornerSize) {
                     // 鼠标在右上角，设置指针形状为 SizeBDiagCursor
                     setCursor(Qt::SizeBDiagCursor);
                 } else if (mousePos.x() >= widgetRect.width() - cornerSize && mousePos.y() >= widgetRect.height() - cornerSize) {
                     // 鼠标在右下角的小范围内，设置指针形状为 SizeFDiagCursor
                     setCursor(Qt::SizeFDiagCursor);
                 }

              }
         }else if (event->type() == QEvent::Leave) {
             // 鼠标离开控件，恢复默认指针形状
             setCursor(Qt::ArrowCursor);
         }else if(event->type() == QEvent::MouseButtonPress){
             if(!m_selectedWidgets.contains(widget) && !(mouseEvent->modifiers() & Qt::ControlModifier)){
                 restorecontrol();
             }
             //以下部分为控件的点击框选功能
             if(obj!=ui->tab_3){
                  QWidget* widget = qobject_cast<QWidget*>(obj);
                  m_startPoint = widget->pos();//点位位置为初始移动位置
                  choicewidget=true;//拖动许可
                  //判断控件有没有被框选
                  if (!m_selectedWidgets.contains(widget)){
                      //如果没有按住ctrl点击没框选的控件，则要清除样式
                      if (!(mouseEvent->modifiers() & Qt::ControlModifier)){
                      m_selectedWidgets.clear();
                      }
                      m_selectedWidgets.append(widget);//把框起来的控件加载进去
                  }
             }
             if (mouseEvent && mouseEvent->button() == Qt::LeftButton) {
                 WidgetHelper::printblue(widget,true);
             }
          }
      }

      //以下为滑块和编辑框的匹配
       if (event->type() == QEvent::MouseButtonPress)
       {

           QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

           // 判断是否按下了Ctrl键
           if (mouseEvent->modifiers() & Qt::ControlModifier)
           {

               if (obj->inherits("DraggableSlider") || obj->inherits("DraggableLineEdit")||obj->inherits("DraggableVolumeBar"))
               {
                   // 配对Slider和LineEdit
                   if (connectedSlider && connectedLineEdit)
                   {
                       //disconnect(connectedSlider, &DraggableSlider::sliderMoved, this, &MainWindow::updateLineEditValue);
                       //disconnect(connectedLineEdit, &DraggableLineEdit::textEdited, this, &MainWindow::updateSliderValue);
                       connectedSlider=nullptr;
                       connectedLineEdit=nullptr;
                   }
                   if(connectedVolumeBar && connectedLineEdit)
                   {
                       connectedVolumeBar=nullptr;
                       connectedLineEdit=nullptr;
                   }

                   if (obj->inherits("DraggableSlider"))
                   {
                       connectedSlider = static_cast<DraggableSlider*>(obj);
                   }
                   else if (obj->inherits("DraggableLineEdit"))
                   {
                       connectedLineEdit = static_cast<DraggableLineEdit*>(obj);
                   }
                   else if(obj->inherits("DraggableVolumeBar"))
                   {
                       connectedVolumeBar = static_cast<DraggableVolumeBar*>(obj);
                   }

                   // 连接信号和槽，实现值的同步更新
                   if (connectedSlider && connectedLineEdit)
                   {
                       connectedSlider->setMatchId(nextMatchId);
                       connectedLineEdit->setMatchId(nextMatchId);
                       if(connectedSlider->uslider&&connectedLineEdit->lineEdit){
                       connect(connectedSlider->uslider, &UserSlider::sliderMoved, connectedLineEdit->lineEdit, &UserLineEdit::setintText);
                       connect(connectedLineEdit->lineEdit, &QLineEdit::textEdited, connectedSlider->uslider, &UserSlider::setvalue);
                       }
                       nextMatchId++;
                       // 将nextMatchId的值保存到文件中
                       QFile file("match_id.txt");
                       if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
                           QTextStream out(&file);
                           out << QString::number(nextMatchId);
                           file.close();
                      }
                       connectedSlider=nullptr;
                       connectedLineEdit=nullptr;
                   }else if(connectedVolumeBar && connectedLineEdit){
                       connectedVolumeBar->setMatchId(nextMatchId);
                       connectedLineEdit->setMatchId(nextMatchId);
                       if(connectedVolumeBar->volumebar&&connectedLineEdit->lineEdit){
                       connect(connectedVolumeBar->volumebar, &UserVolumeBar::sliderMoved, connectedLineEdit->lineEdit, &UserLineEdit::setintText);
                       connect(connectedLineEdit->lineEdit, &QLineEdit::textEdited, connectedVolumeBar->volumebar, &UserVolumeBar::setvalue);
                       }
                       nextMatchId++;
                       // 将nextMatchId的值保存到文件中
                       QFile file("match_id.txt");
                       if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
                           QTextStream out(&file);
                           out << QString::number(nextMatchId);
                           file.close();
                      }
                       connectedVolumeBar=nullptr;
                       connectedLineEdit=nullptr;
                   }
               }
           }

       }
       else if (event->type() == QEvent::MouseMove)//检测鼠标移动
       {
             QWidget* widget = qobject_cast<QWidget*>(obj);
             m_endPoint = widget->pos();//获取当前鼠标位置
             // 计算鼠标移动的距离
             QPoint offset = m_endPoint - m_startPoint;

             // 移动被选中的控件
             if(choicewidget==true){
             foreach (QWidget *widget, m_selectedWidgets)
             {
                 bool islock=WidgetHelper::checklock(widget);
                 if(islock){
                 if(widget!=obj){//禁止移动鼠标正在拖动的控件，不然会抽搐
                 QPoint widgetPos = widget->pos() + offset;
                 // 限制控件的移动范围，避免飞出窗口外
                 int x = qMax(0, qMin(widgetPos.x(), ui->tab_3->width() - widget->width()));
                 int y = qMax(0, qMin(widgetPos.y(), ui->tab_3->height() - widget->height()));
                 widget->move(x, y);
                 }
                 }
             }


           }
           m_startPoint = m_endPoint;
       }
       else if (event->type() == QEvent::MouseButtonRelease){//鼠标释放
           choicewidget=false;//拖动许可
       }

       //一下的代码实现了按住左键拉出一个框并移动控件
       if (obj == ui->tab_3)
       {
           if (event->type() == QEvent::MouseButtonPress)//检测是否鼠标点击
           {
               QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(event);
               m_selectedWidgets.clear();
               if (mouseEvent && mouseEvent->button() == Qt::LeftButton){
               restorecontrol();
               startPoint = static_cast<QMouseEvent*>(event)->pos();//用来画框的
               m_isDrawing = true;//判断是否可以画框
               m_selectionRect.setTopLeft(startPoint);//设定框的起始位置

               return true;//表示事件已被处理
               }
               else if(mouseEvent && mouseEvent->button() == Qt::RightButton){
               m_rightPoint=static_cast<QMouseEvent*>(event)->pos();
               //QPoint globalPos = ui->tab_3->mapToGlobal(m_rightPoint); // 将局部坐标转换为全局坐标
               WidgetHelper::showContextMenufortab(m_rightPoint,ui->tab_3);
               }
           }
           else if (event->type() == QEvent::MouseMove)//检测鼠标移动
           {
                   if (m_isDrawing)//是否正在绘制蓝框
                   {
                       endPoint = static_cast<QMouseEvent*>(event)->pos();//设置框的范围
                       ui->tab_3->update(); // 触发重绘

                       return true;
                   }
           }
           else if (event->type() == QEvent::MouseButtonRelease)//鼠标释放
           {
               // 鼠标释放，结束绘制
                if (m_isDrawing)
                {
                    endPoint = static_cast<QMouseEvent*>(event)->pos();//设置控件框的范围
                    m_selectionRect.setBottomRight(endPoint);//设置范围框的右下角坐标
                    m_isDrawing = false;
                    ui->tab_3->update(); // 触发重绘
                    m_selectedWidgets.clear();//清空选中的控件
                    // 框选完成，遍历 tab_3 中的控件，判断是否在框选区域内，以下为循环
                    foreach (QWidget *widget, ui->tab_3->findChildren<QWidget*>())
                    {
                        QRect widgetRect = widget->geometry();//获取控件位置

                        if (m_selectionRect.intersects(widgetRect)) // 判断是否相交
                        {
                            if (!m_selectedWidgets.contains(widget)){
                                m_selectedWidgets.append(widget);//把框起来的控件加载进去
                            }
                            WidgetHelper::printblue(widget,true);
                        }
                        //恢复原样
                        else
                        {
                            WidgetHelper::printblue(widget,false);
                        }
                    }
                    return true;
                }
           }
           else if (event->type() == QEvent::Paint)//画蓝框
           {
               // 绘制事件，绘制蓝色框
               if (m_isDrawing)
               {
                   QPainter painter(ui->tab_3);
                   painter.setPen(Qt::blue);
                   painter.drawRect(QRect(startPoint, endPoint));
               }
           }
       }

       return QMainWindow::eventFilter(obj, event);
}
//点击保存会调用此函数，目的是为了移除蓝框。
void MainWindow::restorecontrol(){
    foreach (QWidget *widget, ui->tab_3->findChildren<QWidget*>())
    {
        WidgetHelper::printblue(widget,false);

    }

}
//逐个匹配
void MainWindow::matchSL(){
    for (DraggableSlider* Slider : DraggableSlider::generatedSlider){
        for (DraggableLineEdit* LineEdit : DraggableLineEdit::generatedLineEdit){
            if(Slider->matchId==LineEdit->matchId){
            connect(Slider->uslider, &UserSlider::sliderMoved, LineEdit->lineEdit, &UserLineEdit::setintText);
            connect(LineEdit->lineEdit, &QLineEdit::textEdited, Slider->uslider, &UserSlider::setvalue);
            }
        }
    }
    for (DraggableVolumeBar* volumebar : DraggableVolumeBar::generatedVolumeBar){
        for (DraggableLineEdit* LineEdit : DraggableLineEdit::generatedLineEdit){
            if(volumebar->matchId==LineEdit->matchId){
            connect(volumebar->volumebar, &UserVolumeBar::sliderMoved, LineEdit->lineEdit, &UserLineEdit::setintText);
            connect(LineEdit->lineEdit, &QLineEdit::textEdited, volumebar->volumebar, &UserVolumeBar::setvalue);
            }
        }
    }

}

void MainWindow::actionSelectedWidget(QAction *action){
    WidgetHelper::handleContextMenuAction(action);

}

//占位
void MainWindow::sendData() {

}
//占位
void MainWindow::onConnected() {

}
//占位
void MainWindow::onDisconnected() {

}








