
#ifndef QREVIEWCONTROLLER_H
#define QREVIEWCONTROLLER_H

#include "qreobject.h"
#include "qreview.h"
#include <QList>

class CustomClassNameReplace
{
public:
    bool operator==(const CustomClassNameReplace & n) const
    {
        if (className.isEmpty() && n.className.isEmpty()) return false;
        return (className == n.className);
    }
    bool operator==(const QString & n) const
    {
        if (className.isEmpty() && n.isEmpty()) return false;
        return (className == n);
    }
    CustomClassNameReplace & operator=(const CustomClassNameReplace & n)
    {
        className = n.className;
        type = n.type;
        return (*this);
    }

    QString className;
    QREGUIObjectType type;
    CustomClassNameReplace(const CustomClassNameReplace & v) { className = v.className; type = v.type; }
    CustomClassNameReplace() : type(QREGUIObjectType_NONE) { }
};

class QREViewControllerCommon
{
protected:
    typedef struct _qREViewControllerProps
    {
        QtProperty * item;
        QtVariantProperty * name;
    } QREViewControllerProps;
    QREViewControllerProps _qreViewControllerProps;

private:
    static void CreateREViewControllerProperties(QREViewControllerProps * props,
                                                 QtVariantPropertyManager * manager);
    void REViewControllerInfoToProperties();
protected:
    QSizeF _sizeFlQ;
    QSize _sizePxQ;
    QString _nameQ;
    QList<CustomClassNameReplace> _replaceClasses;

    QString GetREViewControllerInfoXMLString(QString linePrefix);
public:
    virtual REViewController * reviewcontroller() = 0;

    const QSizeF & GetSizeFlQ() const { return _sizeFlQ; }
    const QSize & GetSizePxQ() const { return _sizePxQ; }
    const QString & GetNameQ() const { return _nameQ; }
    void SetNameQ(const QString & n) { _nameQ = n; }

    void SetSizeFlQ(const QSizeF & s) { _sizeFlQ = s; }
    void SetSizePxQ(const QSize & s) { _sizePxQ = s; }

    void REViewControllerSetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL);
    bool REViewControllerOnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue);
    bool REViewControllerOnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos);

    QREViewControllerCommon();
    virtual ~QREViewControllerCommon();
};




class QREViewController :
        public REViewController,
        public QREObjectCommon,
        public QREViewCommon,
        public QREViewControllerCommon
{
private:
    static REGUIObject * NewXMLSerializableObjectQ(const char * className, const char * key);
protected:
    /// REViewController
    /// Creates and return object that inherits 'IREXMLSerializable' class.
    /// This new class object is equal to 'className' for 'key' needs.
    virtual REGUIObject * CreateXMLSerializableObject(const char * className, const char * key) const;
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

    virtual QString GetXMLString(QString linePrefix, QString keyPurpose = QString(""));

    virtual void SaveQ(SaveVCInfo * info);

    /// QREViewCommon
    virtual REView * review() { return this; }

    /// QREViewControllerCommon
    virtual REViewController * reviewcontroller() { return this; }

    /// REViewController
    virtual REBOOL AcceptStringParameter(const char * key, const char * value);
    virtual REBOOL LoadByName(const REString & name, REBOOL isLoadInBackground);

    /// REView
    virtual IREObjectProperty * GetPropertyForKey(const char * key) { return this->GetOrCreatePropertyForKey(key); }

    virtual void Render(){REViewController::Render();if(this->IsHighLighted())this->RenderHighLight();}
    virtual void RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY) {REViewController::RenderWithOffset(offsetX,offsetY);if(this->IsHighLighted())this->RenderHighLight();}


    QREViewController();
    virtual ~QREViewController();
};

#endif // QREVIEWCONTROLLER_H
