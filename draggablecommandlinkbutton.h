#ifndef DRAGGABLECOMMANDLINKBUTTON_H
#define DRAGGABLECOMMANDLINKBUTTON_H
#include "QCommandLinkButton"
#include <QMouseEvent>
#include <QVBoxLayout>

class DraggableCommandLinkButton : public QCommandLinkButton
{
    Q_OBJECT

public:
    explicit DraggableCommandLinkButton(QWidget* parent = nullptr);
    static QList<DraggableCommandLinkButton*> generatedCommandLinkButton;
    QPoint PCommandLinkButton;
    QString name;
    bool signalEmitted=false;
    bool m_isResizing;
    QPoint m_resizeStartPosition;
    int matchId;
    int getmatchID();
    void setMatchId(int id);
    int imageCounter;
    QString imagePath;
    QString savePath;
    QCommandLinkButton * control;
    void customcontrol();
    QString backimagePath;
    QString backsavePath;
    void copy(DraggableCommandLinkButton * commandlinkbutton);
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
    void changecolor();
    void paintEvent(QPaintEvent* event) override;




public slots:
    void remove();//移除控件
    void changeproperty();
    void clicktree();
    void changimage();
    void changbackgroundimage();
signals:
    void clicked();


private:
    bool functionCalled = false;
};

#endif // DRAGGABLECOMMANDLINKBUTTON_H
