#ifndef DRAGGABLETOOLBUTTON_H
#define DRAGGABLETOOLBUTTON_H
#include <QMouseEvent>
#include <QVBoxLayout>
#include "QToolButton"

class DraggableToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit DraggableToolButton(QWidget* parent = nullptr);
    static QList<DraggableToolButton*> generatedToolButton;
    QPoint PToolButton;
    QString name;
    bool signalEmitted=false;
    QString savePath;
    int borderstatus;
    void setpath(QString imagepath);
    QString imagePath;
    int matchId;
    int getmatchID();
    void setMatchId(int id);
    int imageCounter;
    QToolButton* control;
    void customcontrol();
    QString backimagePath;
    QString backsavePath;
    void copy(DraggableToolButton * toolbutton);
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
    void addImage();
    void changimage();
    void changbackgroundimage();
signals:
    void clicked();
};

#endif // DRAGGABLETOOLBUTTON_H
