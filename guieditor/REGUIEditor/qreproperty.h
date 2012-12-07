
#ifndef QREPROPERTY_H
#define QREPROPERTY_H

#include <regui.h>
#include <QString>

class QREObjectCommon;

class QREProperty : public REObject, public IREObjectProperty
{
protected:
    QREObjectCommon * _comObj;

    QString _keyStringQ;

public:
    /// Settes property value from pointer of another value.
    //virtual void SetFromValuePointer(const void* v);

    /// Getter for pointer of propert value.
    //virtual void * GetValuePointer();

    /// Settes property value from object.
    virtual void SetObject(REObject * v);

    /// Getter for pointer of object.
    virtual REObject * GetObject() const;

    QREObjectCommon * GetQREObjectCommon() const;

    REUInt32 GetObjectEditorIDQ() const;

    const QString & GetKeyStringQ() const { return _keyStringQ; }
    void SetKeyStringQ(const char * v) { _keyStringQ = v; }
    void SetKeyStringQ(const QString & v) { _keyStringQ = v; }

    QREProperty();
    virtual ~QREProperty();
};

#endif // QREPROPERTY_H
