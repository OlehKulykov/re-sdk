
#ifndef GUIOBJECTSTREEITEM_H
#define GUIOBJECTSTREEITEM_H

#include <QWidget>
#include <QTreeWidgetItem>
#include "qreguiobjectshelper.h"

class QREObjectCommon;

class GUIObjectsTreeItem : public QTreeWidgetItem
{
private:
    QREObjectCommon * _guiObject;
    GUIObjectsTreeItem * _parentItem;
    QREGUIObjectType _guiType;
    bool _isHighLighted;
public:
    QREObjectCommon * GetGuiObject() const;
    GUIObjectsTreeItem * GetParentItem() const;
    QREGUIObjectType GetGuiType() const ;

    void SetGuiObject(QREObjectCommon * v);
    void SetParentItem(GUIObjectsTreeItem * v);
    void SetGuiType(QREGUIObjectType v);

    void HighLight(bool isH);
    bool IsHighLighted() const { return _isHighLighted; }

    GUIObjectsTreeItem(int type = Type );
    GUIObjectsTreeItem(QTreeWidget * parent, int type = Type);

    
};

#endif // GUIOBJECTSTREEITEM_H
