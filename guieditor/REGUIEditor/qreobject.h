
#ifndef QREOBJECT_H
#define QREOBJECT_H

#include <recore.h>
#include <regui.h>
#include "treepropertymanager.h"
#include "qreguiobjectshelper.h"
#include "guiobjectstreeitem.h"
#include "reqtconverter.h"
#include "qstringpath.h"
#include "qreproperty.h"
#include "savevcinfo.h"
#include <QList>

class QREObjectCommon
{
protected:
    typedef struct _qreObjectProps
    {
        QtProperty * item;
        QtVariantProperty * editorid;
        QtVariantProperty * className;
    } QREObjectProps;
    QREObjectProps _qreObjProps;
private:
    static void CreateREObjectProperties(QREObjectProps * props,
                                         QtVariantPropertyManager * manager);
    void REObjectInfoToProperties();
    static QList<REUInt32> _editorIDsList;
protected:
    REUInt32 _editorid;
    QString _className;
    GUIObjectsTreeItem * _assignedTreeItem;
    bool _isHighLighted;
    QREProperty * GetOrCreatePropertyForKey(const char * key);
    QString GetREObjectInfoXMLString(QString linePrefix);
public:
    bool DeleteProperty(QREProperty * p);
    static REUInt32 CreateNewEditorID();
    static bool DeleteEditorID(REUInt32 editID);

    bool IsPropertyExistsWithNameQ(const QString & name);
    QREProperty * CreateNewPropertyWithNameQ(const QString & name);

    virtual void OnPropertyPointedToYouIsDeleted();

    void SetClassNameQ(const char * cl) { _className = cl; }
    GUIObjectsTreeItem * GetAssignedTreeItem() const { return _assignedTreeItem; }
    void SetAssignedTreeItem(GUIObjectsTreeItem * ti) { _assignedTreeItem = ti; }

    virtual REObject * reobject() = 0;
    virtual const REUIdentifier GetClassIdentifierQ() const = 0;
    virtual const REUIdentifier GetObjectIdentifierQ() const = 0;
    virtual void ReleaseQ() = 0;
    virtual void RetainQ() = 0;
    virtual void HighLightQ(bool isH) { _isHighLighted = isH; }
    bool IsHighLighted() const { return _isHighLighted; }

    virtual void SetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL);
    virtual bool OnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue);
    virtual bool OnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos);

    void AssignEditorIDQ();
    const REUInt32 GetEditorIDQ() const { return _editorid; }
    QString GetClassNameQ();
    QList<QREProperty *> GetProps();

    virtual QString GetXMLString(QString linePrefix, QString keyPurpose = QString("")) { return QString(""); }

    virtual void SaveQ(SaveVCInfo * info) {  }

    QREObjectCommon();
    virtual ~QREObjectCommon();

    static void RemoveAllSubProperties(QtProperty * property);
};





class QREObject : public REGUIObject, public QREObjectCommon
{
public:
    /// QREObjectCommon
    virtual REObject * reobject() { return this; }
    virtual const REUIdentifier GetClassIdentifierQ() const { return this->GetClassIdentifier(); }
    virtual const REUIdentifier GetObjectIdentifierQ() const { return this->GetObjectIdentifier(); }
    virtual void ReleaseQ() { this->Release(); }
    virtual void RetainQ() { this->Retain(); }

    QREObject();
    virtual ~QREObject();
};


#endif // QREOBJECT_H

