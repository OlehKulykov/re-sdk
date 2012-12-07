
#ifndef QREPROPERTYMIMEDATA_H
#define QREPROPERTYMIMEDATA_H

#include <QMimeData>
#include "qreproperty.h"
#include "qrepropertyradiobutton.h"

class QREPropertyMimeData : public QMimeData
{
private:
    QREProperty * _property;
    QREPropertyRadioButton * _radioButton;
    QREObjectCommon * _senderObject;
public:
    QREObjectCommon * GetSenderObject() const { return _senderObject; }
    void SetSenderObject(QREObjectCommon * sender) { _senderObject = sender; }

    QREProperty * GetProperty() const { return _property; }
    void SetProperty(QREProperty * p) { _property = p; }

    QREPropertyRadioButton * GetRadioButton() const { return _radioButton; }
    void SetRadioButton(QREPropertyRadioButton * b) { _radioButton = b; }

    QREPropertyMimeData();
    virtual ~QREPropertyMimeData();
};

#endif // QREPROPERTYMIMEDATA_H
