#ifndef DRAGGABLELABEL_H
#define DRAGGABLELABEL_H
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QLabel>
#include "QLineEdit"
class DraggableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit DraggableLabel(QWidget* parent = nullptr);
    static QList<DraggableLabel*> generatedLabels;
    QPoint Plabel;
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
    QLabel* control;
    void customcontrol();
    QString backimagePath;
    QString backsavePath;
    void copy(DraggableLabel * label);
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
    void setEditable(bool editable);
    void mouseDoubleClickEvent(QMouseEvent* event);
    void finishEditing();
    void Tree();
    QLineEdit* lineEdit = nullptr;
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

#endif // DRAGGABLELABEL_H
