
#ifndef QRELABEL_H
#define QRELABEL_H

#include "qreview.h"
#include "qrefontobject.h"

class QRELabel;

class QRELabelCommon
{
protected:
    typedef struct _qreRELabelProps
    {
        QtProperty * item;
        QtVariantProperty * text;
        QtVariantProperty * textColor;
        QtProperty * textInsets;
        QtVariantProperty * textInsetsTop;
        QtVariantProperty * textInsetsLeft;
        QtVariantProperty * textInsetsBottom;
        QtVariantProperty * textInsetsRight;


        QtVariantProperty * charsSpaceRatio;
        QtVariantProperty * shadowOffset;
        QtVariantProperty * shadowColor;
        QtVariantProperty * isUsingShadow;
        QtVariantProperty * isShowBackground;
        QtVariantProperty * textAlignment;
        QtVariantProperty * lineBreakMode;
        QtVariantProperty * textLineTruncationString;

        QtProperty * font;
    } QRERELabelProps;
    QRERELabelProps _reLabelProps;
private:
    static void CreateRELabelProperties(QRERELabelProps * props, QtVariantPropertyManager * manager);
    void RELabelInfoToProperties();
protected:
    QString GetRELabelInfoXMLString(QString linePrefix);
public:
    virtual RELabel * relabel() = 0;
    QREFontObject * GetFontQ();

    void RELabelSetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL);
    bool RELabelOnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue);
    bool RELabelOnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos);

    bool SetNewFontQ();
    bool OpenFontFileQ();
    void RemoveFontQ();

    QRELabelCommon();
    virtual ~QRELabelCommon();

    static bool OpenFontFileQ(QREFontObject * font);
};





class QRELabel :
        public RELabel,
        public QREObjectCommon,
        public QREViewCommon,
        public QRELabelCommon
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

    /// QRELabelCommon
    virtual RELabel * relabel() { return this; }

    /// REView
    virtual IREObjectProperty * GetPropertyForKey(const char * key) { return this->GetOrCreatePropertyForKey(key); }

    /// IRERenderable
    virtual void Render();
    virtual void RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);

    virtual QString GetXMLString(QString linePrefix, QString keyPurpose = QString(""));

    QRELabel();
    virtual ~QRELabel();
};

#endif // QRELABEL_H
