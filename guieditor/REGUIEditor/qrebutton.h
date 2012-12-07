
#ifndef QREBUTTON_H
#define QREBUTTON_H

#include "qreview.h"
#include <regui.h>

class QREButtonCommon
{
protected:
    typedef struct _qreREButtonProps
    {
        QtProperty * item;
        QtProperty * pressedTexture;
        QtProperty * disabledTexture;

        QtVariantProperty * state;
        QtVariantProperty * isDisabled;
    } QREREButtonProps;
    QREREButtonProps _reButtonProps;
private:
    static void CreateREButtonProperties(QREREButtonProps * props, QtVariantPropertyManager * manager);
    void REButtonInfoToProperties();
protected:
public:
    virtual REButton * rebutton() = 0;
    virtual QREViewCommon * qreviewcommon() = 0;
    virtual const REButton::StateType GetStateQ() const = 0;
    virtual void SetStateQ(REButton::StateType st) = 0;
    virtual QRETextureObject * GetPressedTextureQ() = 0;
    virtual QRETextureObject * GetDisabledTextureQ() = 0;

    void REButtonSetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL);
    bool REButtonOnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue);
    bool REButtonOnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos);

    bool SetNewDefaultTextureQ();
    bool OpenDefaultTextureImageQ();
    void RemoveDefaultTextureQ();

    bool SetNewPressedTextureQ();
    bool OpenPressedTextureImageQ();
    void RemovePressedTextureQ();

    bool SetNewDisabledTextureQ();
    bool OpenDisabledTextureImageQ();
    void RemoveDisabledTextureQ();

    QREButtonCommon();
    virtual ~QREButtonCommon();
};




class QREButton :
        public REButton,
        public QREObjectCommon,
        public QREViewCommon,
        public QREButtonCommon
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


    /// QREButtonCommon
    virtual REButton * rebutton() { return this; }
    virtual QREViewCommon * qreviewcommon() { return this; }
    virtual const REButton::StateType GetStateQ() const { return _state; }
    virtual void SetStateQ(REButton::StateType st) { _state = st; }
    virtual QRETextureObject * GetPressedTextureQ();
    virtual QRETextureObject * GetDisabledTextureQ();

    /// REView
    virtual IREObjectProperty * GetPropertyForKey(const char * key) { return this->GetOrCreatePropertyForKey(key); }

    /// IRERenderable
    virtual void Render();
    virtual void RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);

    virtual QString GetXMLString(QString linePrefix, QString keyPurpose = QString(""));

    QREButton();
    virtual ~QREButton();
};

#endif // QREBUTTON_H
