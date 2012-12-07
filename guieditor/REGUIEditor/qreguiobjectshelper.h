
#ifndef QREGUIOBJECTSHELPER_H
#define QREGUIOBJECTSHELPER_H

#include <QObject>

class QREObjectCommon;
class REObject;

typedef enum qreGUIObjectType
{
    QREGUIObjectType_NONE = 0,
    QREGUIObjectType_QREVIEW = 1,
    QREGUIObjectType_QREVIEWCONTROLLER = 2,
    QREGUIObjectType_QREBUTTON = 3,
    QREGUIObjectType_QREFONTOBJECT = 4,
    QREGUIObjectType_QRELABEL = 5,
    QREGUIObjectType_QREPARTICLEVIEW = 6,
    QREGUIObjectType_QRETEXTFIELD = 7,
    QREGUIObjectType_QRETEXTUREOBJECT = 8,
    QREGUIObjectType_QREOBJECT = 9,
    QREGUIObjectType_QREFRAMEDTEXTUREOBJECT = 10
} QREGUIObjectType;


class QREGUIObjectsHelper
{
public:
    static QREGUIObjectType GetType(QREObjectCommon * obj, void ** castedObject = 0);
    static const char * GetREClassNameStringForType(const QREGUIObjectType type);
    static QREObjectCommon * GetCommonObject(REObject * o);
};

#endif // QREGUIOBJECTSHELPER_H
