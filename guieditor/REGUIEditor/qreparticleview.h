
#ifndef QREPARTICLEVIEW_H
#define QREPARTICLEVIEW_H

#include "qreview.h"
#include <regui.h>

class QREParticleViewCommon
{
protected:
    typedef struct _qreREParticleViewProps
    {
        QtProperty * item;
        QtVariantProperty * path;
        QtVariantProperty * screenCenter;
        QtVariantProperty * center;
    } QREREParticleViewProps;
    QREREParticleViewProps _reParticleViewProps;
private:
    static void CreateREParticleViewProperties(QREREParticleViewProps * props,
                                               QtVariantPropertyManager * manager);
protected:
    QString _particleFilePathQ;
    REBuffer _particleFileBuffer;
    void REParticleViewInfoToProperties();
    QString GetREParticleViewSubViewsInfoXMLString(QString linePrefix);
    void SaveREParticleViewWithInfo(SaveVCInfo * info);
public:
    bool SetParticlePathQ(const QString & path);

    void REParticleViewSetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL);
    bool REParticleViewOnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue);
    bool REParticleViewOnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos);

    virtual REParticleView * reparticleview() = 0;

    bool OpenParticleFileQ();

    QREParticleViewCommon();
    virtual ~QREParticleViewCommon();
};





class QREParticleView :
        public REParticleView,
        public QREObjectCommon,
        public QREViewCommon,
        public QREParticleViewCommon
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

    /// QREParticleViewCommon
    virtual REParticleView * reparticleview() { return this; }

    /// REView
    virtual IREObjectProperty * GetPropertyForKey(const char * key) { return this->GetOrCreatePropertyForKey(key); }

    virtual void Render();
    virtual void RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY);

    virtual QString GetXMLString(QString linePrefix, QString keyPurpose = QString(""));

    QREParticleView();
    virtual ~QREParticleView();
};

#endif // QREPARTICLEVIEW_H
