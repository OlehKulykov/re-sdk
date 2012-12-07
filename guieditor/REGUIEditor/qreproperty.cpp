
#include "qreproperty.h"
#include "qreobject.h"
#include "qreguiobjectshelper.h"

/// Settes property value from object.
void QREProperty::SetObject(REObject * v)
{
    if (_comObj)
    {
        _comObj->ReleaseQ();
        _comObj = NULL;
    }

    if (v)
    {
        QREObjectCommon * comObj = QREGUIObjectsHelper::GetCommonObject(v);
        if (comObj)
        {
            const QREGUIObjectType t = QREGUIObjectsHelper::GetType(comObj);
            if (t != QREGUIObjectType_NONE)
            {
                _comObj = comObj;
                _comObj->RetainQ();
            }
        }
    }
}

/// Getter for pointer of object.
REObject * QREProperty::GetObject() const
{
    return (REObject*)_comObj;
}

QREObjectCommon * QREProperty::GetQREObjectCommon() const
{
    return _comObj;
}

REUInt32 QREProperty::GetObjectEditorIDQ() const
{
    if (_comObj)
    {
        return _comObj->GetEditorIDQ();
    }
    return 0;
}

QREProperty::QREProperty() : REObject(),
    _comObj(NULL)
{

}

QREProperty::~QREProperty()
{
    if (_comObj)
    {
        _comObj->ReleaseQ();
    }
}


