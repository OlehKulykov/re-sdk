
#ifndef QREPROPERTYMANAGER_H
#define QREPROPERTYMANAGER_H

#include "qreproperty.h"
#include "qreobject.h"
#include <QList>

class QREPropertyManager
{
private:
    typedef struct _propStruct
    {
        QREProperty * property;
        QREObjectCommon * holder;
    } PropStruct;
    static QList<QREPropertyManager::PropStruct> _props;
    static QREProperty * GetOrCreateProperty(const char * key, QREObjectCommon * obj);
    static void PropertiesForObject(QREObjectCommon * obj, QList<QREProperty *> * list);
    static QREProperty * NewPropertyWithNameQ(const QString & name, QREObjectCommon * obj);
    static bool DelProperty(QREProperty * p, QREObjectCommon * obj);
    static void ObjectIsDel(QREObjectCommon * obj);
    static bool IsHasPropsPointedToObject(QREObjectCommon * obj);
public:
    QREProperty * GetOrCreatePropertyForKey(const char * key, QREObjectCommon * obj);
    void GetPropertiesForObject(QREObjectCommon * obj, QList<QREProperty *> * list);
    QREProperty * CreateNewPropertyWithNameQ(const QString & name, QREObjectCommon * obj);
    bool DeleteProperty(QREProperty * p, QREObjectCommon * obj);
    void ObjectIsDeleted(QREObjectCommon * obj);
    bool IsHasPropertiesPointedToObject(QREObjectCommon * obj);
    QREPropertyManager();
    ~QREPropertyManager();
};


#endif // QREPROPERTYMANAGER_H
