
#ifndef QREPROPERTYLISTITEM_H
#define QREPROPERTYLISTITEM_H

#include <QWidget>
#include <QListWidgetItem>
#include "qreobject.h"
#include "qrepropertyradiobutton.h"

namespace Ui
{
class QREPropertyListItem;
}

class QREPropertyListItem : public QWidget
{
    Q_OBJECT
private:
    QListWidgetItem * _listWidgetItem;
    QREProperty * _property;
    QREObjectCommon * _objectThatContainesProperty;

public:
    void SetListWidgetItem(QListWidgetItem * item);
    QListWidgetItem * GetListWidgetItem() const { return _listWidgetItem; }

    QREProperty * GetProperty() const { return _property; }
    void SetProperty(QREProperty * p);

    QREObjectCommon * GetObjectThatContainesProperty() const { return _objectThatContainesProperty; }
    void SetObjectThatContainesProperty(QREObjectCommon * o);

    QREPropertyListItem(QWidget * parent = 0);
    virtual ~QREPropertyListItem();
    
private slots:
    void on_deletePropButton_clicked();

private:
    Ui::QREPropertyListItem * ui;
};

#endif // QREPROPERTYLISTITEM_H

