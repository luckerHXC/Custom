#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H
#include <QLabel>

class ClickableLabel : public QLabel
{
Q_OBJECT

protected:
    void mousePressEvent(QMouseEvent* event) override;
public:
    explicit ClickableLabel(QWidget* parent = nullptr);
signals:
    void clicked();
};

#endif // CLICKABLELABEL_H
