
#ifndef QREFONTOBJECT_H
#define QREFONTOBJECT_H

#include "qreobject.h"
#include <regui.h>

class QREFontObjectCommon
{
protected:
    typedef struct _qreREFontObjectProps
    {
        QtProperty * item;
        QtVariantProperty * path;
        QtVariantProperty * height;
        QtVariantProperty * charScaleRatio;

    } QREREFontObjectProps;
    QREREFontObjectProps _reFontObjProps;
private:
    static void CreateREFontObjectProperties(QREREFontObjectProps * props,
                                             QtVariantPropertyManager * manager);
protected:
    QString _fontFilePathQ;
    REBuffer _fontFileBuffer;
    REFloat32 _fontHeightQ;

    virtual void SetPathQ(const REString & newPath) = 0;
    virtual void SetHeightQ(const REFloat32 newHeight) = 0;
    QString GetREFontObjectInfoXMLString(QString linePrefix);
public:
    void REFontObjectInfoToProperties();

    bool IsQREFontObjectNull() const { return ( _fontFilePathQ.isEmpty() && (_fontFileBuffer.GetSize() == 0) && (_fontHeightQ <= 0.0f) ); }

    virtual REFontObject * refontobject() = 0;

    void SetFontFilePathQ(const QString & path);
    void SetFontHeightQ(const REFloat32 height);
    const QString & GetFontFilePathQ() const { return _fontFilePathQ; }
    REFloat32 GetFontHeightQ() const { return _fontHeightQ; }
    void ReloadFont();

    virtual void SaveFontQ(SaveVCInfo * info, RELabel * label);

    void REFontObjectCommonSetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL);
    bool REFontObjectCommonOnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue);
    bool REFontObjectCommonOnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos);

    QREFontObjectCommon();
    virtual ~QREFontObjectCommon();
};





class QREFontObject :
        public REFontObject,
        public QREObjectCommon,
        public QREFontObjectCommon
{
protected:
    virtual void SetPathQ(const REString & newPath) { REFontObject::SetPath(newPath); }
    virtual void SetHeightQ(const REFloat32 newHeight) { REFontObject::SetHeight(newHeight); }
public:
    /// QREObjectCommon
    virtual REObject * reobject() { return this; }
    virtual const REUIdentifier GetClassIdentifierQ() const { return this->GetClassIdentifier(); }
    virtual const REUIdentifier GetObjectIdentifierQ() const { return this->GetObjectIdentifier(); }
    virtual void ReleaseQ() { this->Release(); }
    virtual void RetainQ() { this->Retain(); }

    virtual void SetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL);
    virtual bool OnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue);
    virtual bool OnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos);

    /// QREFontObjectCommon
    virtual REFontObject * refontobject() { return this; }

    /// REFontObject
    virtual REBOOL AcceptStringParameter(const char * key, const char * value);

    virtual QString GetXMLString(QString linePrefix, QString keyPurpose = QString(""));

    QREFontObject();
    virtual ~QREFontObject();
};

#endif // QREFONTOBJECT_H

