
#ifndef TREEPROPERTYMANAGER_H
#define TREEPROPERTYMANAGER_H

#include <QObject>
#include <recore.h>
#include <regui.h>
#include "qstringpath.h"
#include "QtTreePropertyBrowser"
#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"

class QREObjectCommon;

class TreePropertyManager : public QObject
{
   Q_OBJECT
protected:
    QtTreePropertyBrowser * _propertyBrowser;
    QtVariantPropertyManager *_variantManager;
    QtVariantEditorFactory * _variantFactory;
    QREObjectCommon * _currentObject;

public:
    QtTreePropertyBrowser * GetPropertyBrowser() const { return _propertyBrowser; }
    QtVariantPropertyManager * GetVariantManager() const { return _variantManager; }
    QtVariantEditorFactory * GetVariantFactory() const { return _variantFactory; }
    void AssignCurrentObject(QREObjectCommon * newCurrentObject);

    bool IsNotNULL() const { if (_propertyBrowser && _variantManager && _variantFactory) { return true; } return false; }
    bool IsNULL() const { return !this->IsNotNULL(); }

    void DisableOnChangeEvent();
    void EnableOnChangeEvent();
    void Clear();

    TreePropertyManager(QtTreePropertyBrowser * propertyBrowser);
    virtual ~TreePropertyManager();

    static bool IsNotNULL(TreePropertyManager * manager) { if (manager) { return manager->IsNotNULL(); } return false; }
    static bool IsNULL(TreePropertyManager * manager) { return !TreePropertyManager::IsNotNULL(manager); }

private slots:
    void ValueOfPropertyChanged(QtProperty * property, const QVariant & value);
};

#endif // TREEPROPERTYMANAGER_H
