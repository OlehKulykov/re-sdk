
#include "guiobjectstreewidget.h"
#include "mainwindow.h"
#include <QEvent>
#include "qreview.h"
#include "qrebutton.h"
#include "qrelabel.h"
#include "qreparticleview.h"
#include "qretextfield.h"
#include "qrepropertymimedata.h"
#include "qrepropertymanager.h"
//#include <QDebug>

bool GUIObjectsTreeWidget::AddNewSubViewToSelected(QREGUIObjectType subViewType)
{
    GUIObjectsTreeItem * selItem = this->GetSelectedGUITreeItem();
    QREObjectCommon * selObject = this->GetSelectedGUIObject();
    if (selItem && selObject)
    {
        REView * selView = dynamic_cast<REView *>(selObject);
        GUIObjectsTreeItem * newItem = new GUIObjectsTreeItem();
        if (newItem && selView)
        {
            newItem->SetGuiType(subViewType);
            switch (subViewType)
            {
            case QREGUIObjectType_QREVIEW: {
                QREView * o = new QREView();
                if (o) { newItem->SetGuiObject(o); selView->AddSubView(o); o->SetAssignedTreeItem(newItem); o->Release(); }
            } break;
            case QREGUIObjectType_QREBUTTON: {
                QREButton * o = new QREButton();
                if (o) { newItem->SetGuiObject(o); selView->AddSubView(o); o->SetAssignedTreeItem(newItem); o->Release(); }
            } break;
            case QREGUIObjectType_QRELABEL: {
                QRELabel * o = new QRELabel();
                if (o) { newItem->SetGuiObject(o); selView->AddSubView(o); o->SetAssignedTreeItem(newItem); o->Release(); }
            } break;
            case QREGUIObjectType_QRETEXTFIELD: {
                QRETextField * o = new QRETextField();
                if (o) { newItem->SetGuiObject(o); selView->AddSubView(o); o->SetAssignedTreeItem(newItem); o->Release(); }
            } break;
            case QREGUIObjectType_QREPARTICLEVIEW: {
                QREParticleView * o = new QREParticleView();
                if (o) { newItem->SetGuiObject(o); selView->AddSubView(o); o->SetAssignedTreeItem(newItem); o->Release(); }
            } break;
            default: break;
            }

            if (newItem->GetGuiObject())
            {
                newItem->setText(0, QREGUIObjectsHelper::GetREClassNameStringForType(subViewType));
                selItem->addChild(newItem);
                newItem->SetParentItem(selItem);
            }
            else
            {
                delete newItem;
            }
        }
    }
    return false;
}

GUIObjectsTreeItem * GUIObjectsTreeWidget::GetItemAtPos(const QPoint & pos)
{
    QTreeWidgetItem * it = this->itemAt(pos);
    if (it)
    {
        return dynamic_cast<GUIObjectsTreeItem *>(it);
    }
    return NULL;
}

GUIObjectsTreeItem * GUIObjectsTreeWidget::GetSelectedGUITreeItem() const
{
    QList<QTreeWidgetItem*> items = this->selectedItems();
    if (items.count())
    {
        QTreeWidgetItem * selItem = items.at(0);
        if (selItem)
        {
            GUIObjectsTreeItem * guiItem = dynamic_cast<GUIObjectsTreeItem *>(selItem);
            if (guiItem)
            {
                return guiItem;
            }
        }
    }
    return NULL;
}

QREObjectCommon * GUIObjectsTreeWidget::GetSelectedGUIObject() const
{
    GUIObjectsTreeItem * treeItem = this->GetSelectedGUITreeItem();
    if (treeItem)
    {
        return treeItem->GetGuiObject();
    }
    return NULL;
}

void GUIObjectsTreeWidget::DeleteSelectedObject()
{
    QREObjectCommon * selObj = this->GetSelectedGUIObject();
    GUIObjectsTreeItem * treeItem = this->GetSelectedGUITreeItem();
    REView * view = dynamic_cast<REView*>(selObj);
    if (selObj && view && treeItem)
    {
        QREPropertyManager m;
        m.ObjectIsDeleted(selObj);
        this->removeItemWidget(treeItem, 0);
        delete treeItem;
        view->RemoveFromParentView();
    }
}

void GUIObjectsTreeWidget::PropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos)
{
    QREObjectCommon * guiObj = this->GetSelectedGUIObject();
    if (guiObj)
    {
        if (guiObj->OnPropertyCustomContextMenuRequested(property, pos))
        {
            this->OnSelectedItemsChanged();
        }
    }
}

void GUIObjectsTreeWidget::CreatePropertyManager()
{
    TreePropertyManager * pm = new TreePropertyManager(MainWindow::GetPropertyBrowser());
    if (pm)
    {
        if (pm->IsNotNULL())
        {
            _propertyManager = pm;
            return;
        }
    }

    if (pm)
    {
        delete pm;
    }
}

void GUIObjectsTreeWidget::ReloadSelectedObjectProperties()
{
    if (_propertyManager == NULL)
    {
        this->CreatePropertyManager();
    }

    if (_propertyManager)
    {
        _propertyManager->Clear();
        _propertyManager->AssignCurrentObject(NULL);


        QREObjectCommon * guiObj = this->GetSelectedGUIObject();
        if (guiObj)
        {
            _propertyManager->DisableOnChangeEvent();
            _propertyManager->AssignCurrentObject(guiObj);
            guiObj->SetProperties(_propertyManager, NULL);
            _propertyManager->EnableOnChangeEvent();
        }
    }
}

void GUIObjectsTreeWidget::OnSelectedItemsChanged()
{
    this->ReloadSelectedObjectProperties();

    if (_objectPropertiesDialog)
    {
        _objectPropertiesDialog->SetObject(this->GetSelectedGUIObject());
    }

    MainWindow::OnSelectedNewTreeObject();
}

void GUIObjectsTreeWidget::UnselectAllItems(bool blockOnChangeEvent)
{
    if (blockOnChangeEvent)
    {
        this->DisconnectOnSelectedItemsChanged();
    }
    QList<QTreeWidgetItem*> items(this->selectedItems());
    if (items.count() > 0)
    {
        for (int i = 0; i < items.count(); i++)
        {
            QTreeWidgetItem * selItem = items.at(i);
            selItem->setSelected(false);
        }
    }
    if (blockOnChangeEvent)
    {
        this->ConnectOnSelectedItemsChanged();
    }
}

void GUIObjectsTreeWidget::AddViewsToItem(REObjectsArray * arr, GUIObjectsTreeItem * item)
{
    if (arr && item)
    {
        for (REUInt32 i = 0; i < arr->Count(); i++)
        {
            GUIObjectsTreeItem * newItem = new GUIObjectsTreeItem();
            REView * view = dynamic_cast<REView*>(arr->At(i));
            QREObjectCommon * comObj = QREGUIObjectsHelper::GetCommonObject(arr->At(i));
            const QREGUIObjectType type = QREGUIObjectsHelper::GetType(comObj);
            if (view && newItem && comObj && (type != QREGUIObjectType_NONE))
            {
                newItem->SetParentItem(item);
                newItem->SetGuiObject(comObj);
                newItem->SetGuiType(type);
                comObj->SetAssignedTreeItem(newItem);
                newItem->setText(0, QREGUIObjectsHelper::GetREClassNameStringForType(type));
                item->addChild(newItem);
                GUIObjectsTreeWidget::AddViewsToItem(view->GetSubViewsArray(), newItem);
            }
            else
            {
                RE_SAFE_DELETE(newItem);
            }
        }
    }
}

void GUIObjectsTreeWidget::ReloadRootVCTreeViews()
{
    this->Clear();

    if (_rootVC)
    {
        GUIObjectsTreeItem * newItem = new GUIObjectsTreeItem();
        QREObjectCommon * comObj = QREGUIObjectsHelper::GetCommonObject(_rootVC);
        if (newItem && comObj)
        {
            comObj->SetAssignedTreeItem(newItem);
            newItem->SetParentItem(NULL);
            newItem->SetGuiObject(comObj);
            newItem->SetGuiType(QREGUIObjectsHelper::GetType(_rootVC));
            newItem->setText(0, "Root view controller");
            this->addTopLevelItem(newItem);

            GUIObjectsTreeWidget::AddViewsToItem(_rootVC->GetSubViewsArray(), newItem);
        }
        this->expandAll();
    }
}

void GUIObjectsTreeWidget::SetRootVC(QREViewController * rootVC)
{
    this->Clear();
    _rootVC = rootVC;
    if (_rootVC)
    {
        this->ReloadRootVCTreeViews();
    }
}


void GUIObjectsTreeWidget::OnCustomContextMenuRequested(const QPoint & pos)
{
    QREObjectCommon * selObi = this->GetSelectedGUIObject();
    if (selObi && _objectPropertiesDialog)
    {
        _objectPropertiesDialog->SetObject(selObi);
        if (_objectPropertiesDialog->isHidden())
        {
            _objectPropertiesDialog->show();
        }
    }
}

void GUIObjectsTreeWidget::Clear()
{
    this->clear();
}

void GUIObjectsTreeWidget::dropEvent(QDropEvent *de)
{
    QPoint pos = de->pos();

    GUIObjectsTreeItem * item = this->GetItemAtPos(pos);
    if (item) item->HighLight(false);
    if (_highLightedItem != item)
    {
        if (_highLightedItem) _highLightedItem->HighLight(false);
        _highLightedItem = NULL;
    }

    const QMimeData * md = de->mimeData();
    if (md)
    {
        const QREPropertyMimeData * mimeData = dynamic_cast<const QREPropertyMimeData *>(md);
        if (mimeData && item)
        {
            QREObjectCommon * com = item->GetGuiObject();
            QREProperty * prop = mimeData->GetProperty();
            if (com && prop)
            {
                REObject * reobj = com->reobject();
                prop->SetObject(reobj);
                com->AssignEditorIDQ();
                if (com->GetEditorIDQ())
                {
                    if (mimeData->GetRadioButton())
                    {
                        mimeData->GetRadioButton()->setChecked(true);
                    }
                    this->ReloadSelectedObjectProperties();
                }
            }
        }
    }
}

void GUIObjectsTreeWidget::dragMoveEvent(QDragMoveEvent *de)
{
    QPoint pos = de->pos();
    GUIObjectsTreeItem * item = this->GetItemAtPos(pos);

    if (item) item->HighLight(true);
    if (_highLightedItem != item)
    {
        if (_highLightedItem) _highLightedItem->HighLight(false);
        _highLightedItem = item;
    }

    de->accept();
}

void GUIObjectsTreeWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (_highLightedItem)
    {
        _highLightedItem->HighLight(false);
        _highLightedItem = NULL;
    }
    event->acceptProposedAction();
}

void GUIObjectsTreeWidget::dragLeaveEvent(QDragLeaveEvent * event)
{
    if (_highLightedItem)
    {
        _highLightedItem->HighLight(false);
        _highLightedItem = NULL;
    }
}

GUIObjectsTreeWidget::GUIObjectsTreeWidget(QWidget *parent) :
    QTreeWidget(parent),
    _rootVC(NULL),
    _propertyManager(NULL),
    _objectPropertiesDialog(NULL),
    _highLightedItem(NULL)
{
    _objectPropertiesDialog = new ObjectPropsDialog(this);

    this->ConnectOnSelectedItemsChanged();

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(OnCustomContextMenuRequested(QPoint)));

    this->setAcceptDrops(true);
}

GUIObjectsTreeWidget::~GUIObjectsTreeWidget()
{
    disconnect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(OnCustomContextMenuRequested(QPoint)));
    RE_SAFE_DELETE(_propertyManager);
}

void GUIObjectsTreeWidget::ConnectOnSelectedItemsChanged()
{
    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(OnSelectedItemsChanged()));
}

void GUIObjectsTreeWidget::DisconnectOnSelectedItemsChanged()
{
    disconnect(this, SIGNAL(itemSelectionChanged()), this, SLOT(OnSelectedItemsChanged()));
}

