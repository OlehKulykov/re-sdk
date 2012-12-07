
#ifndef QREPROPERTYRADIOBUTTON_H
#define QREPROPERTYRADIOBUTTON_H

#include <QRadioButton>
#include <QWidget>
#include <QMimeData>
#include <QDrag>
#include <QMouseEvent>

class QREProperty;

class QREPropertyRadioButton : public QRadioButton
{
protected:
    virtual void mousePressEvent(QMouseEvent *e);
public:
    QString dragText;
    QREProperty * property;
    QREPropertyRadioButton(QWidget *parent=0);
    virtual ~QREPropertyRadioButton();
};

#endif // QREPROPERTYRADIOBUTTON_H
