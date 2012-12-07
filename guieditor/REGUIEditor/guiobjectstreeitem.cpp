
#include "guiobjectstreeitem.h"
#include "qreobject.h"

void GUIObjectsTreeItem::HighLight(bool isH)
{
    if (this->IsHighLighted() != isH)
    {
        _isHighLighted = isH;
        this->setTextColor(0, isH ? QColor::fromRgb(255, 0, 0) : QColor::fromRgb(0, 0, 0));
        if (_guiObject)
        {
            _guiObject->HighLightQ(isH);
        }
    }
}

QREObjectCommon * GUIObjectsTreeItem::GetGuiObject() const
{
    return _guiObject;
}

GUIObjectsTreeItem * GUIObjectsTreeItem::GetParentItem() const
{
    return _parentItem;
}

QREGUIObjectType GUIObjectsTreeItem::GetGuiType() const
{
    return _guiType;
}

void GUIObjectsTreeItem::SetGuiObject(QREObjectCommon * v)
{
    _guiObject = v;
}

void GUIObjectsTreeItem::SetParentItem(GUIObjectsTreeItem * v)
{
    _parentItem = v;
}

void GUIObjectsTreeItem::SetGuiType(QREGUIObjectType v)
{
    _guiType = v;
}

GUIObjectsTreeItem::GUIObjectsTreeItem(int type) :
    QTreeWidgetItem(type)
{
    _guiObject = NULL;
    _parentItem = NULL;
    _guiType = QREGUIObjectType_NONE;
    _isHighLighted = false;
    this->HighLight(false);
}

GUIObjectsTreeItem::GUIObjectsTreeItem (QTreeWidget * parent, int type ) :
    QTreeWidgetItem(parent, type)
{
    _guiObject = NULL;
    _parentItem = NULL;
    _guiType = QREGUIObjectType_NONE;
    _isHighLighted = false;
    this->HighLight(false);
}

