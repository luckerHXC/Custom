#ifndef CLICKABLECOMBOBOX_H
#define CLICKABLECOMBOBOX_H
#include "QComboBox"

class ClickableCombobox : public QComboBox
{
    Q_OBJECT
public:
    ClickableCombobox(QWidget* parent = nullptr);
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // CLICKABLECOMBOBOX_H
