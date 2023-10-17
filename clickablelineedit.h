#ifndef CLICKABLELINEEDIT_H
#define CLICKABLELINEEDIT_H
#include <QLineEdit>

class ClickableLineEdit : public QLineEdit
{
    Q_OBJECT
protected:
    void mousePressEvent(QMouseEvent* event) override;
public:
    explicit ClickableLineEdit(QWidget* parent = nullptr);
signals:
    void clicked();
};

#endif // CLICKABLELINEEDIT_H
