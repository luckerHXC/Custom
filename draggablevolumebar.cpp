#include "draggablevolumebar.h"
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
QList<DraggableVolumeBar*> DraggableVolumeBar::generatedVolumeBar;
DraggableVolumeBar::DraggableVolumeBar(Qt::Orientation orientation, QWidget* parent)
    : QSlider(orientation, parent)
{
    setMouseTracking(true);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &DraggableVolumeBar::customContextMenuRequested, this, &DraggableVolumeBar::showContextMenu);
    setRange(-100, 0);
}
//鼠标点击
void DraggableVolumeBar::mousePressEvent(QMouseEvent* event)
{
    WidgetHelper::mousePressEvent(event,this);
}
//控件移动
void DraggableVolumeBar::mouseMoveEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseMoveEvent(event,this);
    }
}
//鼠标释放
void DraggableVolumeBar::mouseReleaseEvent(QMouseEvent* event)
{
    if(lock==true){
    WidgetHelper::mouseReleaseEvent(event,this);
    this->Tree();
    emit clicked();
    }
}
void DraggableVolumeBar::remove(){
    generatedVolumeBar.removeOne(this);
    deleteLater();
}
//打开菜单栏
void DraggableVolumeBar::showContextMenu(const QPoint& pos)
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
//添加推子图片
void DraggableVolumeBar::addImage(){

    imagePath = QFileDialog::getOpenFileName(this, tr("选择图片"), "", tr("图片文件 (*.png *.jpg *.jpeg)"));
    if (!imagePath.isEmpty()) {
        QPixmap pixmap(imagePath);
        savePath = "D://Custom//"+QString::number(imageCounter)+".jpg";  // 设置保存图片的路径和文件名
        if (pixmap.save(savePath)) {
            qDebug() << "图片保存成功：" << savePath;
        } else {
            qDebug() << "图片保存失败：" << savePath;
        }
        QString currentStyleSheet = this->styleSheet(); // 获取当前的样式表
        /*QString styleSheet = QString("QSlider::handle {height: 30px;width: 10px;background-image: url(%1);margin: 0 -10px;border:1px solid white}"
                                     //"QSlider::groove:vertical {border: 1px solid #000000;}"
                                     ).arg(savePath);*/
        QString styleSheet=QString("QSlider::handle { background: rgba(0, 0, 0, 0); }");
        currentStyleSheet += "\n" + styleSheet; // 将新的样式表拼接到原来的样式表后面
        this->setStyleSheet(currentStyleSheet);
        qDebug()<<currentStyleSheet;
        repaint();

    }

}
//放大缩小
void DraggableVolumeBar::wheelEvent(QWheelEvent* event)
{
    if(lock==true){
    WidgetHelper::wheelEvent(event,this);
    }
}
//属性数据展示
void DraggableVolumeBar::Tree(){
    WidgetHelper::name=this->name;
    WidgetHelper::tree(this);
    for(int i=1;i<2;i++)
    {
        //一级节点，加入mModel
        QList<QStandardItem*> items1;
        QStandardItem* item1 = new QStandardItem(QString::number(i));

        if(i==1){
            item1->setText("滑槽与推子");
            for(int j=1;j<3;j++)
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
                }
            }
        }
        items1.append(item1);
        controlproperty::model->appendRow(items1);

       }


}

void DraggableVolumeBar::changcolor(){
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

         // 将颜色的RGB值转换为字符串，并更新QSlider的样式
         QString styleSheet = QString("QSlider::add-page { background: rgb(%1, %2, %3); border: 1px solid #777; border-radius: 2px; margin-left: 9px; margin-right: 9px; }")
                                  .arg(red).arg(green).arg(blue);
         if (!imagePath.isEmpty()) {
           if (this->orientation() == Qt::Vertical) {
         styleSheet = QString("QSlider::add-page:vertical { background: rgb(%1, %2, %3); border: 1px solid #777; border-radius: 2px; margin-left: 9px; margin-right: 9px; }")
                                   .arg(red).arg(green).arg(blue);
           }else{
         styleSheet = QString("QSlider::add-page:horizontal { background: rgb(%1, %2, %3); border: 1px solid #777; border-radius: 2px; margin-left: 9px; margin-right: 9px; }")
                                   .arg(red).arg(green).arg(blue);
           }
         }
         currentStyleSheet += "\n" + styleSheet; // 将新的样式表拼接到原来的样式表后面
         this->setStyleSheet(currentStyleSheet);
     }
}

//改写属性值
void DraggableVolumeBar::changeproperty(){
    if(lock==true){
    if(this->signalEmitted==true){
    WidgetHelper::changeproperty(this);

    if(controlproperty::firstColumnText=="名字"){
        this->name=controlproperty::newText;
    }
    if(controlproperty::firstColumnText=="槽最大值"){
        int max =controlproperty::newText.toInt();
        this->setMaximum(max);
        if(volumebar!=nullptr){
        volumebar->setMaximum(max);
        }
     }
    if(controlproperty::firstColumnText=="槽最小值"){
        int min =controlproperty::newText.toInt();
        this->setMinimum(min);
        if(volumebar!=nullptr){
        volumebar->setMinimum(min);
        }
    }
    if(controlproperty::firstColumnText=="刻度大小"){
        int interval =controlproperty::newText.toInt();
        this->setTickInterval(interval);
        if(volumebar!=nullptr){
        volumebar->setTickInterval(interval);
       }

    }

  }

 }

}
//点击取消别的控件
void DraggableVolumeBar::clicktree() {
    WidgetHelper::clicktree();
    this->signalEmitted = true;
}
void DraggableVolumeBar::setMatchId(int id) {
    matchId = id;
}
int DraggableVolumeBar::getmatchID(){
    return this->matchId;
}
void DraggableVolumeBar::changimage(){
    if(this->signalEmitted == true){
    addImage();
    }
}
//打印已有控件用此函数添加图片
void DraggableVolumeBar::customcontrol(){
    if (!backsavePath.isEmpty()) {
        QString currentStyleSheet = this->styleSheet(); // 获取当前的样式表
        QString styleSheet = QString("QSlider {background-image: url(%1);}").arg(backsavePath);
        currentStyleSheet += "\n" + styleSheet; // 将新的样式表拼接到原来的样式表后面
        this->setStyleSheet(currentStyleSheet);
    }
    if(volumebar){
    QString currentStyleSheet = this->styleSheet(); // 获取当前的样式表
    QString styleSheet = QString("QSlider{background-image: url(%1);}").arg(backsavePath);
    //currentStyleSheet += "\n" + styleSheet; // 将新的样式表拼接到原来的样式表后面
    //volumebar->setStyleSheet(currentStyleSheet);
    }
}
//属性编辑框的更改背景图片的信号发射后会由此函数接收
void DraggableVolumeBar::changbackgroundimage(){
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
        QString styleSheet = QString("QSlider {background-image: url(%1);}").arg(backsavePath);
        currentStyleSheet += "\n" + styleSheet; // 将新的样式表拼接到原来的样式表后面
        this->setStyleSheet(currentStyleSheet);
    }

  }

}
void DraggableVolumeBar::paintEvent(QPaintEvent *event)
{
    QSlider::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 获取滑槽的外观样式
    QStyleOptionSlider opt;
    initStyleOption(&opt);

    QRect grooveRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);

    // 获取滑块的位置和范围
    int minVal = minimum();
    int maxVal = maximum();
    int val = value();

    // 计算填充部分的矩形
    int fillHeight = (grooveRect.height() * (val - minVal)) / (maxVal - minVal);
    QRect fillRect = grooveRect.adjusted(0, grooveRect.height() - fillHeight, 0, 0);

    // 填充低于当前值的部分为蓝色
    painter.fillRect(grooveRect, Qt::white);
    painter.fillRect(fillRect, Qt::blue);

    if(drawblue==true){
    painter.setPen(QPen(Qt::blue, 4)); // 蓝色边框，线宽为2像素
    QRect rect = this->rect(); // 获取自定义小部件的矩形区域
    QPoint point = rect.topLeft(); // 获取矩形区域的左上角坐标
    painter.drawRect(QRect(point, rect.size())); // 绘制一个蓝色矩形边框，以左下角为原点
    }
}

void DraggableVolumeBar::copy(DraggableVolumeBar *slider){
    slider->name=name;
    slider->setMatchId(matchId);
    slider->setFixedSize(size());
    slider->setStyleSheet(styleSheet());
    slider->savePath=savePath;
    slider->imagePath=imagePath;
    slider->backsavePath=backsavePath;
    slider->backimagePath=backimagePath;
    slider->customcontrol();

    connect(slider, &DraggableVolumeBar::clicked,WidgetHelper::mainwindow->cp, &controlproperty::InitTree);
    connect(WidgetHelper::mainwindow->cp, &controlproperty::editfinish,slider, &DraggableVolumeBar::changeproperty);
    connect(slider,&DraggableVolumeBar::clicked,slider,&DraggableVolumeBar::clicktree);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::image,slider,&DraggableVolumeBar::changimage);
    connect(WidgetHelper::mainwindow->cp,&controlproperty::backgroundimage,slider,&DraggableVolumeBar::changbackgroundimage);
    generatedVolumeBar.append(slider);
}
