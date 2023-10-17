#ifndef SAVEBUTTON_H
#define SAVEBUTTON_H
#include <QPushButton>
#include <QMouseEvent>
#include <QVBoxLayout>
#include "draggablebutton.h"
#include "draggabledial.h"
#include "draggablelabel.h"
#include "draggablelineedit.h"
#include "draggablecheckbox.h"
#include "draggableradiobutton.h"
#include "draggableslider.h"
#include "draggabletoolbutton.h"
#include "draggableframe.h"
#include "userlineedit.h"
#include "QDomDocument"
#include "draggablecommandlinkbutton.h"
#include "draggablevolumebar.h"
//让保存按钮可以实现保存页面上的控件到xml文件，并实现了同步设计界面到用户界面
class savebutton : public QPushButton
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent* event);
    QDomDocument doc;
    QDomElement root = doc.createElement("Control");
public slots:  
    void saveGeneratedButtonsToXml(const QString& filePath, const QList<DraggableButton*>& generatedButtons,QString name);
    void saveGeneratedDialsToXml(const QString& filePath, const QList<DraggableDial*>& generatedDials,QString name);
    void saveGeneratedLablesToXml(const QString& filePath, const QList<DraggableLabel*>& generatedlabel,QString name);
    void saveGeneratedLineEditToXml(const QString& filePath, const QList<DraggableLineEdit*>& generatedLineEdit,QString name);
    void saveGeneratedCheckboxToXml(const QString& filePath, const QList<DraggableCheckBox*>& generatedCheckBox,QString name);
    void saveGeneratedRadioButtonToXml(const QString& filePath, const QList<DraggableRadioButton*>& generateRadioButton,QString name);
    void saveGeneratedSliderToXml(const QString& filePath,const QList<DraggableSlider*>& generatedSlider,QString name);
    void saveGeneratedToolButtonToXml(const QString& filePath, const QList<DraggableToolButton*>& generatedToolButton,QString name);
    void saveGeneratedFrameToXml(const QString& filePath, const QList<DraggableFrame*>& generatedFrame,QString name);
    void saveGeneratedCommandLinkButton(const QString& filePath, const QList<DraggableCommandLinkButton*>& generatedCommandLinkButton,QString name);
    void saveGeneratedVolumeBarToXml(const QString& filePath,const QList<DraggableVolumeBar*>& generatedVolumeBar,QString name);
signals:
    void updateclick();
    void updatematch();
    void clearcontrol();
public:
    explicit savebutton(QWidget* parent = nullptr);
    template<typename T>
    void saveGeneratedControlsToXml(const QString& filePath, const QList<T*>& generatedControls,QString name);
    template<typename T>
    void saveGeneratednotextControlsToXml(const QString& filePath, const QList<T*>& generatedControls,QString name);
    
};

#endif // SAVEBUTTON_H
