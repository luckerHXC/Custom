#include "generate.h"
#include "ui_mainwindow.h"
#include "draggablebutton.h"
#include <QObject>
#include <QWidget>
#include "mainwindow.h"
#include "QDebug"
#include "controlproperty.h"
#include "userlineedit.h"
//生成栏生成控件
generate::generate(MainWindow * parent)
{
    this->mainwindow=parent;
}
//模板
template<typename T>
T* generate::generateControl()
{
    T* control = new T(mainwindow->ui->tab_3);//将生成的对象添加到设计界面
    QPoint globalMousePos = QCursor::pos();
    QPoint localMousePos = mainwindow->ui->tab_3->mapFromGlobal(globalMousePos);
    control->move(localMousePos);
    control->show();
    //control->grabMouse();
    // 使用当前时间戳作为种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed); // 使用时间戳作为种子
    // 创建一个分布对象，指定生成随机数的范围
    std::uniform_int_distribution<> dist(1, 10000000); // 生成区间[1, 100]的随机整数
    // 生成随机数给图片命名
    control->imageCounter = dist(gen);
    connect(control, &T::clicked, mainwindow->cp, &controlproperty::InitTree);//点击控件可将属性数值添加到属性框里
    connect(mainwindow->cp, &controlproperty::editfinish, control, &T::changeproperty);//编辑完属性框的数值后会改变控件
    connect(control,&T::clicked,control,&T::clicktree);//点击控件选取，以防止改变属性后让其它控件一起改变
    connect(mainwindow->cp,&controlproperty::image,control,&T::changimage);
    connect(mainwindow->cp,&controlproperty::backgroundimage,control,&T::changbackgroundimage);

    control->installEventFilter(mainwindow);

    return control;//返回生成的对象
}
//生成按钮
void generate::generateButton(){
    DraggableButton* button=generateControl<DraggableButton>();
    QString buttonText = "按钮";
    button->setText(buttonText);
    DraggableButton::generatedButtons.append(button);
    button->layers=mainwindow->layers;
    mainwindow->layers++;
}
//生成旋钮
void generate::generateDial(){
    DraggableDial* dial=generateControl<DraggableDial>();
    DraggableDial::generatedDials.append(dial);
    dial->layers=mainwindow->layers;
    mainwindow->layers++;
}
//生成标签
void generate::generateLabel(){
    DraggableLabel* label=generateControl<DraggableLabel>();
    QString Text = "标签";
    label->setText(Text);
    label->resize(24,12);
    DraggableLabel::generatedLabels.append(label);
    label->layers=mainwindow->layers;
    mainwindow->layers++;
}
//生成编辑框
void generate::generateLineEdit(){
    DraggableLineEdit* lineEdit=generateControl<DraggableLineEdit>();
    DraggableLineEdit::generatedLineEdit.append(lineEdit);
    lineEdit->layers=mainwindow->layers;
    mainwindow->layers++;
}
//生成方框
void generate::generateCheckBox(){
    DraggableCheckBox* checkbox=generateControl<DraggableCheckBox>();
    QString Text = "按钮";
    checkbox->setText(Text);
    checkbox->resize(50,14);
    DraggableCheckBox::generatedCheckBox.append(checkbox);
    checkbox->layers=mainwindow->layers;
    mainwindow->layers++;
}
//生成声音按钮
void generate::generateRadioButton(){
    DraggableRadioButton* radiobutton=generateControl<DraggableRadioButton>();
    QString Text = "按钮";
    radiobutton->setText(Text);
    radiobutton->resize(50,14);
    DraggableRadioButton::generateRadioButton.append(radiobutton);
    radiobutton->layers=mainwindow->layers;
    mainwindow->layers++;
}
//生成图片按钮
void generate::generatedToolButton(){
    DraggableToolButton* toolbutton=generateControl<DraggableToolButton>();
    DraggableToolButton::generatedToolButton.append(toolbutton);
    toolbutton->layers=mainwindow->layers;
    mainwindow->layers++;
}
//生成按钮
void generate::generatedCommandLinkButton(){
    DraggableCommandLinkButton * commandlinkbutton=generateControl<DraggableCommandLinkButton>();
    QString buttonText = "指令链接按钮";
    commandlinkbutton->setText(buttonText);
    DraggableCommandLinkButton::generatedCommandLinkButton.append(commandlinkbutton);
    commandlinkbutton->layers=mainwindow->layers;
    mainwindow->layers++;
}
//生成背景板
void generate::generatedFrame(){
    DraggableFrame* Frame=generateControl<DraggableFrame>();
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
}
//生成水平滑槽
void generate::generatedhorizontalSlider(){
    DraggableSlider* horizontalSlider=new DraggableSlider(Qt::Horizontal,mainwindow->ui->tab_3);
    QPoint globalMousePos = QCursor::pos();
    QPoint localMousePos = mainwindow->ui->tab_3->mapFromGlobal(globalMousePos);
    horizontalSlider->move(localMousePos);
    horizontalSlider->show();
    //horizontalSlider->grabMouse();
    // 使用当前时间戳作为种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed); // 使用时间戳作为种子
    // 创建一个分布对象，指定生成随机数的范围
    std::uniform_int_distribution<> dist(1, 10000000); // 生成区间[1, 100]的随机整数
    // 生成随机数
    horizontalSlider->imageCounter = dist(gen);
    DraggableSlider::generatedSlider.append(horizontalSlider);
    connect(horizontalSlider, &DraggableSlider::clicked, mainwindow->cp, &controlproperty::InitTree);//点击控件可将属性数值添加到属性框里
    connect(mainwindow->cp, &controlproperty::editfinish, horizontalSlider, &DraggableSlider::changeproperty);//编辑完属性框的数值后会改变控件
    connect(horizontalSlider,&DraggableSlider::clicked,horizontalSlider,&DraggableSlider::clicktree);//点击控件选取，以防止改变属性后让其它控件一起改变
    connect(mainwindow->cp,&controlproperty::image,horizontalSlider,&DraggableSlider::changimage);
    connect(mainwindow->cp,&controlproperty::backgroundimage,horizontalSlider,&DraggableSlider::changbackgroundimage);
    horizontalSlider->layers=mainwindow->layers;
    mainwindow->layers++;
}
//生成垂直滑槽
void generate::generatedverticalSlider(){
    DraggableSlider* verticalSlider=new DraggableSlider(Qt::Vertical,mainwindow->ui->tab_3);
    QPoint globalMousePos = QCursor::pos();
    QPoint localMousePos = mainwindow->ui->tab_3->mapFromGlobal(globalMousePos);
    verticalSlider->move(localMousePos);
    verticalSlider->show();
    //verticalSlider->grabMouse();
    // 使用当前时间戳作为种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed); // 使用时间戳作为种子
    // 创建一个分布对象，指定生成随机数的范围
    std::uniform_int_distribution<> dist(1, 10000000); // 生成区间[1, 100]的随机整数
    // 生成随机数
    verticalSlider->imageCounter = dist(gen);
    DraggableSlider::generatedSlider.append(verticalSlider);
    connect(verticalSlider, &DraggableSlider::clicked, mainwindow->cp, &controlproperty::InitTree);
    connect(mainwindow->cp, &controlproperty::editfinish, verticalSlider, &DraggableSlider::changeproperty);
    connect(verticalSlider,&DraggableSlider::clicked,verticalSlider,&DraggableSlider::clicktree);
    connect(mainwindow->cp,&controlproperty::image,verticalSlider,&DraggableSlider::changimage);
    connect(mainwindow->cp,&controlproperty::backgroundimage,verticalSlider,&DraggableSlider::changbackgroundimage);
    verticalSlider->layers=mainwindow->layers;
    mainwindow->layers++;
}
//生成音量条
void generate::generatedVolumeBar(){
    DraggableVolumeBar* volumebar=new DraggableVolumeBar(Qt::Vertical,mainwindow->ui->tab_3);
    QPoint globalMousePos = QCursor::pos();
    QPoint localMousePos = mainwindow->ui->tab_3->mapFromGlobal(globalMousePos);
    volumebar->setValue(-100);
    volumebar->move(localMousePos);
    volumebar->show();
    //verticalSlider->grabMouse();
    // 使用当前时间戳作为种子
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed); // 使用时间戳作为种子
    // 创建一个分布对象，指定生成随机数的范围
    std::uniform_int_distribution<> dist(1, 10000000); // 生成区间[1, 100]的随机整数
    // 生成随机数
    volumebar->imageCounter = dist(gen);
    DraggableVolumeBar::generatedVolumeBar.append(volumebar);
    connect(volumebar, &DraggableVolumeBar::clicked, mainwindow->cp, &controlproperty::InitTree);
    connect(mainwindow->cp, &controlproperty::editfinish, volumebar, &DraggableVolumeBar::changeproperty);
    connect(volumebar,&DraggableVolumeBar::clicked,volumebar,&DraggableVolumeBar::clicktree);
    connect(mainwindow->cp,&controlproperty::image,volumebar,&DraggableVolumeBar::changimage);
    connect(mainwindow->cp,&controlproperty::backgroundimage,volumebar,&DraggableVolumeBar::changbackgroundimage);
    volumebar->layers=mainwindow->layers;
    mainwindow->layers++;
}





