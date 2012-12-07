
#ifndef QRETEXTUREOBJECT_H
#define QRETEXTUREOBJECT_H

#include "qreobject.h"

class QRETextureObjectCommon
{
protected:
    typedef struct _qreRETextureObjectProps
    {
        QtProperty * item;
        QtVariantProperty * path;
        QtVariantProperty * filter;
        QtVariantProperty * frametetr;

        QtProperty * frame;
        QtVariantProperty * frameTopLeft;
        QtVariantProperty * frameTopRight;
        QtVariantProperty * frameBottomLeft;
        QtVariantProperty * frameBottomRight;
    } QRERETextureObjectProps;
    QRERETextureObjectProps _reTextureObjectProps;
private:
    static void CreateRETextureObjectProperties(QRERETextureObjectProps * props,
                                                QtVariantPropertyManager * manager);
    void RETextureObjectInfoToProperties();
protected:
    QString _imageFilePath;
    REBuffer _imageFileBuffer;
    RETextureFilterType _filterTypeQ;
    QString GetRETextureObjectInfoXMLString(QString linePrefix);
public:
    void SetImageFilePathQ(const QString & path);
    const QString & GetImageFilePathQ() const { return _imageFilePath; }
    void SetFilterTypeQ(const RETextureFilterType filter);
    void ReloadTextureUsingFileBuffer();

    bool IsQRETextureObjectNull() const { return ( _imageFilePath.isEmpty() && (_imageFileBuffer.GetSize() == 0) ); }

    virtual RETextureObject * retextureobject() = 0;

    virtual REBOOL InitBlankTextureQ(const REImagePixelFormat pixelsFormat,
                                     const REUInt32 width,
                                     const REUInt32 height,
                                     const RETextureFilterType filterType) = 0;

    void RETextureObjectSetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL);
    bool RETextureObjectOnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue);
    bool RETextureObjectOnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos);

    virtual void SaveTextureQ(SaveVCInfo * info, REView * view);

    QRETextureObjectCommon();
    virtual ~QRETextureObjectCommon();
};



class QRETextureObject :
        public RETextureObject,
        public QREObjectCommon,
        public QRETextureObjectCommon
{
protected:
   virtual REBOOL UpdateFromImageFilePath(const REString & imageFilePath, const RETextureFilterType filterType);
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

    /// QRETextureObjectCommon
    virtual RETextureObject * retextureobject() { return this; }
    virtual REBOOL InitBlankTextureQ(const REImagePixelFormat pixelsFormat,
                                     const REUInt32 width,
                                     const REUInt32 height,
                                     const RETextureFilterType filterType) { return this->InitBlankTexture(pixelsFormat, width, height, filterType); }
    /// RETextureObject
    virtual void SetFilterType(const RETextureFilterType filter);

    virtual QString GetXMLString(QString linePrefix, QString keyPurpose = QString(""));

    QRETextureObject();
    virtual ~QRETextureObject();
};

#endif // QRETEXTUREOBJECT_H
