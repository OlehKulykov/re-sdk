
#include "qreguiobjectshelper.h"

#include "qreobject.h"
#include "qreview.h"
#include "qreviewcontroller.h"
#include "qrebutton.h"
#include "qrefontobject.h"
#include "qrelabel.h"
#include "qreparticleview.h"
#include "qretextfield.h"
#include "qretextureobject.h"

static const char* __REClassNameStrings[QREGUIObjectType_QREFRAMEDTEXTUREOBJECT + 1] =
{
    "none",
    "REView",
    "REViewController",
    "REButton",
    "REFontObject",
    "RELabel",
    "REParticleView",
    "RETextField",
    "RETextureObject",
    "REObject",
    "REFramedTextureObject"
};

const char * QREGUIObjectsHelper::GetREClassNameStringForType(const QREGUIObjectType type)
{
    return __REClassNameStrings[(int)type];
}


QREGUIObjectType QREGUIObjectsHelper::GetType(QREObjectCommon * obj, void ** castedObject)
{
    if (castedObject)
    {
        *castedObject = NULL;
    }

    if (obj)
    {
        const REUInt32 clID = obj->GetClassIdentifierQ();
        if (clID == REObject::ClassIdentifier())
        {
            if (castedObject)
            {
                QREObject * castO = dynamic_cast<QREObject*>(obj);
                *castedObject = castO;
            }
            return QREGUIObjectType_QREOBJECT;
        }
        else if (clID == REView::ClassIdentifier())
        {
            if (castedObject)
            {
                QREView * castO = dynamic_cast<QREView*>(obj);
                *castedObject = castO;
            }
            return QREGUIObjectType_QREVIEW;
        }
        else if (clID == REViewController::ClassIdentifier())
        {
            if (castedObject)
            {
                QREViewController * castO = dynamic_cast<QREViewController*>(obj);
                *castedObject = castO;
            }
            return QREGUIObjectType_QREVIEWCONTROLLER;
        }
        else if (clID == RELabel::ClassIdentifier())
        {
            if (castedObject)
            {
                QRELabel * castO = dynamic_cast<QRELabel*>(obj);
                *castedObject = castO;
            }
            return QREGUIObjectType_QRELABEL;
        }
        else if (clID == RETextureObject::ClassIdentifier())
        {
            if (castedObject)
            {
                QRETextureObject * castO = dynamic_cast<QRETextureObject*>(obj);
                *castedObject = castO;
            }
            return QREGUIObjectType_QRETEXTUREOBJECT;
        }
        else if (clID == REFontObject::ClassIdentifier())
        {
            if (castedObject)
            {
                QREFontObject * castO = dynamic_cast<QREFontObject*>(obj);
                *castedObject = castO;
            }
            return QREGUIObjectType_QREFONTOBJECT;
        }
        else if (clID == REButton::ClassIdentifier())
        {
            if (castedObject)
            {
                QREButton * castO = dynamic_cast<QREButton*>(obj);
                *castedObject = castO;
            }
            return QREGUIObjectType_QREBUTTON;
        }
        else if (clID == RETextField::ClassIdentifier())
        {
            if (castedObject)
            {
                QRETextField * castO = dynamic_cast<QRETextField*>(obj);
                *castedObject = castO;
            }
            return QREGUIObjectType_QRETEXTFIELD;
        }
        else if (clID == REParticleView::ClassIdentifier())
        {
            if (castedObject)
            {
                QREParticleView * castO = dynamic_cast<QREParticleView*>(obj);
                *castedObject = castO;
            }
            return QREGUIObjectType_QREPARTICLEVIEW;
        }
    }
    return QREGUIObjectType_NONE;
}

QREObjectCommon * QREGUIObjectsHelper::GetCommonObject(REObject * o)
{
    if (o)
    {
        return dynamic_cast<QREObjectCommon *>(o);
    }
    return NULL;
}


