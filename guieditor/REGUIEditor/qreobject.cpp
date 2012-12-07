
#include "qreobject.h"
#include "qrepropertymanager.h"

QList<REUInt32> QREObjectCommon::_editorIDsList;

void QREObjectCommon::CreateREObjectProperties(QREObjectProps * props,
                                               QtVariantPropertyManager * manager)
{
    /*
    m_constraintAttribute(QLatin1String("constraint")),
    m_singleStepAttribute(QLatin1String("singleStep")),
    m_decimalsAttribute(QLatin1String("decimals")),
    m_enumIconsAttribute(QLatin1String("enumIcons")),
    m_enumNamesAttribute(QLatin1String("enumNames")),
    m_flagNamesAttribute(QLatin1String("flagNames")),
    m_maximumAttribute(QLatin1String("maximum")),
    m_minimumAttribute(QLatin1String("minimum")),
    m_regExpAttribute(QLatin1String("regExp"))
    */

    props->item = manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("REObject"));

    props->editorid = manager->addProperty(QVariant::Int, QLatin1String("Editor ID"));
    props->editorid->setEnabled(false);
    props->item->addSubProperty(props->editorid);

    props->className = manager->addProperty(QVariant::String, QLatin1String("Class Name"));
    props->item->addSubProperty(props->className);
}

void QREObjectCommon::REObjectInfoToProperties()
{
    if (_qreObjProps.editorid)
    {
        _qreObjProps.editorid->setValue(QVariant(this->GetEditorIDQ()));
    }

    if (_qreObjProps.className)
    {
        _qreObjProps.className->setValue(this->GetClassNameQ());
    }
}

QString QREObjectCommon::GetREObjectInfoXMLString(QString linePrefix)
{
    QString xml;

    if (_editorid)
    {
        xml += linePrefix + "<integer key=\"editorid\">" + QString::number(_editorid) + "</integer>\n";
    }

    QList<QREProperty *> props;
    QREPropertyManager m;
    m.GetPropertiesForObject(this, &props);
    if (!props.isEmpty())
    {
        for (int i = 0; i < props.count(); i++)
        {
            QREProperty * p = props.at(i);
            const REUInt32 eID = p->GetObjectEditorIDQ();
            if (eID && (!p->GetKeyStringQ().isEmpty()))
            {
                xml += linePrefix + "<property key=\"" + p->GetKeyStringQ() + "\">" + QString::number(eID) + "</property>\n";
            }
        }
    }

    return xml;
}

void QREObjectCommon::SetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL)
{
    if (TreePropertyManager::IsNotNULL(propertyManager))
    {
        if (_qreObjProps.item == NULL)
        {
            QREObjectCommon::CreateREObjectProperties(&_qreObjProps, propertyManager->GetVariantManager());
        }

        this->REObjectInfoToProperties();

        if (_qreObjProps.item)
        {
            if (parentItemOrNULL) { parentItemOrNULL->addSubProperty(_qreObjProps.item); }
            else { propertyManager->GetPropertyBrowser()->addProperty(_qreObjProps.item); }
        }
    }
}

bool QREObjectCommon::OnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue)
{
    if (property && newValue.isValid())
    {
        if (property == _qreObjProps.className)
        {
            _className = newValue.toString();
            return true;
        }
    }
    return false;
}

bool QREObjectCommon::OnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos)
{
    return false;
}

QREProperty * QREObjectCommon::GetOrCreatePropertyForKey(const char * key)
{
    QREPropertyManager m;
    return m.GetOrCreatePropertyForKey(key, this);
}

QString QREObjectCommon::GetClassNameQ()
{
    if (_className.isEmpty())
    {
        const QREGUIObjectType type = QREGUIObjectsHelper::GetType(this);
        _className = QREGUIObjectsHelper::GetREClassNameStringForType(type);
    }
    return _className;
}

bool QREObjectCommon::IsPropertyExistsWithNameQ(const QString & name)
{
    QList<QREProperty *> props;
    QREPropertyManager m;
    m.GetPropertiesForObject(this, &props);
    for (int i = 0; i < props.count(); i++)
    {
        QREProperty * p = props.at(i);
        if (name.compare(p->GetKeyStringQ(), Qt::CaseSensitive) == 0)
        {
            return true;
        }
    }
    return false;
}

QREProperty * QREObjectCommon::CreateNewPropertyWithNameQ(const QString & name)
{
    QREPropertyManager m;
    return m.CreateNewPropertyWithNameQ(name, this);
}

bool QREObjectCommon::DeleteProperty(QREProperty * p)
{
    QREPropertyManager m;
    return m.DeleteProperty(p, this);
}

void QREObjectCommon::AssignEditorIDQ()
{
    if (_editorid == 0)
    {
        _editorid = QREObjectCommon::CreateNewEditorID();
    }
}

void QREObjectCommon::OnPropertyPointedToYouIsDeleted()
{
    QREPropertyManager m;
    if (!m.IsHasPropertiesPointedToObject(this))
    {
        if (QREObjectCommon::DeleteEditorID(_editorid))
        {
            _editorid = 0;
        }
    }
}

QREObjectCommon::QREObjectCommon()
{
    memset(&_qreObjProps, 0, sizeof(QREObjectProps));

    _editorid = 0;
    _assignedTreeItem = NULL;
    _isHighLighted = false;
}

QList<QREProperty *> QREObjectCommon::GetProps()
{
    QList<QREProperty *> list;
    QREPropertyManager m;
    m.GetPropertiesForObject(this, &list);
    return list;
}

QREObjectCommon::~QREObjectCommon()
{
    QREObjectCommon::RemoveAllSubProperties(_qreObjProps.item);

    QREObjectCommon::DeleteEditorID(_editorid);
}

REUInt32 QREObjectCommon::CreateNewEditorID()
{
    for (REUInt32 i = 1; i < REUInt32Max; i++)
    {
        if (!_editorIDsList.contains(i))
        {
            _editorIDsList.append(i);
            return i;
        }
    }
    return 0;
}

bool QREObjectCommon::DeleteEditorID(REUInt32 editID)
{
    if (editID)
    {
        const int before = _editorIDsList.count();
        _editorIDsList.removeOne(editID);
        return (before != _editorIDsList.count());
    }
    return false;
}

void QREObjectCommon::RemoveAllSubProperties(QtProperty * property)
{
    if (property)
    {
        QList<QtProperty *> list(property->subProperties());
        if (!list.isEmpty())
        {
            for (int i = 0; i < list.count(); i++)
            {
                QtProperty * p = list.at(i);
                if (p)
                {
                    property->removeSubProperty(p);
                }
            }
        }
    }
}






QREObject::QREObject() : REGUIObject(), QREObjectCommon()
{

}

QREObject::~QREObject()
{

}

