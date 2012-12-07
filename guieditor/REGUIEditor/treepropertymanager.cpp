
#include "treepropertymanager.h"
#include "qreobject.h"

void TreePropertyManager::ValueOfPropertyChanged(QtProperty * property, const QVariant & value)
{
    if (_currentObject)
    {
        _currentObject->OnValueOfPropertyChanged(property, value);
    }
}


void TreePropertyManager::AssignCurrentObject(QREObjectCommon * newCurrentObject)
{
    _currentObject = newCurrentObject;
}

void TreePropertyManager::DisableOnChangeEvent()
{
    if (_variantManager)
    {
        disconnect(_variantManager, SIGNAL(valueChanged(QtProperty*,QVariant)), this, SLOT(ValueOfPropertyChanged(QtProperty*,QVariant)));
    }
}

void TreePropertyManager::EnableOnChangeEvent()
{
    if (_variantManager)
    {
        connect(_variantManager, SIGNAL(valueChanged(QtProperty*,QVariant)), this, SLOT(ValueOfPropertyChanged(QtProperty*,QVariant)));
    }
}

void TreePropertyManager::Clear()
{
    if (this->IsNotNULL())
    {
        _propertyBrowser->clear();

        _propertyBrowser->setFactoryForManager(_variantManager, _variantFactory);
        _propertyBrowser->setPropertiesWithoutValueMarked(true);
        _propertyBrowser->setRootIsDecorated(true);
        _propertyBrowser->setContextMenuPolicy(Qt::CustomContextMenu);
    }
}

TreePropertyManager::TreePropertyManager(QtTreePropertyBrowser * propertyBrowser) : QObject(),
    _propertyBrowser(propertyBrowser),
    _variantManager(NULL),
    _variantFactory(NULL),
    _currentObject(NULL)
{
    if (_propertyBrowser)
    {
        QtVariantPropertyManager * variantManager = new QtVariantPropertyManager();
        QtVariantEditorFactory * variantFactory = new QtVariantEditorFactory();
        if (variantManager && variantFactory)
        {
            _variantManager = variantManager;
            _variantFactory = variantFactory;
        }
        else
        {
            RE_SAFE_DELETE(variantManager);
            RE_SAFE_DELETE(variantFactory);
        }
    }

}

TreePropertyManager::~TreePropertyManager()
{
    this->DisableOnChangeEvent();

    RE_SAFE_DELETE(_variantManager);
    RE_SAFE_DELETE(_variantFactory);
}

