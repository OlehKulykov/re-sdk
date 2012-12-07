
#ifndef QREVIEW_H
#define QREVIEW_H

#include "qreobject.h"
#include "qretextureobject.h"

class QREView;

class QREViewCommon
{
protected:
    typedef struct _qreREViewProps
    {
        QtProperty * item;
        QtVariantProperty * tag;
        QtVariantProperty * screenFramePx;
        QtVariantProperty * framePx;
        QtVariantProperty * frameFl;
        QtVariantProperty * screenFrameFl;
        QtVariantProperty * color;
        QtVariantProperty * isVisible;
        QtVariantProperty * isRespondsForUserAction;
        QtVariantProperty * interceptsUserAction;

        QtProperty * textureItem;
    } QREREViewProps;
    QREREViewProps _reViewProps;
private:
    static void CreateREViewProperties(QREREViewProps * props,
                                       QtVariantPropertyManager * manager);
    void REViewInfoToProperties();
    QString SubViewsInfoXMLString(QString linePrefix, REObjectsArray * arr);
protected:
    QString GetREViewInfoXMLString(QString linePrefix);
    virtual bool IsCanAddSubViewToXMLOutPut(REView * subView);
    QString GetREViewSubViewsInfoXMLString(QString linePrefix);
    void RenderHighLight();
    void SaveREViewSubViewsWithInfo(SaveVCInfo * info);
public:
    virtual REView * review() = 0;
    QRETextureObject * GetTextureQ();
    void RemoveTextureQ();

    void REViewSetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL);
    bool REViewOnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue);
    bool REViewOnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos);

    bool SetNewTextureQ();
    bool OpenTextureImageQ();

    QREViewCommon();
    virtual ~QREViewCommon();

    static bool OpenTextureImageQ(QRETextureObject * qTexture);
};







class QREView :
        public REView,
        public QREObjectCommon,
        public QREViewCommon
{
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

    virtual void SaveQ(SaveVCInfo * info);

    /// QREViewCommon
    virtual REView * review() { return this; }

    /// IRERenderable
    virtual void Render();
    virtual void RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);

    /// REView
    virtual IREObjectProperty * GetPropertyForKey(const char * key) { return this->GetOrCreatePropertyForKey(key); }

    virtual QString GetXMLString(QString linePrefix, QString keyPurpose = QString(""));

    QREView();
    virtual ~QREView();

    static RERect ConvertRenderFrameToScreenFrameInPixels(const RERect & f);
    static RERect ConvertScreenFrameFInPixelsToRenderFrame(const QRectF & f);
    static RERect ConvertScreenFrameIInPixelsToRenderFrame(const QRect & f);
    static REPoint2 ConvertScreenPointIInPixelsToRenderPoint(const QPoint & p);
};

#endif // QREVIEW_H
