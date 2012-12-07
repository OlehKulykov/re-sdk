
#include "qrepropertylistwidgetitem.h"
#include "qreobject.h"

bool QREPropertyListWidgetItem::HighLight(bool isH)
{
    if (this->IsHighLighted() != isH)
    {
        if (_property)
        {
            QREObjectCommon * com = _property->GetQREObjectCommon();
            if (com)
            {
                if (com->GetAssignedTreeItem())
                {
                    _isHighLighted = isH;
                    com->GetAssignedTreeItem()->HighLight(isH);
                }
            }
        }
    }
    return true;
}

void QREPropertyListWidgetItem::SetProperty(QREProperty * p)
{
    this->HighLight(false);
    _property = p;
}

QREPropertyListWidgetItem::QREPropertyListWidgetItem(QListWidget *view, int type) :
    QListWidgetItem(view, type),
    _property(NULL),
    _isHighLighted(false)
{

}

QREPropertyListWidgetItem::~QREPropertyListWidgetItem()
{

}

