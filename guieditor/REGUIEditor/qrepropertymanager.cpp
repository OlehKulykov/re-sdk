
#include "qrepropertymanager.h"
#include <recore.h>

QList<QREPropertyManager::PropStruct> QREPropertyManager::_props;

QREProperty * QREPropertyManager::GetOrCreateProperty(const char * key, QREObjectCommon * obj)
{
    QString keyStr(key);
    for (int i = 0; i < _props.count(); i++)
    {
        QREPropertyManager::PropStruct s = _props.at(i);
        if (s.holder->GetObjectIdentifierQ() == obj->GetObjectIdentifierQ())
        {
            QREProperty * p = s.property;
            if (keyStr.compare(p->GetKeyStringQ(), Qt::CaseSensitive) == 0)
            {
                return p;
            }
        }
    }

    QREProperty * newProp = new QREProperty();
    if (newProp)
    {
        newProp->SetKeyStringQ(keyStr);
        QREPropertyManager::PropStruct s;
        s.holder = obj;
        s.property = newProp;
        _props.append(s);
        return newProp;
    }

    return NULL;
}

void QREPropertyManager::PropertiesForObject(QREObjectCommon * obj, QList<QREProperty *> * list)
{
    for (int i = 0; i < _props.count(); i++)
    {
        QREPropertyManager::PropStruct s = _props.at(i);
        if (s.holder->GetObjectIdentifierQ() == obj->GetObjectIdentifierQ())
        {
            list->append(s.property);
        }
    }
}

QREProperty * QREPropertyManager::NewPropertyWithNameQ(const QString & name, QREObjectCommon * obj)
{
    QREProperty * p = new QREProperty();
    if (p)
    {
        QREPropertyManager::PropStruct s;
        s.holder = obj;
        s.property = p;
        p->SetKeyStringQ(name);
        _props.append(s);
        return p;
    }
    return NULL;
}

QREProperty * QREPropertyManager::CreateNewPropertyWithNameQ(const QString & name, QREObjectCommon * obj)
{
    if ((!name.isEmpty()) && obj) return QREPropertyManager::NewPropertyWithNameQ(name, obj);
    return NULL;
}

void QREPropertyManager::GetPropertiesForObject(QREObjectCommon * obj, QList<QREProperty *> * list)
{
    if (obj && list) QREPropertyManager::PropertiesForObject(obj, list);
}

QREProperty * QREPropertyManager::GetOrCreatePropertyForKey(const char * key, QREObjectCommon * obj)
{
    if (key && obj) return QREPropertyManager::GetOrCreateProperty(key, obj);
    return NULL;
}

bool QREPropertyManager::DelProperty(QREProperty * p, QREObjectCommon * obj)
{
    const REUIdentifier propID = p->GetObjectIdentifier();
    const REUIdentifier objID = obj->GetObjectIdentifierQ();
    bool retRes = false;
    bool isSearching = true;
    while (isSearching)
    {
        bool isContinueSearching = false;
        int i = 0;
        while (i < _props.count())
        {
            QREPropertyManager::PropStruct s = _props.at(i);
            if ((s.holder->GetObjectIdentifierQ() == objID) &&
                    (s.property->GetObjectIdentifier() == propID))
            {
                _props.removeAt(i);
                QREObjectCommon * pointedComObject = s.property->GetQREObjectCommon();
                s.property->SetObject(NULL);
                if (pointedComObject) pointedComObject->OnPropertyPointedToYouIsDeleted();
                i = _props.count();
                retRes = true;
                isContinueSearching = true;
            }
            i++;
        }
        isSearching = isContinueSearching;
    }

    if (retRes)
    {
        delete p;
    }

    return retRes;
}

bool QREPropertyManager::DeleteProperty(QREProperty * p, QREObjectCommon * obj)
{
    if (p && obj) return QREPropertyManager::DelProperty(p, obj);
    return false;
}

void QREPropertyManager::ObjectIsDel(QREObjectCommon * obj)
{
    const REUIdentifier objID = obj->GetObjectIdentifierQ();
    /// del all object props
    bool isSearching = true;
    while (isSearching)
    {
        bool isContinueSearching = false;
        int i = 0;
        while (i < _props.count())
        {
            QREPropertyManager::PropStruct s = _props.at(i);
            if (s.holder->GetObjectIdentifierQ() == objID)
            {
                _props.removeAt(i);
                QREObjectCommon * pointedComObject = s.property->GetQREObjectCommon();
                s.property->SetObject(NULL);
                delete s.property;
                if (pointedComObject) pointedComObject->OnPropertyPointedToYouIsDeleted();
                i = _props.count();
                isContinueSearching = true;
            }
            i++;
        }
        isSearching = isContinueSearching;
    }


    /// del all props witch points to object
    isSearching = true;
    while (isSearching)
    {
        bool isContinueSearching = false;
        int i = 0;
        while (i < _props.count())
        {
            QREPropertyManager::PropStruct s = _props.at(i);
            QREObjectCommon * comObj = s.property->GetQREObjectCommon();
            if (comObj)
            {
                if (comObj->GetObjectIdentifierQ() == objID)
                {
                    _props.removeAt(i);
                    QREObjectCommon * pointedComObject = s.property->GetQREObjectCommon();
                    s.property->SetObject(NULL);
                    delete s.property;
                    if (pointedComObject) pointedComObject->OnPropertyPointedToYouIsDeleted();
                    i = _props.count();
                    isContinueSearching = true;
                }
            }
            i++;
        }
        isSearching = isContinueSearching;
    }
}

void QREPropertyManager::ObjectIsDeleted(QREObjectCommon * obj)
{
    if (obj) QREPropertyManager::ObjectIsDel(obj);
}

bool QREPropertyManager::IsHasPropsPointedToObject(QREObjectCommon * obj)
{
    const REUIdentifier objID = obj->GetObjectIdentifierQ();
    for (int i = 0; i < _props.count(); i++)
    {
        QREPropertyManager::PropStruct s = _props.at(i);
        QREObjectCommon * comObj = s.property->GetQREObjectCommon();
        if (comObj)
        {
            if (comObj->GetObjectIdentifierQ() == objID)
            {
               return true;
            }
        }
    }
    return false;
}

bool QREPropertyManager::IsHasPropertiesPointedToObject(QREObjectCommon * obj)
{
    if (obj) return QREPropertyManager::IsHasPropsPointedToObject(obj);
    return false;
}

QREPropertyManager::QREPropertyManager()
{

}

QREPropertyManager::~QREPropertyManager()
{

}

