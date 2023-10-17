#ifndef DRAGGABLELINEEDIT_H
#define DRAGGABLELINEEDIT_H
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QLabel>
#include "QLineEdit"
#include "userlineedit.h"

class DraggableLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit DraggableLineEdit(QWidget* parent = nullptr);
    static QList<DraggableLineEdit*> generatedLineEdit;
    QPoint PlineEdit;
    QString name;
    bool signalEmitted=false;
    UserLineEdit *lineEdit= nullptr;
    int matchId;
    void setMatchId(int id);
    int getmatchID();
    int imageCounter;
    QString imagePath;
    QString savePath;
    void customcontrol();
    QString backimagePath;
    QString backsavePath;
    void copy(DraggableLineEdit * LineEdit);
    int frameid;
    int layers;
    bool lock=true;
    bool drawblue=false;
protected:
    void mousePressEvent(QMouseEvent* event) override;//鼠标按下事件
    void mouseMoveEvent(QMouseEvent* event) override;//鼠标移动事件
    void mouseReleaseEvent(QMouseEvent* event) override;//松开鼠标事件
    void showContextMenu(const QPoint& pos);//菜单栏展示
    void wheelEvent(QWheelEvent* event);//缩放
    void Tree();
    void paintEvent(QPaintEvent* event) override;

public slots:
    void remove();//移除控件
    void changeproperty();
    void clicktree();
    void changimage();
    void changbackgroundimage();
signals:
    void clicked();
};

#endif // DRAGGABLELINEEDIT_H
