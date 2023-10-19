#include <QLayout>
#include <QMenu>
#include <QDrag>
#include <QApplication>
#include <QPainter>
#include <QFileDialog>
#include "draggableslider.h"
#include "widgethelper.h"
#include "connectsl.h"
#include <QStyle>
QList<DraggableSlider*> DraggableSlider::generatedSlider;
DraggableSlider::DraggableSlider(Qt::Orientation orientation, QWidget* parent)
    : QSlider(orientation, parent)
{
    setMouseTracking(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &DraggableSlider::customContextMenuRequested, this, &DraggableSlider::showContextMenu);
    setRange(-72, 12);
    this->setTickPosition(QSlider::TicksLeft);
    int tickInterval = 12; // 设置刻度间隔为12
    this->setTickInterval(tickInterval);

    if(orientation==Qt::Orientation::Vertical){
    this->setStyleSheet(
    "QSlider::add-page:vertical { background: rgb(255,255,255);border: 1px solid #777;border-radius: 2px; margin-left: 9px; margin-right: 9px; }"
    "QSlider::handle:vertical {background:rgb(0, 120, 255);}"
    );
    }else{
    this->setStyleSheet(
    "QSlider::add-page:horizontal { background: rgb(255,255,255);border: 1px solid #777;border-radius: 2px; margin-top: 9px; margin-bottom: 9px; }"
    "QSlider::handle:horizontal {background:rgb(0, 120, 255);}"
    );
    }

}
//鼠标点击
void DraggableSlider::mousePressEvent(QMouseEvent* event)
{
    WidgetHelper::mousePressEvent(event,this);
}
//控件移动
void DraggableSlider::mouseMoveEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseMoveEvent(event,this);
    }
}
//鼠标释放
void DraggableSlider::mouseReleaseEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseReleaseEvent(event,this);
    this->Tree();
    emit clicked();
    }
}
void DraggableSlider::remove(){
    generatedSlider.removeOne(this);
    deleteLater();
}
//打开菜单栏
void DraggableSlider::showContextMenu(const QPoint& pos)
{
    WidgetHelper::enable=lock;
    WidgetHelper::showContextMenu(pos,this);
}
//添加推子图片
void DraggableSlider::addImage(){

    imagePath = QFileDialog::getOpenFileName(this, tr("选择图片"), "", tr("图片文件 (*.png *.jpg *.jpeg)"));
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        savePath = "C:\\Users\\RDSE\\Documents\\Custom_data\\"+QString::number(imageCounter)+".jpg";  // 设置保存图片的路径和文件名
        if (pixmap.save(savePath)) {
            qDebug() << "图片保存成功：" << savePath;
        } else {
            qDebug() << "图片保存失败：" << savePath;
        }
        QString currentStyleSheet = this->styleSheet(); // 获取当前的样式表
        // 创建正则表达式来匹配 QSlider::add-page 部分
        QRegularExpression regex("QSlider::handle[^{]*\\{[^}]*\\}");
        // 替换样式表中匹配的部分
        QString replacement = QString("QSlider::handle { background: rgb(0, 0,0,0);}");
        currentStyleSheet.replace(regex, replacement);

        this->setStyleSheet(currentStyleSheet);
        qDebug()<<currentStyleSheet;
        repaint();

    }

}
//放大缩小
void DraggableSlider::wheelEvent(QWheelEvent* event)
{
    if(lock==true){
    WidgetHelper::wheelEvent(event,this);
    }
}
//属性数据展示
void DraggableSlider::Tree(){
    WidgetHelper::name=this->name;
    WidgetHelper::tree(this);
    for(int i=1;i<2;i++)
    {
        //一级节点，加入mModel
        QList<QStandardItem*> items1;
        QStandardItem* item1 = new QStandardItem(QString::number(i));

        if(i==1){
            item1->setText("滑槽与推子");
            for(int j=1;j<5;j++)
            {
                //二级节点,加入第1个一级节点
                QList<QStandardItem*> items2;
                QStandardItem* item2 = new QStandardItem(QString::number(j));
                QStandardItem* item3 = new QStandardItem(QStringLiteral(""));
                if(j==1){
                    item2->setText("槽最大值");
                    item3->setText(QString::number(this->maximum()));
                    items2.append(item2);
                    items2.append(item3);
                    item1->appendRow(items2);
                }else if(j==2){
                    item2->setText("槽最小值");
                    item3->setText(QString::number(this->minimum()));
                    items2.append(item2);
                    items2.append(item3);
                    item1->appendRow(items2);
                }else if(j==3){
                    item2->setText("刻度大小");
                    item3->setText(QString::number(this->tickInterval()));
                    items2.append(item2);
                    items2.append(item3);
                    item1->appendRow(items2);
                }else if(j==4){
                    item2->setText("滑槽颜色");
                    items2.append(item2);
                    items2.append(item3);
                    item1->appendRow(items2);
                    WidgetHelper::mainwindow->cp->slider=this;
                }
            }
        }
        items1.append(item1);
        controlproperty::model->appendRow(items1);

       }


}

void DraggableSlider::changcolor(){
    // 创建一个调色板
    QColorDialog colorDialog;

    // 弹出调色板对话框
    QColor selectedColor = colorDialog.getColor();

    if (selectedColor.isValid()) {
        // 获取颜色的RGB值
        int red = selectedColor.red();
        int green = selectedColor.green();
        int blue = selectedColor.blue();
        QString currentStyleSheet = this->styleSheet(); // 获取当前的样式表

        // 创建正则表达式来匹配 QSlider::add-page 部分
        QRegularExpression regex("QSlider::add-page[^{]*\\{[^}]*\\}");
        QString replacement;
        // 替换样式表中匹配的部分
        if(this->orientation() == Qt::Vertical){
        replacement = QString("QSlider::add-page { background: rgb(%1, %2, %3); border: 1px solid #777; border-radius: 2px; margin-left: 9px; margin-right: 9px; }")
            .arg(red).arg(green).arg(blue);
        }else{
        replacement = QString("QSlider::add-page { background: rgb(%1, %2, %3); border: 1px solid #777; border-radius: 2px; margin-top: 9px; margin-bottom: 9px; }")
             .arg(red).arg(green).arg(blue);
        }
        currentStyleSheet.replace(regex, replacement);

        // 设置更新后的样式表
        this->setStyleSheet(currentStyleSheet);
        qDebug() << currentStyleSheet;
    }
}

//改写属性值
void DraggableSlider::changeproperty(){
    if(lock==true){
    if(this->signalEmitted==true){
    WidgetHelper::changeproperty(this);

    if(controlproperty::firstColumnText=="名字"){
        this->name=controlproperty::newText;
    }
    if(controlproperty::firstColumnText=="槽最大值"){
        int max =controlproperty::newText.toInt();
        this->setMaximum(max);
        if(uslider!=nullptr){
        uslider->setMaximum(max);
        }
     }
    if(controlproperty::firstColumnText=="槽最小值"){
        int min =controlproperty::newText.toInt();
        this->setMinimum(min);
        if(uslider!=nullptr){
        uslider->setMinimum(min);
        }
    }
    if(controlproperty::firstColumnText=="刻度大小"){
        int interval =controlproperty::newText.toInt();
        this->setTickInterval(interval);
        if(uslider!=nullptr){
        uslider->setTickInterval(interval);
       }

    }

  }

 }

}
//点击取消别的控件
void DraggableSlider::clicktree() {
    WidgetHelper::clicktree();
    this->signalEmitted = true;
}
void DraggableSlider::setMatchId(int id) {
    matchId = id;
}
int DraggableSlider::getmatchID(){
    return this->matchId;
}
void DraggableSlider::changimage(){
    if(this->signalEmitted == true){
    addImage();
    }
}
//打印已有控件用此函数添加图片
void DraggableSlider::customcontrol(){
    if (!backsavePath.isEmpty()) {
        QString currentStyleSheet = this->styleSheet(); // 获取当前的样式表
        QString styleSheet = QString("QSlider {background-image: url(%1);}").arg(backsavePath);
        currentStyleSheet += "\n" + styleSheet; // 将新的样式表拼接到原来的样式表后面
        this->setStyleSheet(currentStyleSheet);
    }
    if(uslider){
    QString currentStyleSheet = this->styleSheet(); // 获取当前的样式表
    QString styleSheet = QString("QSlider{background-image: url(%1);}").arg(backsavePath);
    currentStyleSheet += "\n" + styleSheet; // 将新的样式表拼接到原来的样式表后面
    uslider->setStyleSheet(currentStyleSheet);
    }
}
//属性编辑框的更改背景图片的信号发射后会由此函数接收
void DraggableSlider::changbackgroundimage(){
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
        QString currentStyleSheet = this->styleSheet(); // 获取当前的样式表
        QString styleSheet = QString("QSlider {background-image: url(%1);}").arg(backsavePath);
        currentStyleSheet += "\n" + styleSheet; // 将新的样式表拼接到原来的样式表后面
        this->setStyleSheet(currentStyleSheet);
    }

  }

}
void DraggableSlider::paintEvent(QPaintEvent *event)
{
    QSlider::paintEvent(event); // 调用基类的绘制
    QPainter painter(this);
    if(drawblue==true){
    painter.setPen(QPen(Qt::blue, 4)); // 蓝色边框，线宽为2像素
    QRect rect = this->rect(); // 获取自定义小部件的矩形区域
    QPoint point = rect.topLeft(); // 获取矩形区域的左上角坐标
    painter.drawRect(QRect(point, rect.size())); // 绘制一个蓝色矩形边框，以左下角为原点
    }
    if(savePath==""){

    }else{
    QPixmap pixmap(savePath);
    QPainter painter(this);
    QRect handleRect = QRect();

    if (orientation() == Qt::Vertical) {
        int sliderWidth = width(); // 获取滑块宽度
        int sliderHeight = height(); // 获取滑块高度
        int handleHeight = sliderHeight / 7; // 计算滑块高度，占用整个 slider 高度的七分之一
        int handleWidth = sliderWidth / 1.8; // 计算滑块宽度，占用整个 slider 宽度的二分之一

        int handleY = (sliderHeight - handleHeight) * (maximum() - value()) / (maximum() - minimum()); // 计算滑块位置
        int handleX = (sliderWidth) * 7 / 18; // 计算滑块的横坐标，位于整个 slider 宽度的三分之一位置
        handleRect = QRect(handleX, handleY, handleWidth, handleHeight); // 创建滑块的 QRect

        // 调整图片大小以适应滑块
        pixmap = pixmap.scaled(handleHeight, handleHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    } else if (orientation() == Qt::Horizontal) {
        int sliderWidth = width(); // 获取滑块宽度
        int sliderHeight = height(); // 获取滑块高度
        int handleWidth = sliderWidth / 7; // 计算滑块宽度，占用整个 slider 宽度的七分之一
        int handleHeight = sliderHeight / 1.8; // 计算滑块高度，占用整个 slider 高度的七分之一
        int handleX = (sliderWidth - handleWidth) * (value() - minimum()) / (maximum() - minimum()); // 计算滑块位置
        int handleY = (sliderHeight) * 7 / 18; // 计算滑块的纵坐标，位于整个 slider 宽度的三分之二位置
        handleRect = QRect(handleX, handleY, handleWidth, handleHeight); // 创建滑块的 QRect

        // 调整图片大小以适应滑块
        pixmap = pixmap.scaled(handleWidth, sliderHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation).transformed(QTransform().rotate(270));
    }

    painter.drawPixmap(handleRect, pixmap);
    }
}

void DraggableSlider::copy(DraggableSlider *slider){
    slider->name=name;
    slider->setMatchId(matchId);
    slider->setFixedSize(size());
    slider->setStyleSheet(styleSheet());
    slider->savePath=savePath;
    slider->imagePath=imagePath;
    slider->backsavePath=backsavePath;
    slider->backimagePath=backimagePath;
    slider->customcontrol();

    connect(slider, &DraggableSlider::clicked,WidgetHelper::mainwindow->cp, &controlproperty::InitTree);
    connect(WidgetHelper::mainwindow->cp, &controlproperty::editfinish,slider, &DraggableSlider::changeproperty);
    connect(slider,&DraggableSlider::clicked,slider,&DraggableSlider::clicktree);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::image,slider,&DraggableSlider::changimage);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::backgroundimage,slider,&DraggableSlider::changbackgroundimage);
    generatedSlider.append(slider);
}


