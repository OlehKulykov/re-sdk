
#ifndef GUIOBJECTSTREEWIDGET_H
#define GUIOBJECTSTREEWIDGET_H

#include <QTreeWidget>
#include "qreviewcontroller.h"
#include "guiobjectstreeitem.h"
#include "treepropertymanager.h"
#include "objectpropsdialog.h"
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>

class GUIObjectsTreeItem;

class GUIObjectsTreeWidget : public QTreeWidget
{
    Q_OBJECT

private:
    QREViewController * _rootVC;
    TreePropertyManager * _propertyManager;
    ObjectPropsDialog * _objectPropertiesDialog;
    GUIObjectsTreeItem * _highLightedItem;

    void CreatePropertyManager();
    void ConnectOnSelectedItemsChanged();
    void DisconnectOnSelectedItemsChanged();
    static void AddViewsToItem(REObjectsArray * arr, GUIObjectsTreeItem * item);
protected:
    virtual void dropEvent(QDropEvent *de);
    virtual void dragMoveEvent(QDragMoveEvent *de);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragLeaveEvent (QDragLeaveEvent * event);
public:
    GUIObjectsTreeItem * GetItemAtPos(const QPoint & pos);
    GUIObjectsTreeItem * GetSelectedGUITreeItem() const;
    QREObjectCommon * GetSelectedGUIObject() const;
    bool AddNewSubViewToSelected(QREGUIObjectType subViewType);
    void ReloadSelectedObjectProperties();
    void SetRootVC(QREViewController * rootVC);
    QREViewController * GetRootVC() const { return _rootVC; }
    void ReloadRootVCTreeViews();
    void Clear();
    void UnselectAllItems(bool blockOnChangeEvent);
    void PropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos);
    void DeleteSelectedObject();

    explicit GUIObjectsTreeWidget(QWidget *parent = 0);
    virtual ~GUIObjectsTreeWidget();
signals:
    
public slots:
    void OnSelectedItemsChanged();
    void OnCustomContextMenuRequested(const QPoint & pos);
};

#endif // GUIOBJECTSTREEWIDGET_H
