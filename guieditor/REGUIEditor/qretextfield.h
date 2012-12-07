
#ifndef QRETEXTFIELD_H
#define QRETEXTFIELD_H

#include "qrelabel.h"
#include <regui.h>

class QRETextFieldCommon
{
protected:
    typedef struct _qreRETextFieldProps
    {
        QtProperty * item;
        QtProperty * cursorView;

        QtVariantProperty * cursorBlinkTime;
        QtVariantProperty * maximumInputedTextLength;
        QtVariantProperty * isHasMaximumInputedTextLength;
        QtVariantProperty * isClearPreviousTextInputText;
        QtVariantProperty * isShowCursor;
    } QRERETextFieldProps;
    QRERETextFieldProps _reTextFieldProps;
private:
    static void CreateRETextFieldProperties(QRERETextFieldProps * props, QtVariantPropertyManager * manager);
    void RETextFieldInfoToProperties();
protected:
    QString GetRETextFieldInfoXMLString(QString linePrefix);
public:
    virtual RETextField * retextfield() = 0;

    void RETextFieldSetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL);
    bool RETextFieldOnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue);
    bool RETextFieldOnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos);

    QRETextFieldCommon();
    virtual ~QRETextFieldCommon();
};




class QRETextField :
        public RETextField,
        public QREObjectCommon,
        public QREViewCommon,
        public QRELabelCommon,
        public QRETextFieldCommon
{
protected:
    /// QREViewCommon
    virtual bool IsCanAddSubViewToXMLOutPut(REView * subView);
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

    /// QRETextFieldCommon
    virtual RETextField * retextfield() { return this; }

    /// REView
    virtual IREObjectProperty * GetPropertyForKey(const char * key) { return this->GetOrCreatePropertyForKey(key); }

    virtual void Render();
    virtual void RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);

    virtual QString GetXMLString(QString linePrefix, QString keyPurpose = QString(""));

    QRETextField();
    virtual ~QRETextField();
};

#endif // QRETEXTFIELD_H
