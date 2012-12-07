
#ifndef QREPROPERTYLISTWIDGETITEM_H
#define QREPROPERTYLISTWIDGETITEM_H

#include <QListWidget>
#include <QListWidgetItem>
#include "qreproperty.h"

class QREPropertyListWidgetItem : public QListWidgetItem
{
private:
    QREProperty * _property;
    bool _isHighLighted;
public:
    bool HighLight(bool isH);
    bool IsHighLighted() const { return _isHighLighted; }

    QREProperty * GetProperty() const { return _property; }
    void SetProperty(QREProperty * p);

    QREPropertyListWidgetItem(QListWidget * view = 0, int type = Type);
    virtual ~QREPropertyListWidgetItem();
};

#endif // QREPROPERTYLISTWIDGETITEM_H
