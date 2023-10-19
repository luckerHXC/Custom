#include "widgethelper.h"
#include "QWidget"
#include "draggablebutton.h"
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
#include "controlproperty.h"
#include "QRect"
#include <QMimeData>
//通用类
bool WidgetHelper::delet;
QPoint WidgetHelper::m_dragStartPosition;
bool WidgetHelper::isResizing;
QPoint WidgetHelper::resizeStartPosition;
QSize WidgetHelper::originalSize;
QString WidgetHelper::resizeCorner;
MainWindow * WidgetHelper::mainwindow;
QString WidgetHelper::name;
QString WidgetHelper::Text;
DraggableButton * WidgetHelper::button;
DraggableDial * WidgetHelper::dial;
DraggableCheckBox * WidgetHelper::checkbox;
DraggableFrame * WidgetHelper::frame;
DraggableLabel * WidgetHelper::label;
DraggableLineEdit * WidgetHelper::lineEdit;
DraggableRadioButton * WidgetHelper::radiobutton;
DraggableSlider * WidgetHelper::slider;
DraggableToolButton * WidgetHelper::toolbutton;
DraggableCommandLinkButton * WidgetHelper::commandlinkbutton;
DraggableVolumeBar * WidgetHelper::volumebar;
QList<QWidget*> WidgetHelper::copiedWidgets;
QMap<QWidget*, QPoint> WidgetHelper::distancesMap;
bool WidgetHelper::enable=true;
WidgetHelper::WidgetHelper(MainWindow * parent)
{
    this->mainwindow=parent;
}
//鼠标点击事件
void WidgetHelper::mousePressEvent(QMouseEvent* event,QWidget *widget){
    if (event->button() == Qt::LeftButton){
    m_dragStartPosition = event->pos();
    widget->setAutoFillBackground(true);

    // 检测鼠标是否在四个角之一
    QPoint mousePos = event->pos();
    QSize widgetSize = widget->size();
    int cornerSize = 10; // 角的大小，可以根据需要调整

    if (mousePos.x() < cornerSize && mousePos.y() < cornerSize) {
        // 左上角
        isResizing = true;
        resizeCorner = "TopLeft";
        resizeStartPosition = event->pos();
        originalSize = widgetSize;
    } else if (mousePos.x() >= widgetSize.width() - cornerSize && mousePos.y() < cornerSize) {
        // 右上角
        isResizing = true;
        resizeCorner = "TopRight";
        resizeStartPosition = event->pos();
        originalSize = widgetSize;
    } else if (mousePos.x() < cornerSize && mousePos.y() >= widgetSize.height() - cornerSize) {
        // 左下角
        isResizing = true;
        resizeCorner = "BottomLeft";
        resizeStartPosition = event->pos();
        originalSize = widgetSize;
    } else if (mousePos.x() >= widgetSize.width() - cornerSize && mousePos.y() >= widgetSize.height() - cornerSize) {
        // 右下角
        isResizing = true;
        resizeCorner = "BottomRight";
        resizeStartPosition = event->pos();
        originalSize = widgetSize;
    } else {
        // 在其他位置按住左键可以移动控件
        isResizing = false;
        m_dragStartPosition = event->pos();
    }
  }
}
//鼠标移动事件
void WidgetHelper::mouseMoveEvent(QMouseEvent* event,QWidget *widget){
     if (isResizing) {
            int deltaX = event->pos().x() - resizeStartPosition.x();
            int deltaY = event->pos().y() - resizeStartPosition.y();
            int newWidth = originalSize.width() + deltaX;
            int newHeight = originalSize.height() + deltaY;

            // 根据鼠标在四个角的位置来调整大小
            if (resizeCorner == "TopLeft") {
                widget->resize(newWidth, newHeight);
            } else if (resizeCorner == "TopRight") {
                widget->resize(newWidth, newHeight);
            } else if (resizeCorner == "BottomLeft") {
                widget->resize(newWidth, newHeight);
            } else if (resizeCorner == "BottomRight") {
                widget->resize(newWidth, newHeight);
            }
        } else if (event->buttons() & Qt::LeftButton) {
            QPoint delta = event->pos() - m_dragStartPosition;
            QPoint newpos = widget->pos() + delta;
            // 限制新位置在窗口范围内
            int x = qMax(0, qMin(newpos.x(), mainwindow->ui->tab_3->width() - widget->width()));
            int y = qMax(0, qMin(newpos.y(), mainwindow->ui->tab_3->height() - widget->height()));

            widget->move(x, y);
            event->accept();
        }

}
//鼠标释放事件
void WidgetHelper::mouseReleaseEvent(QMouseEvent* event,QWidget *widget){

    widget->releaseMouse();  // 释放鼠标
    QRect Rect = widget->geometry();
    QPoint P=Rect.topLeft();
    /*if(P.x()<0){
    widget->deleteLater();
    }*/
    if (widget->cursor().shape() == Qt::ClosedHandCursor) {
    widget->setCursor(QCursor(Qt::ArrowCursor));
    }
    widget->setAutoFillBackground(false);
    isResizing = false;
    widget->unsetCursor();
    //释放鼠标为控件安装事件过滤器
    widget->installEventFilter(mainwindow);
    //重置位置
    m_dragStartPosition.setX(0);
    m_dragStartPosition.setY(0);
}
//滚轮缩放
void WidgetHelper::wheelEvent(QWheelEvent* event,QWidget *widget)
{
    if (event->modifiers() == Qt::ControlModifier) {
        int delta = event->delta();
        QSize size = widget->size();
        if (delta > 0) {
                   size *= 1.2;
               } else {
                   size /= 1.2;
               }
        widget->resize(size);
        // 获取按钮当前的字体
        QFont font = widget->font();
        // 根据按钮的尺寸调整字体大小
        int fontSize = size.height() * 0.4; // 自定义字体大小计算方式，可以根据实际需求调整
        font.setPointSize(fontSize);
        // 设置按钮的新字体
        widget->setFont(font);
        event->accept();
    }
}
//属性值
//如果控件有文本，要传入第二个参数，如果没有只需要传入一个
void WidgetHelper::tree(QWidget *widget){
    mainwindow->cp->widget=widget;
    QRect Geometry = widget->geometry();
    int x = Geometry.x();     // 按钮左上角的 x 坐标
    int y = Geometry.y();     // 按钮左上角的 y 坐标
    int width = Geometry.width();     // 宽度
    int height = Geometry.height();   // 高度
    // 背景颜色
    QColor backgroundColor = widget->palette().color(QPalette::Background);
    QString backgroundColorString = backgroundColor.name();
    QFont buttonFont = widget->font();
    int fontSize = buttonFont.pointSize();
    int borderWidth;
    int borderRadius;
    // 获取控件的样式表
    QString styleSheet = widget->styleSheet();
    // 使用正则表达式查找 border-radius 属性
    QRegularExpression regex("border-radius:\\s*(\\d+)px;");
    QRegularExpressionMatch match = regex.match(styleSheet);
    if (match.hasMatch()) {
        // 获取匹配到的值并转换为整数
        borderRadius = match.captured(1).toInt();
    }
    QRegularExpression regex1("border:\\s*(\\d+)px;");
    QRegularExpressionMatch match1 = regex1.match(styleSheet);
    if (match1.hasMatch()) {
        // 获取匹配到的值并转换为整数
        borderWidth = match1.captured(1).toInt();
    }
    //1，构造Model，这里示例具有3层关系的model构造过程
    controlproperty::model->clear();
    controlproperty::model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("属性") << QStringLiteral("属性值"));     //设置列头
    //i为一级节点
    for(int i=1;i<3;i++)
    {
        //一级节点，加入mModel
        QList<QStandardItem*> items1;
        QStandardItem* item1 = new QStandardItem(QString::number(i));//父节点item1

        if(i==1){
            item1->setText("信息");
            for(int j=1;j<10;j++)
            {
                //二级节点或叫子节点,加入第1个一级节点
                QList<QStandardItem*> items2;
                QStandardItem* item2 = new QStandardItem(QString::number(j));
                QStandardItem* item3 = new QStandardItem(QStringLiteral(""));
                if(j==1){
                    item2->setText("名字");
                    item3->setText(name);
                    items2.append(item2);
                    items2.append(item3);
                    item1->appendRow(items2);
                }else if(j==2){
                    item2->setText("位置");
                    item3->setText(QString::number(x)+","+QString::number(y));
                    items2.append(item2);
                    items2.append(item3);
                    item1->appendRow(items2);
                }
                else if(j==3){
                    if(typeid(*widget)==typeid(DraggableButton)){
                    item2->setText("圆角");
                    item3->setText(QString::number(borderRadius));
                    items2.append(item2);
                    items2.append(item3);
                    item1->appendRow(items2);
                    }
                }
                else if(j==4){
                    item2->setText("大小");
                    item3->setText(QString::number(width)+","+QString::number(height));
                    items2.append(item2);
                    items2.append(item3);
                    item1->appendRow(items2);
                }else if(j==5){
                    item2->setText("背景颜色");
                    //item3->setText(backgroundColorString);
                    mainwindow->cp->backgroundcolor=backgroundColorString;
                    items2.append(item2);
                    items2.append(item3);
                    item1->appendRow(items2);
                }
                else if(j==6){
                    item2->setText("边框宽度");
                    items2.append(item2);
                    item3->setText(QString::number(borderWidth));
                    items2.append(item3);
                    //item1->appendRow(items2);
                }
                else if(j==7){
                    item2->setText("边框颜色");
                    items2.append(item2);
                    items2.append(item3);
                    //item1->appendRow(items2);
                }
                else if(j==8){
                    if (typeid(*widget)!=typeid(DraggableDial) && typeid(*widget)!=typeid(DraggableLabel) && typeid(*widget)!=typeid(DraggableLineEdit)){
                    item2->setText("添加图片");
                    items2.append(item2);
                    items2.append(item3);
                    item1->appendRow(items2);
                    }
                }else if(j==9){
                    if (typeid(*widget)!=typeid(DraggableDial) && typeid(*widget)!=typeid(DraggableLabel) && typeid(*widget)!=typeid(DraggableLineEdit)){
                    item2->setText("添加背景图片");
                    items2.append(item2);
                    items2.append(item3);
                    item1->appendRow(items2);
                    }
                }

            }
        }

        if(i==2){
            if(Text==""){
                break;
            }
            item1->setText("文本");
            for(int j=1;j<3;j++)
            {
                //二级节点,加入第1个一级节点
                QList<QStandardItem*> items2;
                QStandardItem* item2 = new QStandardItem(QString::number(j));
                QStandardItem* item3 = new QStandardItem(QStringLiteral("二级节点"));
                if(j==1){
                    item2->setText("文本");
                    item3->setText(Text);
                    items2.append(item2);
                    items2.append(item3);
                    item1->appendRow(items2);
                }else{
                    item2->setText("文本大小");
                    item3->setText(QString::number(fontSize));
                    items2.append(item2);
                    items2.append(item3);
                    item1->appendRow(items2);
                }

            }
            Text="";
        }

        items1.append(item1);
        controlproperty::model->appendRow(items1);
    }
}
//改变属性值可同步改变控件
void WidgetHelper::changeproperty(QWidget *widget){


    if(controlproperty::firstColumnText=="位置"){
        controlproperty::newText.remove('(').remove(')'); // 去除括号
        // 拆分字符串获取 x 和 y 值
        QStringList positionValues = controlproperty::newText.split(',');

        if (positionValues.size() == 2) {
            bool xOk, yOk;
            int x = positionValues[0].toInt(&xOk);
            int y = positionValues[1].toInt(&yOk);
            widget->move(x,y);
        }
    }

    if(controlproperty::firstColumnText=="圆角"){
        controlproperty::newText.remove('(').remove(')'); // 去除括号
        // 获取圆角数值
        int radius=controlproperty::newText.toInt();
        if(radius>10){
            radius=10;
        }
        QString currentStyleSheet = widget->styleSheet(); // 获取当前的样式表
        // 使用正则表达式来匹配旧的 border-radius 属性并替换为新值
        QRegularExpression regex("border-radius:\\s*\\d+px;");
        currentStyleSheet.replace(regex, QString("border-radius: %1px;").arg(radius));
        widget->setStyleSheet(currentStyleSheet); // 设置更新后的样式表
    }

    if(controlproperty::firstColumnText=="大小"){
        controlproperty::newText.remove('(').remove(')'); // 去除括号
        // 拆分字符串获取 x 和 y 值
        QStringList positionValues = controlproperty::newText.split(',');

        if (positionValues.size() == 2) {
            bool xOk, yOk;
            int x = positionValues[0].toInt(&xOk);
            int y = positionValues[1].toInt(&yOk);
            widget->resize(x,y);
        }
    }

    if(controlproperty::firstColumnText=="背景颜色"){
        widget->setStyleSheet(controlproperty::newText);
    }

    if(controlproperty::firstColumnText=="边框宽度"){
        int border = controlproperty::newText.toInt(); // 获取新的边框宽度值
        QString currentStyleSheet = widget->styleSheet(); // 获取当前的样式表
        // 使用正则表达式匹配并替换边框宽度值
        QRegularExpression regex("border:\\s*\\d+px;");
        currentStyleSheet.replace(regex, QString("border: %1px;").arg(border));
        widget->setStyleSheet(currentStyleSheet); // 设置更新后的样式表
    }

    if(controlproperty::firstColumnText=="文本大小"){
        QFont font;
        int size=controlproperty::newText.toInt();
        font.setPointSize(size);
        widget->setFont(font);
    }
}
//但点击某控件时，就会触发此函数
void WidgetHelper::clicktree(){
    for (DraggableButton* button : DraggableButton::generatedButtons) {
            button->signalEmitted=false;
    }
    for (DraggableDial* dial : DraggableDial::generatedDials) {
        dial->signalEmitted= false;
    }
    for (DraggableLabel* label : DraggableLabel::generatedLabels) {
        label->signalEmitted= false;
    }
    for(DraggableCheckBox* checkboxbutton:DraggableCheckBox::generatedCheckBox){
        checkboxbutton->signalEmitted=false;
    }
    for(DraggableRadioButton* radiobutton:DraggableRadioButton::generateRadioButton){
        radiobutton->signalEmitted=false;
    }
    for (DraggableLineEdit* lineEdit : DraggableLineEdit::generatedLineEdit) {
        lineEdit->signalEmitted= false;
    }
    for (DraggableSlider* Slider : DraggableSlider::generatedSlider) {
        Slider->signalEmitted= false;
    }
    for (DraggableVolumeBar* VolumeBar : DraggableVolumeBar::generatedVolumeBar) {
        VolumeBar->signalEmitted= false;
    }

    for(DraggableToolButton* toolbutton:DraggableToolButton::generatedToolButton){
        toolbutton->signalEmitted=false;
    }

    for(DraggableFrame* Frame:DraggableFrame::generatedFrame){
        Frame->signalEmitted=false;
    }

    for(DraggableCommandLinkButton* commandlinkbutton:DraggableCommandLinkButton::generatedCommandLinkButton){
        commandlinkbutton->signalEmitted=false;
    }

}
//展示菜单
void WidgetHelper::showContextMenu(const QPoint& pos, QWidget *widget) {
    QMenu contextMenu(widget->tr("Context menu"), widget);

    // 设置菜单的样式表，将背景颜色设置为白色
    contextMenu.setStyleSheet("QMenu { border-color:black; background-color: grey; color: white; }");
    //删除
    QAction* removeAction = contextMenu.addAction(widget->tr("删除"));
    //复制
    QAction* copyAction = contextMenu.addAction(widget->tr("复制"));
    //粘贴
    QAction* pasteAction = contextMenu.addAction(widget->tr("粘贴"));
    //锁定
    QAction* lockAction = contextMenu.addAction(widget->tr("锁"));
    //解锁
    QAction* unlockAction = contextMenu.addAction(widget->tr("解锁"));
    //颜色
    QAction* colorMenu = contextMenu.addAction(widget->tr("颜色"));
    // 添加 "背景颜色" 选项
    QAction* changeBgColorAction = contextMenu.addAction(widget->tr("背景颜色"));
    //向前一个图层
    QAction* RaiseAction = contextMenu.addAction(widget->tr("放到顶层"));
    //往后一个图层
    QAction* BackAction = contextMenu.addAction(widget->tr("放到底层"));
    //合并
    QAction* mergeAction = contextMenu.addAction(widget->tr("合并"));
    //拆解
    QAction* disassembleAction = contextMenu.addAction(widget->tr("拆解"));

    QAction* triggeredAction = contextMenu.exec(widget->mapToGlobal(pos));

    if (triggeredAction == removeAction){  //删除
        //把frame里面的控件遍历进去
        for (QWidget * control : mainwindow->m_selectedWidgets){
            frame = dynamic_cast<DraggableFrame*>(control);
            if(frame){
                QList<QWidget*> childWidgets = frame->findChildren<QWidget*>(); // 查找所有子控件
                for (QWidget* childWidget : childWidgets) {
                    mainwindow->m_selectedWidgets.append(childWidget);
                }
            }
        }
        for (QWidget * control : mainwindow->m_selectedWidgets){
            deletecontrol(control);
      }
        mainwindow->m_selectedWidgets.clear();

    }//上锁
    else if(triggeredAction == lockAction){
        lockcontrol(widget,false);
        for(QWidget* control:mainwindow->m_selectedWidgets){
            lockcontrol(control,false);
        }
    }//解锁
    else if(triggeredAction == unlockAction){
        lockcontrol(widget,true);
        for(QWidget* control:mainwindow->m_selectedWidgets){
            lockcontrol(control,true);
        }
    }// 复制控件
    else if (triggeredAction == copyAction){
        //复制前去掉蓝框
        cancelborder();
        //把上一次复制的清除
        copiedWidgets.clear();
        distancesMap.clear();
        //遍历框选的控件
        for (QWidget * control : mainwindow->m_selectedWidgets){

        frame = dynamic_cast<DraggableFrame*>(control);
        if(frame){
        QList<QWidget*> childWidgets = frame->findChildren<QWidget*>();
           if(!childWidgets.isEmpty()){
           int count=0;
           for (QWidget* widget : childWidgets){
           mainwindow->m_selectedWidgets.append(widget);
           count++;
           }
           if(count>1){
           mainwindow->m_selectedWidgets.removeOne(control);
           }
           }
        }

        }

        for (QWidget * control : mainwindow->m_selectedWidgets){
        copycontrol(control);
        findLeftmostWidget(copiedWidgets);
      }


    }
    else if(triggeredAction==colorMenu&&enable==true){ //改变颜色
        // 创建一个调色板
        QColorDialog colorDialog;

        // 弹出调色板对话框
        QColor selectedColor = colorDialog.getColor();

        if (selectedColor.isValid())
        {
            QString colorStr = selectedColor.name();
            QString currentStyleSheet = widget->styleSheet();
            QString setStyleSheet("color: " + colorStr + ";");
            currentStyleSheet += "\n" + setStyleSheet;
            widget->setStyleSheet(currentStyleSheet);
        }
    }
    else if (triggeredAction == changeBgColorAction&&enable==true) {  //改变背景颜色
        // 创建一个调色板
        QColorDialog colorDialog;

        // 弹出调色板对话框
        QColor selectedColor = colorDialog.getColor();

        if (selectedColor.isValid())
        {
            QString colorStr = selectedColor.name();
            QString setStyleSheet("background-color: " + colorStr + ";");
            QString currentStyleSheet = widget->styleSheet(); // 获取当前的样式表
            currentStyleSheet += "\n" + setStyleSheet; // 将新的样式表拼接到原来的样式表后面
            widget->setStyleSheet(currentStyleSheet);
        }

    }else if(triggeredAction == RaiseAction&&enable==true){ //图层升高
            widget->raise();
            raiseORdown(widget,"up");
    }else if(triggeredAction == BackAction&&enable==true){  //图层降低
            widget->lower();
            raiseORdown(widget,"down");
    }else if(triggeredAction == mergeAction&&enable==true){
        QPoint topLeft = QPoint(INT_MAX, INT_MAX);
        QPoint bottomRight = QPoint(INT_MIN, INT_MIN);
            for (QWidget * control : mainwindow->m_selectedWidgets){
                // 获取控件的位置信息
                QPoint controlPos = control->mapToGlobal(QPoint(0, 0));
                QPoint localMousePos = mainwindow->ui->tab_3->mapFromGlobal(controlPos);
                QRect controlRect = control->rect();

                // 计算控件的最左上角和最右下角坐标
                QPoint controlTopLeft = localMousePos;
                QPoint controlBottomRight = localMousePos + controlRect.bottomRight();

                // 更新最左上角和最右下角的坐标
                topLeft.setX(qMin(topLeft.x(), controlTopLeft.x()));
                topLeft.setY(qMin(topLeft.y(), controlTopLeft.y()));

                bottomRight.setX(qMax(bottomRight.x(), controlBottomRight.x()));
                bottomRight.setY(qMax(bottomRight.y(), controlBottomRight.y()));
            }
        // 计算控件的大小
        int width = bottomRight.x() - topLeft.x();
        int height = bottomRight.y() - topLeft.y();
        // 创建一个控件，并设置其位置和大小
        DraggableFrame* Frame=mainwindow->gen->generateControl<DraggableFrame>();
        DraggableFrame::generatedFrame.append(Frame);
        // 使用当前时间戳作为种子
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed); // 使用时间戳作为种子
        // 创建一个分布对象，指定生成随机数的范围
        std::uniform_int_distribution<> dist(1, 10000000); // 生成区间[1, 100]的随机整数
        // 生成随机数
        Frame->frameid = dist(gen);
        Frame->layers=mainwindow->layers;
        mainwindow->layers++;
        Frame->setGeometry(topLeft.x(), topLeft.y(), width, height);
        //取消蓝框
        cancelborder();
        //遍历框选的控件
        for (QWidget * control : mainwindow->m_selectedWidgets){
            QPoint controlPos = control->mapToGlobal(QPoint(0, 0));
            QPoint localMousePos = Frame->mapFromGlobal(controlPos);
            control->setParent(Frame);
            control->move(localMousePos);
            control->show();
            
            frame = dynamic_cast<DraggableFrame*>(control);
            if(frame){
            frame->frameid=Frame->frameid;
            }

            button = dynamic_cast<DraggableButton*>(control);
            if(button){
            button->frameid=Frame->frameid;
            }

            dial = dynamic_cast<DraggableDial*>(control);
            if(dial){
            dial->frameid=Frame->frameid;
            }

            checkbox = dynamic_cast<DraggableCheckBox*>(control);
            if(checkbox){
            checkbox->frameid=Frame->frameid;
            }

            label = dynamic_cast<DraggableLabel*>(control);
            if(label){
            label->frameid=Frame->frameid;
            }

            lineEdit = dynamic_cast<DraggableLineEdit*>(control);
            if(lineEdit){
            lineEdit->frameid=Frame->frameid;
            }

            radiobutton = dynamic_cast<DraggableRadioButton*>(control);
            if(radiobutton){
            radiobutton->frameid=Frame->frameid;
            }

            slider = dynamic_cast<DraggableSlider*>(control);
            if(slider){
            slider->frameid=Frame->frameid;
            }

            volumebar = dynamic_cast<DraggableVolumeBar*>(control);
            if(volumebar){
            volumebar->frameid=Frame->frameid;
            }

            toolbutton = dynamic_cast<DraggableToolButton*>(control);
            if(toolbutton){
            toolbutton->frameid=Frame->frameid;
            }

            commandlinkbutton = dynamic_cast<DraggableCommandLinkButton*>(control);
            if(commandlinkbutton){
            commandlinkbutton->frameid=Frame->frameid;
            }
        }
        Frame->lower();
        raiseORdown(Frame,"down");
        mainwindow->m_selectedWidgets.clear();
    }//拆解
    else if(triggeredAction == disassembleAction&&enable==true){
        frame = dynamic_cast<DraggableFrame*>(widget);
        if(frame){
        // 使用 findChildren 查找 QFrame 下的所有子控件
        QList<QWidget*> childWidgets = frame->findChildren<QWidget*>();
        for(QWidget * control : childWidgets){
            QPoint controlPos = control->mapToGlobal(QPoint(0, 0));
            QPoint localMousePos = mainwindow->ui->tab_3->mapFromGlobal(controlPos);
            control->setParent(mainwindow->ui->tab_3);
            control->move(localMousePos);
            control->show();
          }
        frame->remove();
        }
    }

}
//在tab3右键呼出来的菜单
void WidgetHelper::showContextMenufortab(const QPoint& pos,QWidget *widget){
    QMenu contextMenu(widget->tr("Context menu"), widget);
    contextMenu.setStyleSheet("QMenu { border-color:black; background-color: grey; color: white; }");
    //删除
    QAction* removeAction = contextMenu.addAction(widget->tr("删除"));
    //复制
    QAction* copyAction = contextMenu.addAction(widget->tr("复制"));
    //粘贴
    QAction* pasteAction = contextMenu.addAction(widget->tr("粘贴"));
    //锁定
    QAction* lockAction = contextMenu.addAction(widget->tr("锁"));
    //解锁
    QAction* unlockAction = contextMenu.addAction(widget->tr("解锁"));
    // 创建一个子菜单，名为 "颜色"
    QAction* colorMenu = contextMenu.addAction(widget->tr("颜色"));
    // 添加 "背景颜色" 选项
    QAction* changeBgColorAction = contextMenu.addAction(widget->tr("背景颜色"));
    //向前一个图层
    QAction* RaiseAction = contextMenu.addAction(widget->tr("放到顶层"));
    //往后一个图层
    QAction* BackAction = contextMenu.addAction(widget->tr("放到底层"));
    QAction* triggeredAction = contextMenu.exec(widget->mapToGlobal(pos));
    if(copiedWidgets.isEmpty()){
        pasteAction->setEnabled(false);
    }else{
        pasteAction->setEnabled(true);
    }
    if(triggeredAction==pasteAction){
            for (QWidget * control : copiedWidgets){

                DraggableButton * button = dynamic_cast<DraggableButton*>(control);
                if(button){
                DraggableButton * c_button =mainwindow->gen->generateControl<DraggableButton>();
                c_button->releaseMouse();
                button->copy(c_button); 
                QPoint point=distancesMap.value(control);
                int X = c_button->geometry().x() + point.x();
                int Y = c_button->geometry().y() + point.y();
                c_button->move(X,Y);
                }

                DraggableDial * dial = dynamic_cast<DraggableDial*>(control);
                if(dial){
                DraggableDial * c_dial=mainwindow->gen->generateControl<DraggableDial>();
                c_dial->releaseMouse();
                dial->copy(c_dial);
                QPoint point=distancesMap.value(control);
                int X = c_dial->geometry().x() + point.x();
                int Y = c_dial->geometry().y() + point.y();
                c_dial->move(X,Y);
                }

                DraggableCheckBox * checkbox = dynamic_cast<DraggableCheckBox*>(control);
                if(checkbox){
                DraggableCheckBox * c_checkbox=mainwindow->gen->generateControl<DraggableCheckBox>();
                c_checkbox->releaseMouse();
                checkbox->copy(c_checkbox);
                QPoint point=distancesMap.value(control);
                int X = c_checkbox->geometry().x() + point.x();
                int Y = c_checkbox->geometry().y() + point.y();
                c_checkbox->move(X,Y);
                }

                DraggableFrame * frame = dynamic_cast<DraggableFrame*>(control);
                if(frame){
                DraggableFrame * c_frame=mainwindow->gen->generateControl<DraggableFrame>();
                c_frame->releaseMouse();
                frame->copy(c_frame);
                QPoint point=distancesMap.value(control);
                int X = c_frame->geometry().x() + point.x();
                int Y = c_frame->geometry().y() + point.y();
                c_frame->move(X,Y);
                }

                DraggableLabel * label = dynamic_cast<DraggableLabel*>(control);
                if(label){
                DraggableLabel * c_label=mainwindow->gen->generateControl<DraggableLabel>();
                c_label->releaseMouse();
                label->copy(c_label);
                QPoint point=distancesMap.value(control);
                int X = c_label->geometry().x() + point.x();
                int Y = c_label->geometry().y() + point.y();
                c_label->move(X,Y);
                }

                DraggableLineEdit * lineEdit = dynamic_cast<DraggableLineEdit*>(control);
                if(lineEdit){
                DraggableLineEdit * c_lineEdit =mainwindow->gen->generateControl<DraggableLineEdit>();
                c_lineEdit ->releaseMouse();
                lineEdit->copy(c_lineEdit );
                QPoint point=distancesMap.value(control);
                int X = c_lineEdit->geometry().x() + point.x();
                int Y = c_lineEdit->geometry().y() + point.y();
                c_lineEdit->move(X,Y);
                }

                DraggableRadioButton * radiobutton = dynamic_cast<DraggableRadioButton*>(control);
                if(radiobutton){
                DraggableRadioButton * c_radiobutton =mainwindow->gen->generateControl<DraggableRadioButton>();
                c_radiobutton ->releaseMouse();
                radiobutton->copy(c_radiobutton);
                QPoint point=distancesMap.value(control);
                int X = c_radiobutton->geometry().x() + point.x();
                int Y = c_radiobutton->geometry().y() + point.y();
                c_radiobutton->move(X,Y);
                }

                DraggableSlider * slider = dynamic_cast<DraggableSlider*>(control);
                if(slider){
                Qt::Orientation sliderOrientation = slider->orientation();
                DraggableSlider * c_slider =new DraggableSlider(sliderOrientation,mainwindow->ui->tab_3);
                c_slider->move(mainwindow->m_rightPoint);
                c_slider->show();
                c_slider ->releaseMouse();
                slider->copy(c_slider);
                QPoint point=distancesMap.value(control);
                int X = c_slider->geometry().x() + point.x();
                int Y = c_slider->geometry().y() + point.y();
                c_slider->move(X,Y);
                }

                DraggableVolumeBar * volumebar = dynamic_cast<DraggableVolumeBar*>(control);
                if(volumebar){
                Qt::Orientation sliderOrientation = volumebar->orientation();
                DraggableVolumeBar * c_volumebar =new DraggableVolumeBar(sliderOrientation,mainwindow->ui->tab_3);
                c_volumebar->move(mainwindow->m_rightPoint);
                c_volumebar->show();
                c_volumebar->releaseMouse();
                volumebar->copy(c_volumebar);
                QPoint point=distancesMap.value(control);
                int X = c_volumebar->geometry().x() + point.x();
                int Y = c_volumebar->geometry().y() + point.y();
                c_volumebar->move(X,Y);
                c_volumebar->setValue(-100);
                }

                DraggableToolButton * toolbutton = dynamic_cast<DraggableToolButton*>(control);
                if(toolbutton){
                DraggableToolButton * c_toolbutton=mainwindow->gen->generateControl<DraggableToolButton>();
                c_toolbutton->releaseMouse();
                toolbutton->copy(c_toolbutton);
                QPoint point=distancesMap.value(control);
                int X = c_toolbutton->geometry().x() + point.x();
                int Y = c_toolbutton->geometry().y() + point.y();
                c_toolbutton->move(X,Y);
                }

                DraggableCommandLinkButton * commandlinkbutton = dynamic_cast<DraggableCommandLinkButton*>(control);
                if(commandlinkbutton){
                DraggableCommandLinkButton * c_commandlinkbutton =mainwindow->gen->generateControl<DraggableCommandLinkButton>();
                c_commandlinkbutton->releaseMouse();
                commandlinkbutton->copy(c_commandlinkbutton);
                QPoint point=distancesMap.value(control);
                int X = c_commandlinkbutton->geometry().x() + point.x();
                int Y = c_commandlinkbutton->geometry().y() + point.y();
                c_commandlinkbutton->move(X,Y);
                }


            }
    }
}


void WidgetHelper::findLeftmostWidget(const QList<QWidget*>& widgetList){
    QWidget* leftmostWidget = nullptr;
    int leftmostX = std::numeric_limits<int>::max(); // 初始化为最大值

    // 遍历控件列表，查找最左侧的控件
    for (QWidget* widget : widgetList) {
        int widgetX = widget->geometry().x(); // 获取控件的横坐标
        if (widgetX < leftmostX) {
            leftmostX = widgetX;
            leftmostWidget = widget;
        }
    }

    for (QWidget* widget : copiedWidgets) {
        int deltaX = widget->geometry().x() - leftmostWidget->geometry().x();
        int deltaY = widget->geometry().y() - leftmostWidget->geometry().y();
        QPoint distance(deltaX, deltaY);
        distancesMap[widget] = distance;
    }

}

void WidgetHelper::cancelborder(){
    for (QWidget * control : mainwindow->m_selectedWidgets){
        //取消蓝框
        if(mainwindow->styleMap.contains(control)){
        QString style=mainwindow->styleMap.value(control);//把样式取出来
        control->setStyleSheet(style);
        mainwindow->styleMap.remove(control);
        DraggableDial* dialPtr = dynamic_cast<DraggableDial*>(control);
        if (dialPtr) {
            dialPtr->drawblue=false;
            dialPtr->repaint();
        }
        DraggableFrame* framePtr = dynamic_cast<DraggableFrame*>(control);
        if (framePtr) {
            framePtr->drawblue=false;
            framePtr->repaint();
        }
        DraggableSlider* sliderPtr = dynamic_cast<DraggableSlider*>(control);
        if (sliderPtr) {
            sliderPtr->drawblue=false;
            sliderPtr->repaint();
        }
        DraggableVolumeBar* volumebarPtr = dynamic_cast<DraggableVolumeBar*>(control);
        if (volumebarPtr) {
            volumebarPtr->drawblue=false;
            volumebarPtr->repaint();
        }
       }
    }
}

void WidgetHelper::raiseORdown(QWidget * control,QString UD){
    frame = dynamic_cast<DraggableFrame*>(control);
    if(frame){
    if(UD=="up"){
        frame->layers=mainwindow->layers;
        mainwindow->layers++;
      }else if(UD=="down"){
        frame->layers=mainwindow->downlayers;
        mainwindow->downlayers--;
      }
    }

    button = dynamic_cast<DraggableButton*>(control);
    if(button){
        if(UD=="up"){
            button->layers=mainwindow->layers;
            mainwindow->layers++;
          }else if(UD=="down"){
            button->layers=mainwindow->downlayers;
            mainwindow->downlayers--;
          }
    }

    dial = dynamic_cast<DraggableDial*>(control);
    if(dial){
        if(UD=="up"){
            dial->layers=mainwindow->layers;
            mainwindow->layers++;
          }else if(UD=="down"){
            dial->layers=mainwindow->downlayers;
            mainwindow->downlayers--;
          }
    }

    checkbox = dynamic_cast<DraggableCheckBox*>(control);
    if(checkbox){
        if(UD=="up"){
            checkbox->layers=mainwindow->layers;
            mainwindow->layers++;
          }else if(UD=="down"){
            checkbox->layers=mainwindow->downlayers;
            mainwindow->downlayers--;
          }
    }


    label = dynamic_cast<DraggableLabel*>(control);
    if(label){
        if(UD=="up"){
            label->layers=mainwindow->layers;
            mainwindow->layers++;
          }else if(UD=="down"){
            label->layers=mainwindow->downlayers;
            mainwindow->downlayers--;
          }
    }

    lineEdit = dynamic_cast<DraggableLineEdit*>(control);
    if(lineEdit){
        if(UD=="up"){
            lineEdit->layers=mainwindow->layers;
            mainwindow->layers++;
          }else if(UD=="down"){
            lineEdit->layers=mainwindow->downlayers;
            mainwindow->downlayers--;
          }
    }

    radiobutton = dynamic_cast<DraggableRadioButton*>(control);
    if(radiobutton){
        if(UD=="up"){
            radiobutton->layers=mainwindow->layers;
            mainwindow->layers++;
          }else if(UD=="down"){
            radiobutton->layers=mainwindow->downlayers;
            mainwindow->downlayers--;
          }
    }

    slider = dynamic_cast<DraggableSlider*>(control);
    if(slider){
        if(UD=="up"){
            slider->layers=mainwindow->layers;
            mainwindow->layers++;
          }else if(UD=="down"){
            slider->layers=mainwindow->downlayers;
            mainwindow->downlayers--;
          }
    }

    volumebar = dynamic_cast<DraggableVolumeBar*>(control);
    if(volumebar){
        if(UD=="up"){
            volumebar->layers=mainwindow->layers;
            mainwindow->layers++;
          }else if(UD=="down"){
            volumebar->layers=mainwindow->downlayers;
            mainwindow->downlayers--;
          }
    }

    toolbutton = dynamic_cast<DraggableToolButton*>(control);
    if(toolbutton){
        if(UD=="up"){
            toolbutton->layers=mainwindow->layers;
            mainwindow->layers++;
          }else if(UD=="down"){
            toolbutton->layers=mainwindow->downlayers;
            mainwindow->downlayers--;
          }
    }

    commandlinkbutton = dynamic_cast<DraggableCommandLinkButton*>(control);
    if(commandlinkbutton){
        if(UD=="up"){
            commandlinkbutton->layers=mainwindow->layers;
            mainwindow->layers++;
          }else if(UD=="down"){
            commandlinkbutton->layers=mainwindow->downlayers;
            mainwindow->downlayers--;
          }
    }
}


void WidgetHelper::handleContextMenuAction(QAction *triggeredAction) {

    if (triggeredAction == mainwindow->ui->actionDelete){  //删除
        //把frame里面的控件遍历进去
        for (QWidget * control : mainwindow->m_selectedWidgets){
            frame = dynamic_cast<DraggableFrame*>(control);
            if(frame){
                QList<QWidget*> childWidgets = frame->findChildren<QWidget*>(); // 查找所有子控件
                for (QWidget* childWidget : childWidgets) {
                    mainwindow->m_selectedWidgets.append(childWidget);
                }
            }
        }
        for (QWidget * control : mainwindow->m_selectedWidgets){
            deletecontrol(control);
      }
        mainwindow->m_selectedWidgets.clear();

    }//上锁
    else if(triggeredAction == mainwindow->ui->actionLock){
        for(QWidget * control:mainwindow->m_selectedWidgets){
            lockcontrol(control,false);
       }

    }//解锁
    else if(triggeredAction == mainwindow->ui->actionUnlock){
        for(QWidget * control:mainwindow->m_selectedWidgets){
            lockcontrol(control,true);
        }
    }
    // 复制控件
    else if (triggeredAction == mainwindow->ui->actionCopy){
        //复制前去掉蓝框
        cancelborder();
        //把上一次复制的清除
        copiedWidgets.clear();
        distancesMap.clear();
        //遍历框选的控件
        for (QWidget * control : mainwindow->m_selectedWidgets){

        frame = dynamic_cast<DraggableFrame*>(control);
        if(frame){
        QList<QWidget*> childWidgets = frame->findChildren<QWidget*>();
           if(!childWidgets.isEmpty()){
           for (QWidget* widget : childWidgets){
           mainwindow->m_selectedWidgets.append(widget);
           }
           mainwindow->m_selectedWidgets.removeOne(control);
           }
        }

        }

        for (QWidget * control : mainwindow->m_selectedWidgets){
           copycontrol(control);
           findLeftmostWidget(copiedWidgets);

      }

    }else if(triggeredAction == mainwindow->ui->actionTop){ //图层升高
        for(QWidget* widget:mainwindow->m_selectedWidgets){
            widget->raise();
            raiseORdown(widget,"up");
        }
    }else if(triggeredAction == mainwindow->ui->actionBottom){  //图层降低
        for(QWidget* widget:mainwindow->m_selectedWidgets){
            widget->lower();
            raiseORdown(widget,"down");
        }
    }
}

bool WidgetHelper::checklock(QWidget *control){
    frame = dynamic_cast<DraggableFrame*>(control);
    if(frame){
    if(frame->lock==false){
    return false;
    }
    }

    button = dynamic_cast<DraggableButton*>(control);
    if(button){
    if(button->lock==false){
    return false;
    }
    }

    dial = dynamic_cast<DraggableDial*>(control);
    if(dial){
    if(dial->lock==false){
    return false;
    }
    }

    checkbox = dynamic_cast<DraggableCheckBox*>(control);
    if(checkbox){
    if(checkbox->lock==false){
    return false;
    }
    }

    label = dynamic_cast<DraggableLabel*>(control);
    if(label){
    if(label->lock==false){
    return false;
    }
    }

    lineEdit = dynamic_cast<DraggableLineEdit*>(control);
    if(lineEdit){
    if(lineEdit->lock==false){
    return false;
    }
    }

    radiobutton = dynamic_cast<DraggableRadioButton*>(control);
    if(radiobutton){
    if(radiobutton->lock==false){
    return false;
    }
    }

    slider = dynamic_cast<DraggableSlider*>(control);
    if(slider){
    if(slider->lock==false){
    return false;
    }
    }

    volumebar = dynamic_cast<DraggableVolumeBar*>(control);
    if(volumebar){
    if(volumebar->lock==false){
    return false;
    }
    }

    toolbutton = dynamic_cast<DraggableToolButton*>(control);
    if(toolbutton){
    if(toolbutton->lock==false){
    return false;
    }
    }

    commandlinkbutton = dynamic_cast<DraggableCommandLinkButton*>(control);
    if(commandlinkbutton){
    if(commandlinkbutton->lock==false){
    return false;
    }
    }

    return true;
}

void WidgetHelper::printblue(QWidget* widget,bool blue){
    DraggableButton* buttonptr = dynamic_cast<DraggableButton*>(widget);
    if (buttonptr) {
        buttonptr->drawblue=blue;
        buttonptr->repaint();
    }
    DraggableCheckBox* checkboxptr = dynamic_cast<DraggableCheckBox*>(widget);
    if (checkboxptr) {
        checkboxptr->drawblue=blue;
        checkboxptr->repaint();
    }
    DraggableCommandLinkButton* commandlinkbuttonptr = dynamic_cast<DraggableCommandLinkButton*>(widget);
    if (commandlinkbuttonptr) {
        commandlinkbuttonptr->drawblue=blue;
        commandlinkbuttonptr->repaint();
    }
    DraggableLabel* labelptr = dynamic_cast<DraggableLabel*>(widget);
    if (labelptr) {
        labelptr->drawblue=blue;
        labelptr->repaint();
    }
    DraggableLineEdit* lineEditptr = dynamic_cast<DraggableLineEdit*>(widget);
    if (lineEditptr) {
        lineEditptr->drawblue=blue;
        lineEditptr->repaint();
    }
    DraggableRadioButton* radiobuttonptr = dynamic_cast<DraggableRadioButton*>(widget);
    if (radiobuttonptr) {
        radiobuttonptr->drawblue=blue;
        radiobuttonptr->repaint();
    }
    DraggableToolButton* toolbuttonptr = dynamic_cast<DraggableToolButton*>(widget);
    if (toolbuttonptr) {
        toolbuttonptr->drawblue=blue;
        toolbuttonptr->repaint();
    }
    DraggableDial* dialPtr = dynamic_cast<DraggableDial*>(widget);
    if (dialPtr) {
        dialPtr->drawblue=blue;
        dialPtr->repaint();
    }
    DraggableFrame* framePtr = dynamic_cast<DraggableFrame*>(widget);
    if (framePtr) {
        framePtr->drawblue=blue;
        framePtr->repaint();
    }
    DraggableSlider* sliderPtr = dynamic_cast<DraggableSlider*>(widget);
    if (sliderPtr) {
        sliderPtr->drawblue=blue;
        sliderPtr->repaint();
    }
    DraggableVolumeBar* volptr = dynamic_cast<DraggableVolumeBar*>(widget);
    if (volptr) {
        volptr->drawblue=blue;
        volptr->repaint();
    }

}

void WidgetHelper::lockcontrol(QWidget * control,bool lock){
    if(lock==false){
    control->removeEventFilter(mainwindow);
    }else{
    control->installEventFilter(mainwindow);
    }
    button = dynamic_cast<DraggableButton*>(control);
    if(button){
    button->lock=lock;
    }

    dial = dynamic_cast<DraggableDial*>(control);
    if(dial){
    dial->lock=lock;
    }

    checkbox = dynamic_cast<DraggableCheckBox*>(control);
    if(checkbox){
    checkbox->lock=lock;
    }

    frame = dynamic_cast<DraggableFrame*>(control);
    if(frame){
    frame->lock=lock;
    }

    label = dynamic_cast<DraggableLabel*>(control);
    if(label){
    label->lock=lock;
    }

    lineEdit = dynamic_cast<DraggableLineEdit*>(control);
    if(lineEdit){
    lineEdit->lock=lock;
    }

    radiobutton = dynamic_cast<DraggableRadioButton*>(control);
    if(radiobutton){
    radiobutton->lock=lock;
    }

    slider = dynamic_cast<DraggableSlider*>(control);
    if(slider){
    slider->lock=false;
    }

    volumebar = dynamic_cast<DraggableVolumeBar*>(control);
    if(volumebar){
    volumebar->lock=lock;
    }

    toolbutton = dynamic_cast<DraggableToolButton*>(control);
    if(toolbutton){
    toolbutton->lock=lock;
    }

    commandlinkbutton=dynamic_cast<DraggableCommandLinkButton*>(control);
    if(commandlinkbutton){
    commandlinkbutton->lock=lock;
    }
}

void WidgetHelper::deletecontrol(QWidget * control){
    button = dynamic_cast<DraggableButton*>(control);
    if(button&&button->lock==true){
    button->remove();
    }

    dial = dynamic_cast<DraggableDial*>(control);
    if(dial&&dial->lock==true){
    dial->remove();
    }

    checkbox = dynamic_cast<DraggableCheckBox*>(control);
    if(checkbox&&checkbox->lock==true){
    checkbox->remove();
    }

    frame = dynamic_cast<DraggableFrame*>(control);
    if(frame&&frame->lock==true){
    frame->remove();
    }

    label = dynamic_cast<DraggableLabel*>(control);
    if(label&&label->lock==true){
    label->remove();
    }

    lineEdit = dynamic_cast<DraggableLineEdit*>(control);
    if(lineEdit&&lineEdit->lock==true){
    lineEdit->remove();
    }

    radiobutton = dynamic_cast<DraggableRadioButton*>(control);
    if(radiobutton&&radiobutton->lock==true){
    radiobutton->remove();
    }

    slider = dynamic_cast<DraggableSlider*>(control);
    if(slider&&slider->lock==true){
    slider->remove();
    }

    volumebar = dynamic_cast<DraggableVolumeBar*>(control);
    if(volumebar&&volumebar->lock==true){
    volumebar->remove();
    }

    toolbutton = dynamic_cast<DraggableToolButton*>(control);
    if(toolbutton&&toolbutton->lock==true){
    toolbutton->remove();
    }

    commandlinkbutton=dynamic_cast<DraggableCommandLinkButton*>(control);
    if(commandlinkbutton&&commandlinkbutton->lock==true){
    commandlinkbutton->remove();
    }
}

void WidgetHelper::copycontrol(QWidget *control){
    frame = dynamic_cast<DraggableFrame*>(control);
    if(frame&&frame->lock==true){
    copiedWidgets.append(frame); // 将副本添加到列表中
    }

    button = dynamic_cast<DraggableButton*>(control);
    if(button&&button->lock==true){
    copiedWidgets.append(button); // 将副本添加到列表中
    }

    dial = dynamic_cast<DraggableDial*>(control);
    if(dial&&dial->lock==true){
    copiedWidgets.append(dial); // 将副本添加到列表中
    }

    checkbox = dynamic_cast<DraggableCheckBox*>(control);
    if(checkbox&&checkbox->lock==true){
    copiedWidgets.append(checkbox); // 将副本添加到列表中
    }

    label = dynamic_cast<DraggableLabel*>(control);
    if(label&&label->lock==true){
    copiedWidgets.append(label); // 将副本添加到列表中
    }

    lineEdit = dynamic_cast<DraggableLineEdit*>(control);
    if(lineEdit&&lineEdit->lock==true){
    copiedWidgets.append(lineEdit); // 将副本添加到列表中
    }

    radiobutton = dynamic_cast<DraggableRadioButton*>(control);
    if(radiobutton&&radiobutton->lock==true){
    copiedWidgets.append(radiobutton); // 将副本添加到列表中
    }

    slider = dynamic_cast<DraggableSlider*>(control);
    if(slider&&slider->lock==true){
    copiedWidgets.append(slider); // 将副本添加到列表中
    }

    volumebar = dynamic_cast<DraggableVolumeBar*>(control);
    if(volumebar&&volumebar->lock==true){
    copiedWidgets.append(volumebar);
    }

    toolbutton = dynamic_cast<DraggableToolButton*>(control);
    if(toolbutton&&toolbutton->lock==true){
    copiedWidgets.append(toolbutton); // 将副本添加到列表中
    }

    commandlinkbutton = dynamic_cast<DraggableCommandLinkButton*>(control);
    if(commandlinkbutton&&commandlinkbutton->lock==true){
    copiedWidgets.append(commandlinkbutton); // 将副本添加到列表中
    }
}
