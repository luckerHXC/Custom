#include "printcontroll.h"
#include "generate.h"
#include "ui_mainwindow.h"
#include "draggablebutton.h"
#include "draggabledial.h"
#include "draggablelineedit.h"
#include "draggabletoolbutton.h"
#include <QObject>
#include <QWidget>
#include "mainwindow.h"
#include "QDebug"
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
#include <QMainWindow>
#include "controlproperty.h"
#include "userlineedit.h"
//打印控件
printcontroll::printcontroll(MainWindow * parent)
{
    this->mainwindow=parent;
}

//在程序启动时调用此函数，将xml上的控件全部打印到界面上
void printcontroll::printall(){
    printFrame();
    printToolButton();
    printall_t<DraggableButton,QPushButton>("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml",DraggableButton::generatedButtons,"Buttons");
    printall_not<DraggableDial,UserDial>("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml",DraggableDial::generatedDials,"Dials");
    printall_t<DraggableLabel,QLabel>("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml",DraggableLabel::generatedLabels,"Lables");
    printLineEdit();
    printall_t<DraggableCheckBox,QCheckBox>("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml",DraggableCheckBox::generatedCheckBox,"Checkbox");
    printall_t<DraggableRadioButton,QRadioButton>("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml",DraggableRadioButton::generateRadioButton,"RadioButton");
    printslider();
    printvolumebar();
    printall_t<DraggableCommandLinkButton,QCommandLinkButton>("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml",DraggableCommandLinkButton::generatedCommandLinkButton,"CommandLinkButton");
    layers();
}


void printcontroll::layers(){
    for(DraggableButton * control :DraggableButton::generatedButtons){
    rankMap.insert(control,control->layers) ;   
    }
    for(DraggableCheckBox * control :DraggableCheckBox::generatedCheckBox){
    rankMap.insert(control,control->layers) ;   
    }
    for(DraggableDial * control :DraggableDial::generatedDials){
    rankMap.insert(control,control->layers) ;   
    }
    for(DraggableDial * control :DraggableDial::generatedDials){
    rankMap.insert(control,control->layers) ;   
    }
    for(DraggableFrame * control :DraggableFrame::generatedFrame){
    rankMap.insert(control,control->layers) ;
    }
    for(DraggableLabel * control :DraggableLabel::generatedLabels){
    rankMap.insert(control,control->layers) ;
    }
    for(DraggableLineEdit * control :DraggableLineEdit::generatedLineEdit){
    rankMap.insert(control,control->layers) ;
    }
    for(DraggableRadioButton * control :DraggableRadioButton::generateRadioButton){
    rankMap.insert(control,control->layers) ;
    }
    for(DraggableSlider * control :DraggableSlider::generatedSlider){
    rankMap.insert(control,control->layers) ;
    }
    for(DraggableToolButton * control :DraggableToolButton::generatedToolButton){
    rankMap.insert(control,control->layers) ;
    }
    for(DraggableCommandLinkButton * control :DraggableCommandLinkButton::generatedCommandLinkButton){
    rankMap.insert(control,control->layers) ;
    }
    for(DraggableVolumeBar * control :DraggableVolumeBar::generatedVolumeBar){
    rankMap.insert(control,control->layers) ;
    }
    mainwindow->layers=0;
    int max=0;
    int min=9999999;
    // 创建一个包含键值对的 QVector
    QVector<QPair<QWidget*, int>> sortedPairs;
    for (auto it = rankMap.begin(); it != rankMap.end(); ++it) {
        sortedPairs.push_back(qMakePair(it.key(), it.value()));
    }

    // 使用 Lambda 函数对 QVector 进行排序
    std::sort(sortedPairs.begin(), sortedPairs.end(), [](const QPair<QWidget*, int>& a, const QPair<QWidget*, int>& b) {
        return a.second < b.second;
    });

    // 输出排序后的键值对
    for (const auto& pair : sortedPairs) {
        pair.first->raise();
        if(controlMap.contains(pair.first)){
            controlMap.value(pair.first)->raise();
        }
        if(pair.second>max||pair.second==max){
            max=pair.second+1;
        }
        if(pair.second<min||pair.second==min){
            min=pair.second-1;
        }
    }
    mainwindow->layers=max;
    mainwindow->downlayers=min;
    rankMap.clear();
}

//打印带有文本的控件
template<typename T,typename U>
void printcontroll::printall_t(QString filepath, QList<T*>& generatedControls,QString name){
    // 创建XML文档，创建根目录root
    QDomDocument doc;
    QDomElement root = doc.createElement("root");
    doc.appendChild(root);
    QFile file(filepath);
    if (!file.exists()) {
        qDebug() << "File does not exist: " << filepath;
        return;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open the file for reading: " << filepath;
        return;
    }

    if (!doc.setContent(&file)) {
        qDebug() << "Failed to parse the XML content from file: " << filepath;
        file.close();
        return;
    }
    file.close();

    root = doc.documentElement();
    QDomNodeList List = root.elementsByTagName(name);
    // 遍历元素
        for (int i = 0; i < List.size(); ++i) {
            QDomElement Elem = List.at(i).toElement();

            // 读取属性
            QString cname=Elem.attribute("name");
            QString colorName = Elem.attribute("textcolor");
            QColor textcolor;
            textcolor.setNamedColor(colorName);
            QString colorName1 = Elem.attribute("basecolor");
            QString stylesheet=Elem.attribute("stylesheet");
            int matchId=Elem.attribute("matchId").toInt();
            int x = Elem.attribute("x").toInt();
            int y = Elem.attribute("y").toInt();
            int width = Elem.attribute("width").toInt();
            int height = Elem.attribute("height").toInt();
            int frameid=Elem.attribute("frameid").toInt();
            int layers=Elem.attribute("layers").toInt();

            QString text = Elem.attribute("text");
            QString fontString = Elem.attribute("textsize");
            QFont font;
            font.fromString(fontString);
            // 文本编码转换
            QTextCodec *codec = QTextCodec::codecForName("UTF-8");
            QByteArray encodedText = codec->fromUnicode(text);
            QString decodedText = codec->toUnicode(encodedText);

            QString imagepath=Elem.attribute("savePath");
            QString backimagepath=Elem.attribute("backsavePath");


            //设计界面控件
            T* mycontrol = new T(this->mainwindow->ui->tab_3);
            mycontrol->name=cname;
            mycontrol->setMatchId(matchId);
            mycontrol->resize(width, height);
            mycontrol->move(x, y);
            mycontrol->setText(decodedText);
           // mycontrol->setStyleSheet(QString("color: %1; background-color: %2;").arg(textcolor.name(),colorName1));
            mycontrol->setFont(font);
            mycontrol->savePath=imagepath;
            mycontrol->imagePath=imagepath;
            mycontrol->backsavePath=backimagepath;
            mycontrol->backimagePath=backimagepath;
            mycontrol->frameid=frameid;
            mycontrol->layers=layers;
            mycontrol->setStyleSheet(stylesheet);
            connect(mycontrol, &T::clicked,mainwindow->cp, &controlproperty::InitTree);
            connect(mainwindow->cp, &controlproperty::editfinish, mycontrol, &T::changeproperty);
            connect(mycontrol,&T::clicked,mycontrol,&T::clicktree);
            connect(mainwindow->cp,&controlproperty::image,mycontrol,&T::changimage);
            connect(mainwindow->cp,&controlproperty::backgroundimage,mycontrol,&T::changbackgroundimage);
            mycontrol->installEventFilter(mainwindow);
            generatedControls.append(mycontrol);
            mycontrol->show();
            //用户界面
            U* control=new U(this->mainwindow->ui->tab_4);
            control->resize(width, height);
            control->move(x, y);
            control->setText(decodedText);
            //control->setStyleSheet(QString("color: %1; background-color: %2;").arg(textcolor.name(),colorName1));
            control->setFont(font);
            control->setStyleSheet(stylesheet);

            mycontrol->control=control;
            mycontrol->customcontrol();

            controlMap.insert(mycontrol,control);
            
            for(DraggableFrame * Frame :DraggableFrame::generatedFrame){
                if(frameid==Frame->frameid){
                    mycontrol->setParent(Frame);
                    mycontrol->show();
                    mycontrol->removeEventFilter(mainwindow->ui->tab_3);
                    // 将按钮的坐标从 QFrame 转换为 tab_3 的坐标
                    QPoint controlPos = control->mapToGlobal(Frame->pos());
                    QPoint localMousePos = mainwindow->ui->tab_3->mapFromGlobal(controlPos);
                    x=localMousePos.x();
                    y=localMousePos.y();
                    control->move(x, y);
                }
            }

        }
}
//打印没有文本的控件
template<typename T,typename U>
void printcontroll::printall_not(QString filepath, QList<T*>& generatedControls,QString name){
    // 创建XML文档，创建根目录root
    QDomDocument doc;
    QDomElement root = doc.createElement("root");
    doc.appendChild(root);
    QFile file(filepath);
    if (!doc.setContent(&file)) {
        // 处理XML解析错误
        file.close();
        return;
    }
    file.close();

    root = doc.documentElement();
    // 遍历 <Button> 元素
    QDomNodeList List = root.elementsByTagName(name);
        for (int i = 0; i < List.size(); ++i) {
            QDomElement Elem = List.at(i).toElement();

            // 读取属性
            QString cname=Elem.attribute("name");
            QString colorName = Elem.attribute("color");
            QColor color;
            color.setNamedColor(colorName);
            int matchId=Elem.attribute("matchId").toInt();
            int x = Elem.attribute("x").toInt();
            int y = Elem.attribute("y").toInt();
            int width = Elem.attribute("width").toInt();
            int height = Elem.attribute("height").toInt();
            QString imagepath=Elem.attribute("savePath");
            QString backimagepath=Elem.attribute("backsavePath");
            int max = Elem.attribute("max").toInt();
            int min = Elem.attribute("min").toInt();
            double step = Elem.attribute("step").toDouble();
            int frameid = Elem.attribute("frameid").toInt();
            int layers=Elem.attribute("layers").toInt();
            T* mycontrol = new T(this->mainwindow->ui->tab_3);
            mycontrol->name=cname;
            mycontrol->setMatchId(matchId);
            mycontrol->resize(width, height);
            mycontrol->move(x, y);
            mycontrol->setStyleSheet(QString("background-color: %1;").arg(color.name()));
            mycontrol->savePath=imagepath;
            mycontrol->imagePath=imagepath;
            mycontrol->backsavePath=backimagepath;
            mycontrol->backimagePath=backimagepath;
            mycontrol->setMaximum(max);
            mycontrol->setMinimum(min);
            mycontrol->setSingleStep(step);
            mycontrol->frameid=frameid;
            mycontrol->layers=layers;

            connect(mycontrol, &T::clicked,mainwindow->cp, &controlproperty::InitTree);
            connect(mainwindow->cp, &controlproperty::editfinish, mycontrol, &T::changeproperty);
            connect(mycontrol,&T::clicked,mycontrol,&T::clicktree);
            connect(mainwindow->cp,&controlproperty::image,mycontrol,&T::changimage);
            connect(mainwindow->cp,&controlproperty::backgroundimage,mycontrol,&T::changbackgroundimage);
            mycontrol->installEventFilter(mainwindow);
            generatedControls.append(mycontrol);
            mycontrol->show();

            U* control=new U(this->mainwindow->ui->tab_4);
            control->resize(width, height);
            control->move(x, y);
            control->setStyleSheet(QString("background-color: %1;").arg(color.name()));
            control->setMaximum(max);
            control->setMinimum(min);
            control->setSingleStep(step);

            mycontrol->control=control;
            mycontrol->customcontrol();

            controlMap.insert(mycontrol,control);
            
            for(DraggableFrame * Frame :DraggableFrame::generatedFrame){
                if(frameid==Frame->frameid){
                    mycontrol->setParent(Frame);
                    mycontrol->show();
                    mycontrol->removeEventFilter(mainwindow->ui->tab_3);
                    // 将按钮的坐标从 QFrame 转换为 tab_3 的坐标
                    QPoint controlPos = control->mapToGlobal(Frame->pos());
                    QPoint localMousePos = mainwindow->ui->tab_3->mapFromGlobal(controlPos);
                    x=localMousePos.x();
                    y=localMousePos.y();
                    control->move(x, y);
                }
            }

        }


}
//打印编辑框
void printcontroll::printLineEdit(){
    // 创建XML文档，创建根目录root
    QDomDocument doc;
    QDomElement root = doc.createElement("root");
    doc.appendChild(root);
    QFile file("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml");
    if (!doc.setContent(&file)) {
        // 处理XML解析错误
        file.close();
        return;
    }
    file.close();

    root = doc.documentElement();
    // 遍历 <Button> 元素
        QDomNodeList List = root.elementsByTagName("LineEdit");
        for (int i = 0; i < List.size(); ++i) {
            QDomElement Elem = List.at(i).toElement();

            // 读取属性
            QString cname=Elem.attribute("name");
            QString colorName = Elem.attribute("textcolor");
            QColor textcolor;
            textcolor.setNamedColor(colorName);
            QString colorName1 = Elem.attribute("basecolor");


            int matchId=Elem.attribute("matchId").toInt();
            int x = Elem.attribute("x").toInt();
            int y = Elem.attribute("y").toInt();
            int width = Elem.attribute("width").toInt();
            int height = Elem.attribute("height").toInt();
            int frameid=Elem.attribute("frameid").toInt();
            QString text = Elem.attribute("text");
            QString fontString = Elem.attribute("textsize");
            int layers=Elem.attribute("layers").toInt();
            QFont font;
            font.fromString(fontString);
            // 文本编码转换
            QTextCodec *codec = QTextCodec::codecForName("UTF-8");
            QByteArray encodedText = codec->fromUnicode(text);
            QString decodedText = codec->toUnicode(encodedText);
            //如果是用户界面调用此函数则不会调用这个判断语句里的代码
            DraggableLineEdit* mycontrol=nullptr;

            mycontrol = new DraggableLineEdit(this->mainwindow->ui->tab_3);
            mycontrol->name=cname;
            mycontrol->setMatchId(matchId);
            mycontrol->resize(width, height);
            mycontrol->move(x, y);
            mycontrol->setText(decodedText);
            mycontrol->setStyleSheet(QString("color: %1; background-color: %2;").arg(textcolor.name(),colorName1));
            mycontrol->setFont(font);
            mycontrol->frameid=frameid;
            mycontrol->layers=layers;
            connect(mycontrol, &DraggableLineEdit::clicked,mainwindow->cp, &controlproperty::InitTree);
            connect(mainwindow->cp, &controlproperty::editfinish, mycontrol, &DraggableLineEdit::changeproperty);
            connect(mycontrol,&DraggableLineEdit::clicked,mycontrol,&DraggableLineEdit::clicktree);
            connect(mainwindow->cp,&controlproperty::image,mycontrol,&DraggableLineEdit::changimage);
            connect(mainwindow->cp,&controlproperty::backgroundimage,mycontrol,&DraggableLineEdit::changbackgroundimage);
            DraggableLineEdit::generatedLineEdit.append(mycontrol);
            mycontrol->installEventFilter(mainwindow);
            mycontrol->show();

            UserLineEdit* control=new UserLineEdit(this->mainwindow->ui->tab_4);
            control->resize(width, height);
            control->move(x, y);
            control->setText(decodedText);
            control->setStyleSheet(QString("color: %1; background-color: %2;").arg(textcolor.name(),colorName1));
            control->setFont(font);
            mycontrol->lineEdit=control;
            mycontrol->customcontrol();

            controlMap.insert(mycontrol,control);

            for(DraggableFrame * Frame :DraggableFrame::generatedFrame){
                if(frameid==Frame->frameid){
                    mycontrol->setParent(Frame);
                    mycontrol->show();
                    mycontrol->removeEventFilter(mainwindow->ui->tab_3);

                    // 将按钮的坐标从 QFrame 转换为 tab_3 的坐标
                    QPoint controlPos = control->mapToGlobal(Frame->pos());
                    QPoint localMousePos = mainwindow->ui->tab_3->mapFromGlobal(controlPos);
                    x=localMousePos.x();
                    y=localMousePos.y();
                    control->move(x, y);
                }
            }

        }
}
//打印滑块
void printcontroll::printslider(){
    QDomDocument doc;
    QDomElement root = doc.createElement("root");
    doc.appendChild(root);
    QFile file("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml");
    if (!doc.setContent(&file)) {
    // 处理XML解析错误
    file.close();
    return;
     }
    file.close();
    root = doc.documentElement();
    // 遍历 <Button> 元素
    QDomNodeList SliderList = root.elementsByTagName("Slider");
    for(int i=0;i<SliderList.size();i++){
    QDomElement LEElem = SliderList.at(i).toElement();
    // 读取属性
    QString cname=LEElem.attribute("name");
    int matchId=LEElem.attribute("matchId").toInt();
    int x = LEElem.attribute("x").toInt();
    int y = LEElem.attribute("y").toInt();
    int width = LEElem.attribute("width").toInt();
    int height = LEElem.attribute("height").toInt();
    int frameid=LEElem.attribute("frameid").toInt();
    QString str = LEElem.attribute("orientation");
    QString styleSheet=LEElem.attribute("styleSheet");
    QString savepath=LEElem.attribute("savePath");
    int imagecount=LEElem.attribute("imageCounter").toInt();
    int max=LEElem.attribute("max").toInt();
    int min=LEElem.attribute("min").toInt();
    int tickInterval=LEElem.attribute("tickInterval").toInt();
    QString backimagepath=LEElem.attribute("backsavePath");
    int layers=LEElem.attribute("layers").toInt();

    Qt::Orientation orientation;
    if(str.toLower() == "1"){
            orientation=Qt::Horizontal;
    }
        else if (str.toLower() == "2"){
            orientation=Qt::Vertical;
    }
    DraggableSlider* Slider=nullptr;

    Slider=new DraggableSlider(orientation,mainwindow->ui->tab_3);
    Slider->name=cname;
    Slider->setMatchId(matchId);
    Slider->resize(width,height);
    Slider->move(x,y);
    Slider->setStyleSheet(styleSheet);
    Slider->savePath=savepath;
    Slider->imageCounter=imagecount;
    Slider->setMaximum(max);
    Slider->setMinimum(min);
    Slider->setTickInterval(tickInterval);
    Slider->backsavePath=backimagepath;
    Slider->backimagePath=backimagepath;
    Slider->frameid=frameid;
    Slider->layers=layers;
    connect(Slider, &DraggableSlider::clicked,mainwindow->cp, &controlproperty::InitTree);
    connect(mainwindow->cp, &controlproperty::editfinish, Slider, &DraggableSlider::changeproperty);
    connect(Slider,&DraggableSlider::clicked,Slider,&DraggableSlider::clicktree);
    connect(mainwindow->cp,&controlproperty::image,Slider,&DraggableSlider::changimage);
    connect(mainwindow->cp,&controlproperty::backgroundimage,Slider,&DraggableSlider::changbackgroundimage);
    DraggableSlider::generatedSlider.append(Slider);
    Slider->installEventFilter(mainwindow);
    Slider->show();


    UserSlider* slider=new UserSlider(orientation,mainwindow->ui->tab_4);
    slider->resize(width,height);
    slider->move(x,y);
    slider->setStyleSheet(styleSheet);
    slider->setMaximum(max);
    slider->setMinimum(min);
    slider->setTickInterval(tickInterval);
    slider->savePath=savepath;
    Slider->uslider=slider;
    Slider->customcontrol();

    controlMap.insert(Slider,slider);

    for(DraggableFrame * Frame :DraggableFrame::generatedFrame){
        if(frameid==Frame->frameid){
            Slider->setParent(Frame);
            Slider->show();
            Slider->removeEventFilter(mainwindow->ui->tab_3);

            // 将按钮的坐标从 QFrame 转换为 tab_3 的坐标
            QPoint controlPos = slider->mapToGlobal(Frame->pos());
            QPoint localMousePos = mainwindow->ui->tab_3->mapFromGlobal(controlPos);
            x=localMousePos.x();
            y=localMousePos.y();
            slider->move(x, y);
        }
    }

    }
}
//打印工具按钮
void printcontroll::printToolButton(){
    QDomDocument doc;
    QDomElement root = doc.createElement("root");
    doc.appendChild(root);
    QFile file("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml");
    if (!doc.setContent(&file)) {
    // 处理XML解析错误
    file.close();
    return;
     }
    file.close();

    root = doc.documentElement();
    // 遍历 <Button> 元素
    QDomNodeList toobuttonList = root.elementsByTagName("ToolButton");
    for(int i=0;i<toobuttonList.size();i++){
    QDomElement LEElem = toobuttonList.at(i).toElement();
    // 读取属性
    QString cname=LEElem.attribute("name");
    int matchId=LEElem.attribute("matchId").toInt();
    int x = LEElem.attribute("x").toInt();
    int y = LEElem.attribute("y").toInt();
    int width = LEElem.attribute("width").toInt();
    int height = LEElem.attribute("height").toInt();
    int frameid = LEElem.attribute("frameid").toInt();
    QString imagepath=LEElem.attribute("savePath");
    QString borderColor=LEElem.attribute("border");
    QString backsavepath=LEElem.attribute("backsavePath");
    int layers=LEElem.attribute("layers").toInt();
    QPixmap pixmap(imagepath);
    QIcon icon(pixmap);
    QString style=LEElem.attribute("stylesheet");

    DraggableToolButton* ToolButton=new DraggableToolButton(mainwindow->ui->tab_3);
    ToolButton->name=cname;
    ToolButton->setMatchId(matchId);
    ToolButton->resize(width,height);
    ToolButton->move(x,y);
    ToolButton->setStyleSheet("QToolButton { border: none; }");
    ToolButton->setpath(imagepath);
    ToolButton->backsavePath=backsavepath;
    ToolButton->frameid=frameid;
    ToolButton->layers=layers;
    ToolButton->setStyleSheet(style);
    if (!imagepath.isEmpty()) {
        if (!pixmap.isNull()) {
           ToolButton->setIcon(icon);
           ToolButton->setIconSize(ToolButton->size()); // 设置图标大小为按钮大小
        }
    }
    connect(ToolButton, &DraggableToolButton::clicked,mainwindow->cp, &controlproperty::InitTree);
    connect(mainwindow->cp, &controlproperty::editfinish,ToolButton, &DraggableToolButton::changeproperty);
    connect(ToolButton,&DraggableToolButton::clicked,ToolButton,&DraggableToolButton::clicktree);
    connect(mainwindow->cp,&controlproperty::image,ToolButton,&DraggableToolButton::changimage);
    connect(mainwindow->cp,&controlproperty::backgroundimage,ToolButton,&DraggableToolButton::changbackgroundimage);
    ToolButton->installEventFilter(mainwindow);
    DraggableToolButton::generatedToolButton.append(ToolButton);
    ToolButton->show();




    QToolButton* toolbutton=new QToolButton(mainwindow->ui->tab_4);
    toolbutton->resize(width,height);
    toolbutton->move(x,y);
    toolbutton->setStyleSheet(style);
    if (!imagepath.isEmpty()) {
        if (!pixmap.isNull()) {
           toolbutton->setIcon(icon);
           toolbutton->setIconSize(toolbutton->size()); // 设置图标大小为按钮大小
        }
    }
    if(borderColor=="0"){
    toolbutton->setStyleSheet("QToolButton { border: none; }");
    }
    ToolButton->control=toolbutton;
    ToolButton->customcontrol();

    controlMap.insert(ToolButton,toolbutton);

    for(DraggableFrame * Frame :DraggableFrame::generatedFrame){
        if(frameid==Frame->frameid){
            ToolButton->setParent(Frame);
            ToolButton->show();
            ToolButton->removeEventFilter(mainwindow->ui->tab_3);

            // 将按钮的坐标从 QFrame 转换为 tab_3 的坐标
            QPoint controlPos = toolbutton->mapToGlobal(Frame->pos());
            QPoint localMousePos = mainwindow->ui->tab_3->mapFromGlobal(controlPos);
            x=localMousePos.x();
            y=localMousePos.y();
            toolbutton->move(x, y);
        }
    }

   }


}
//打印背景框
void printcontroll::printFrame(){
    QDomDocument doc;
    QDomElement root = doc.createElement("root");
    doc.appendChild(root);
    QFile file("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml");
    if (!doc.setContent(&file)) {
    // 处理XML解析错误
    file.close();
    return;
     }
    file.close();

    root = doc.documentElement();

    QDomNodeList FrameList = root.elementsByTagName("Frame");
    for(int i=0;i<FrameList.size();i++){
    QDomElement LEElem = FrameList.at(i).toElement();
    // 读取属性
    QString cname=LEElem.attribute("name");
    int matchId=LEElem.attribute("matchId").toInt();
    int x = LEElem.attribute("x").toInt();
    int y = LEElem.attribute("y").toInt();
    int width = LEElem.attribute("width").toInt();
    int height = LEElem.attribute("height").toInt();
    QString imagepath=LEElem.attribute("savePath");
    QString borderColor=LEElem.attribute("border");
    QString stylesheet=LEElem.attribute("stylesheet");
    int frameid=LEElem.attribute("frameid").toInt();
    int layers=LEElem.attribute("layers").toInt();
    QPixmap pixmap(imagepath);
    QIcon icon(pixmap);

    QString backimagepath=LEElem.attribute("backsavePath");

    DraggableFrame* Frame=new DraggableFrame(mainwindow->ui->tab_3);
    Frame->name=cname;
    Frame->setMatchId(matchId);
    Frame->resize(width,height);
    Frame->move(x,y);
    //Frame->setStyleSheet("QFrame { border: none; }");
    Frame->setpath(imagepath);
    Frame->frameid=frameid;
    Frame->backsavePath=backimagepath;
    Frame->backimagePath=backimagepath;
    Frame->frameid=frameid;
    Frame->layers=layers;
    Frame->setStyleSheet(stylesheet);
    if (!imagepath.isEmpty()) {
        if (!pixmap.isNull()) {
           Frame->setIcon(icon);
           Frame->setIconSize(Frame->size()); // 设置图标大小为按钮大小
        }
    }
    connect(Frame, &DraggableFrame::clicked,mainwindow->cp, &controlproperty::InitTree);
    connect(mainwindow->cp, &controlproperty::editfinish,Frame, &DraggableFrame::changeproperty);
    connect(Frame,&DraggableFrame::clicked,Frame,&DraggableFrame::clicktree);
    connect(mainwindow->cp,&controlproperty::image,Frame,&DraggableFrame::changimage);
    connect(mainwindow->cp,&controlproperty::backgroundimage,Frame,&DraggableFrame::changbackgroundimage);
    Frame->installEventFilter(mainwindow);
    DraggableFrame::generatedFrame.append(Frame);
    Frame->show();


    UserFrame* frame=new UserFrame(mainwindow->ui->tab_4);
    frame->resize(width,height);
    frame->move(x,y);
    frame->setStyleSheet(stylesheet);
    //frame->setStyleSheet("QFrame { border: none; }");
    if (!imagepath.isEmpty()) {
        if (!pixmap.isNull()) {
           frame->setIcon(icon);
           frame->setIconSize(frame->size()); // 设置图标大小为按钮大小
        }
    }
    if(borderColor=="0"){
    //frame->setStyleSheet("QFrame { border: none; }");
     }
    UserFrame::generatedUserFrame.append(frame);

    Frame->control=frame;
    Frame->customcontrol();

    controlMap.insert(Frame,frame);

   }
}
//打印音量条
void printcontroll::printvolumebar(){
    QDomDocument doc;
    QDomElement root = doc.createElement("root");
    doc.appendChild(root);
    QFile file("C:\\Users\\RDSE\\Documents\\Custom_data\\controls.xml");
    if (!doc.setContent(&file)) {
    // 处理XML解析错误
    file.close();
    return;
     }
    file.close();
    root = doc.documentElement();
    // 遍历 <Button> 元素
    QDomNodeList volumebarList = root.elementsByTagName("VolumeBar");
    for(int i=0;i<volumebarList.size();i++){
    QDomElement LEElem = volumebarList.at(i).toElement();
    // 读取属性
    QString cname=LEElem.attribute("name");
    int matchId=LEElem.attribute("matchId").toInt();
    int x = LEElem.attribute("x").toInt();
    int y = LEElem.attribute("y").toInt();
    int width = LEElem.attribute("width").toInt();
    int height = LEElem.attribute("height").toInt();
    int frameid=LEElem.attribute("frameid").toInt();
    QString str = LEElem.attribute("orientation");
    QString styleSheet=LEElem.attribute("styleSheet");
    QString savepath=LEElem.attribute("savePath");
    int imagecount=LEElem.attribute("imageCounter").toInt();
    int max=LEElem.attribute("max").toInt();
    int min=LEElem.attribute("min").toInt();
    int tickInterval=LEElem.attribute("tickInterval").toInt();
    QString backimagepath=LEElem.attribute("backsavePath");
    int layers=LEElem.attribute("layers").toInt();

    Qt::Orientation orientation;
    if(str.toLower() == "1"){
            orientation=Qt::Horizontal;
    }
        else if (str.toLower() == "2"){
            orientation=Qt::Vertical;
    }
    DraggableVolumeBar* VolumeBar=nullptr;

    VolumeBar=new DraggableVolumeBar(orientation,mainwindow->ui->tab_3);
    VolumeBar->name=cname;
    VolumeBar->setMatchId(matchId);
    VolumeBar->resize(width,height);
    VolumeBar->move(x,y);
    VolumeBar->setStyleSheet(styleSheet);
    VolumeBar->savePath=savepath;
    VolumeBar->imageCounter=imagecount;
    VolumeBar->setMaximum(max);
    VolumeBar->setMinimum(min);
    VolumeBar->setTickInterval(tickInterval);
    VolumeBar->backsavePath=backimagepath;
    VolumeBar->backimagePath=backimagepath;
    VolumeBar->frameid=frameid;
    VolumeBar->layers=layers;
    VolumeBar->setValue(-100);
    connect(VolumeBar, &DraggableVolumeBar::clicked,mainwindow->cp, &controlproperty::InitTree);
    connect(mainwindow->cp, &controlproperty::editfinish, VolumeBar, &DraggableVolumeBar::changeproperty);
    connect(VolumeBar,&DraggableVolumeBar::clicked,VolumeBar,&DraggableVolumeBar::clicktree);
    connect(mainwindow->cp,&controlproperty::image,VolumeBar,&DraggableVolumeBar::changimage);
    connect(mainwindow->cp,&controlproperty::backgroundimage,VolumeBar,&DraggableVolumeBar::changbackgroundimage);
    DraggableVolumeBar::generatedVolumeBar.append(VolumeBar);
    VolumeBar->installEventFilter(mainwindow);
    VolumeBar->show();


    UserVolumeBar* volumebar=new UserVolumeBar(orientation,mainwindow->ui->tab_4);
    volumebar->resize(width,height);
    volumebar->move(x,y);
    volumebar->setStyleSheet(styleSheet);
    volumebar->setMaximum(max);
    volumebar->setMinimum(min);
    volumebar->setTickInterval(tickInterval);
    volumebar->savePath=savepath;
    volumebar->setValue(-100);

    VolumeBar->volumebar=volumebar;
    VolumeBar->customcontrol();

    controlMap.insert(VolumeBar,volumebar);

    for(DraggableFrame * Frame :DraggableFrame::generatedFrame){
        if(frameid==Frame->frameid){
            VolumeBar->setParent(Frame);
            VolumeBar->show();
            VolumeBar->removeEventFilter(mainwindow->ui->tab_3);

            // 将按钮的坐标从 QFrame 转换为 tab_3 的坐标
            QPoint controlPos = volumebar->mapToGlobal(Frame->pos());
            QPoint localMousePos = mainwindow->ui->tab_3->mapFromGlobal(controlPos);
            x=localMousePos.x();
            y=localMousePos.y();
            volumebar->move(x, y);
        }
    }

    }    
}

//更新
void printcontroll::updatecontroll()
{
    //先删除用户界面
    QWidget* widget2 = mainwindow->ui->tab_4; // 要删除的 widget
    // 获取 widget2 的所有子控件
    const QList<QWidget*> children2 = widget2->findChildren<QWidget*>();
    // 遍历子控件列表，并删除控件
    for (QWidget* childWidget : children2) {
        // 从 widget2 中移除子控件
        childWidget->setParent(nullptr);
        // 删除子控件
        delete childWidget;
    }
    //删除设计界面
    for(DraggableButton* control : DraggableButton::generatedButtons){
        control->remove();
    }
    for(DraggableCommandLinkButton* control : DraggableCommandLinkButton::generatedCommandLinkButton){
        control->remove();
    }
    for(DraggableCheckBox* control : DraggableCheckBox::generatedCheckBox){
        control->remove();
    }
    for(DraggableDial* control : DraggableDial::generatedDials){
        control->remove();
    }
    for(DraggableLineEdit* control : DraggableLineEdit::generatedLineEdit){
        control->remove();
    }
    for(DraggableRadioButton* control : DraggableRadioButton::generateRadioButton){
        control->remove();
    }
    for(DraggableSlider* control : DraggableSlider::generatedSlider){
        control->remove();
    }
    for(DraggableVolumeBar* control : DraggableVolumeBar::generatedVolumeBar){
        control->remove();
    }
    for(DraggableToolButton* control : DraggableToolButton::generatedToolButton){
        control->remove();
    }
    for(DraggableFrame* control : DraggableFrame::generatedFrame){
        control->remove();
    }
    for(DraggableLabel* control : DraggableLabel::generatedLabels){
        control->remove();
    }
    for(UserFrame* control : UserFrame::generatedUserFrame){
        control->remove();
    }


    printall();

    mainwindow->matchSL();//把关联的slider和lineEdit关联起来
}
