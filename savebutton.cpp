#include "savebutton.h"
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
#include <iostream>


savebutton::savebutton(QWidget* parent)
   : QPushButton(parent){
    setMouseTracking(true);
    doc.appendChild(root);//把根节点添加进doc里面
}
//保存带有文本的控件至xml的模板
template<typename T>
void savebutton::saveGeneratedControlsToXml(const QString& filePath, const QList<T*>& generatedControls,QString name)
{
    for (T* control : generatedControls)
    {
        QDomElement controlElement = doc.createElement(name);
        root.appendChild(controlElement);
        controlElement.setAttribute("name",control->name);
        controlElement.setAttribute("text", control->text());
        controlElement.setAttribute("textsize",control->font().toString());
        controlElement.setAttribute("x", control->x());
        controlElement.setAttribute("y", control->y());
        controlElement.setAttribute("width", control->width());
        controlElement.setAttribute("height", control->height());
        controlElement.setAttribute("matchId",control->getmatchID());
        controlElement.setAttribute("savePath",control->savePath);
        controlElement.setAttribute("backsavePath",control->backsavePath);
        controlElement.setAttribute("frameid",control->frameid);
        controlElement.setAttribute("layers",control->layers);
        controlElement.setAttribute("stylesheet",control->styleSheet());

        // 添加其他控件属性...
        QPalette palette = control->palette();
        QColor Color = palette.color(QPalette::Text);
        // 将背景颜色转换成RGBA格式，并获取其中的透明度值
        // 获取控件的背景颜色
        QColor bgColor = control->palette().color(QPalette::Background);
        QString bgColorStr = QString("rgba(%1, %2, %3, %4)").arg(bgColor.red()).arg(bgColor.green()).arg(bgColor.blue()).arg(bgColor.alpha());
        controlElement.setAttribute("textcolor", Color.name());
        controlElement.setAttribute("basecolor", bgColorStr);


    }



}
//保存无文本的控件至xml的模板
template<typename T>
void savebutton::saveGeneratednotextControlsToXml(const QString& filePath, const QList<T*>& generatedControls,QString name)
{
    emit clearcontrol();
    for (T* control : generatedControls)
    {
        QDomElement controlElement = doc.createElement(name);
        root.appendChild(controlElement);
        controlElement.setAttribute("name",control->name);
        controlElement.setAttribute("x", control->x());
        controlElement.setAttribute("y", control->y());
        controlElement.setAttribute("width", control->width());
        controlElement.setAttribute("height", control->height());
        controlElement.setAttribute("matchId",control->getmatchID());
        controlElement.setAttribute("savePath",control->savePath);
        controlElement.setAttribute("backsavePath",control->backsavePath);
        controlElement.setAttribute("max",control->maximum());
        controlElement.setAttribute("min",control->minimum());
        controlElement.setAttribute("step",control->angleStep);
        controlElement.setAttribute("frameid",control->frameid);
        controlElement.setAttribute("layers",control->layers);
        // 添加其他控件属性...
        QPalette palette = control->palette();
        QColor Color = palette.color(QPalette::Button);
        controlElement.setAttribute("color", Color.name());
    }


}
//鼠标按下保存
void savebutton::mousePressEvent(QMouseEvent* event)
{
    QFile file("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml");//创建文件
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        // 文件已清空
        file.close();
    }
    else
    {
        // 处理无法打开文件的情况
        qDebug() << "Failed to open the file.";
    }
    saveGeneratedButtonsToXml("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml",DraggableButton::generatedButtons,"Buttons");
    saveGeneratedDialsToXml("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml",DraggableDial::generatedDials,"Dials");
    saveGeneratedLablesToXml("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml",DraggableLabel::generatedLabels,"Lables");
    saveGeneratedLineEditToXml("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml",DraggableLineEdit::generatedLineEdit,"LineEdit");
    saveGeneratedCheckboxToXml("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml", DraggableCheckBox::generatedCheckBox,"Checkbox");
    saveGeneratedRadioButtonToXml("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml", DraggableRadioButton::generateRadioButton,"RadioButton");
    saveGeneratedSliderToXml("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml",DraggableSlider::generatedSlider,"Slider");
    saveGeneratedToolButtonToXml("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml",DraggableToolButton::generatedToolButton,"ToolButton");
    saveGeneratedFrameToXml("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml",DraggableFrame::generatedFrame,"Frame");
    saveGeneratedCommandLinkButton("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml",DraggableCommandLinkButton::generatedCommandLinkButton,"CommandLinkButton");
    saveGeneratedVolumeBarToXml("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml",DraggableVolumeBar::generatedVolumeBar,"VolumeBar");
    // 保存 XML 文档到文件
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");  // 设置编码为UTF-8
        stream << doc.toString();
        doc.clear();
        root = doc.createElement("Control");
        doc.appendChild(root);
        file.close();
    }
    else
    {
        qDebug() << "Failed to save to XML file.";
    }
    emit updateclick();
    emit updatematch();

}
//保存按钮
void savebutton::saveGeneratedButtonsToXml(const QString& filePath, const QList<DraggableButton*>& generatedButtons,QString name)
{
    saveGeneratedControlsToXml<DraggableButton>(filePath, generatedButtons,name);
}
//保存旋钮
void savebutton::saveGeneratedDialsToXml(const QString& filePath, const QList<DraggableDial*>& generatedDials,QString name)
{
    saveGeneratednotextControlsToXml<DraggableDial>(filePath, generatedDials,name);
}
//保存标签
void savebutton::saveGeneratedLablesToXml(const QString& filePath, const QList<DraggableLabel*>& generatedLabels,QString name){
    saveGeneratedControlsToXml<DraggableLabel>(filePath, generatedLabels,name);
}
//保存编辑框
void savebutton::saveGeneratedLineEditToXml(const QString& filePath, const QList<DraggableLineEdit*>& generatedLineEdit,QString name){
    saveGeneratedControlsToXml<DraggableLineEdit>(filePath, generatedLineEdit,name);
}
//保存方框
void savebutton::saveGeneratedCheckboxToXml(const QString& filePath, const QList<DraggableCheckBox*>& generatedCheckBox,QString name){
    saveGeneratedControlsToXml<DraggableCheckBox>(filePath, generatedCheckBox,name);
}
//保存声音按钮
void savebutton::saveGeneratedRadioButtonToXml(const QString& filePath, const QList<DraggableRadioButton*>& generateRadioButton,QString name){
    saveGeneratedControlsToXml<DraggableRadioButton>(filePath,generateRadioButton,name);
}
//保持指令链接按钮
void savebutton::saveGeneratedCommandLinkButton(const QString &filePath, const QList<DraggableCommandLinkButton *> &generatedCommandLinkButton, QString name){
    saveGeneratedControlsToXml<DraggableCommandLinkButton>(filePath,generatedCommandLinkButton,name);
}

//保存工具按钮
void savebutton::saveGeneratedToolButtonToXml(const QString& filePath, const QList<DraggableToolButton*>& generatedToolButton,QString name){

    // 遍历控件容器，保存每个控件的属性
    for (DraggableToolButton* ToolButton : generatedToolButton)
    {
        // 创建控件元素
        QDomElement control = doc.createElement(name);
        root.appendChild(control);
        // 设置控件属性
        control.setAttribute("name",ToolButton->name);
        control.setAttribute("x", ToolButton->x());
        control.setAttribute("y", ToolButton->y());
        control.setAttribute("width", ToolButton->width());
        control.setAttribute("height", ToolButton->height());
        control.setAttribute("savePath",ToolButton->savePath);
        control.setAttribute("border",ToolButton->borderstatus);
        control.setAttribute("matchId",ToolButton->getmatchID());
        control.setAttribute("backsavePath",ToolButton->backsavePath);
        control.setAttribute("frameid",ToolButton->frameid);
        control.setAttribute("layers",ToolButton->layers);
        control.setAttribute("stylesheet",ToolButton->styleSheet());
        // 添加其他控件属性...
    }

}
//保存框架
void savebutton::saveGeneratedFrameToXml(const QString& filePath, const QList<DraggableFrame*>& generatedFrame,QString name){

    // 遍历控件容器，保存每个控件的属性
    for (DraggableFrame* Frame : generatedFrame)
    {
        // 创建控件元素
        QDomElement control = doc.createElement(name);
        root.appendChild(control);
        // 设置控件属性
        control.setAttribute("name",Frame->name);
        control.setAttribute("x", Frame ->x());
        control.setAttribute("y", Frame ->y());
        control.setAttribute("width", Frame ->width());
        control.setAttribute("height", Frame ->height());
        control.setAttribute("savePath",Frame ->savePath);
        control.setAttribute("border",Frame ->borderstatus);
        control.setAttribute("matchId",Frame->getmatchID());
        control.setAttribute("backsavePath",Frame->backsavePath);
        control.setAttribute("frameid",Frame->frameid);
        control.setAttribute("layers",Frame->layers);
        control.setAttribute("stylesheet",Frame->styleSheet());
        // 添加其他控件属性...
    }
}
//保存滑动条
void savebutton::saveGeneratedSliderToXml(const QString& filePath,const QList<DraggableSlider*>& generatedSlider,QString name){

    // 遍历控件容器，保存每个控件的属性
    for (DraggableSlider* Slider : generatedSlider)
    {
        // 创建控件元素
        QDomElement control = doc.createElement(name);
        root.appendChild(control);

        // 设置控件属性
        control.setAttribute("name",Slider->name);
        control.setAttribute("x", Slider->x());
        control.setAttribute("y", Slider->y());
        control.setAttribute("width", Slider->width());
        control.setAttribute("height", Slider->height());
        control.setAttribute("orientation",Slider->orientation());
        control.setAttribute("matchId",Slider->getmatchID());
        control.setAttribute("styleSheet",Slider->styleSheet());
        control.setAttribute("savePath",Slider->savePath);
        control.setAttribute("imageCounter",Slider->imageCounter);
        control.setAttribute("max",Slider->maximum());
        control.setAttribute("min",Slider->minimum());
        control.setAttribute("tickInterval",Slider->tickInterval());
        control.setAttribute("backsavePath",Slider->backsavePath);
        control.setAttribute("frameid",Slider->frameid);
        control.setAttribute("layers",Slider->layers);

        // 添加其他控件属性...
    }

}

void savebutton::saveGeneratedVolumeBarToXml(const QString &filePath, const QList<DraggableVolumeBar *> &generatedVolumeBar, QString name){
    // 遍历控件容器，保存每个控件的属性
    for (DraggableVolumeBar* VolumeBar : generatedVolumeBar)
    {
        // 创建控件元素
        QDomElement control = doc.createElement(name);
        root.appendChild(control);

        // 设置控件属性
        control.setAttribute("name",VolumeBar->name);
        control.setAttribute("x", VolumeBar->x());
        control.setAttribute("y", VolumeBar->y());
        control.setAttribute("width", VolumeBar->width());
        control.setAttribute("height", VolumeBar->height());
        control.setAttribute("orientation",VolumeBar->orientation());
        control.setAttribute("matchId",VolumeBar->getmatchID());
        control.setAttribute("styleSheet",VolumeBar->styleSheet());
        control.setAttribute("savePath",VolumeBar->savePath);
        control.setAttribute("imageCounter",VolumeBar->imageCounter);
        control.setAttribute("max",VolumeBar->maximum());
        control.setAttribute("min",VolumeBar->minimum());
        control.setAttribute("tickInterval",VolumeBar->tickInterval());
        control.setAttribute("backsavePath",VolumeBar->backsavePath);
        control.setAttribute("frameid",VolumeBar->frameid);
        control.setAttribute("layers",VolumeBar->layers);

        // 添加其他控件属性...
    }
}

