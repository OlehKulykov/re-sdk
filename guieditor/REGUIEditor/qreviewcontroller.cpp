
#include "qreviewcontroller.h"
#include "viewcontrollercreatecustomxmlserializableobject.h"
#include "mainwindow.h"
#include "qrebutton.h"
#include "qrelabel.h"
#include "qreparticleview.h"
#include "qrefontobject.h"
#include "qretextfield.h"
#include "qretextureobject.h"


void QREViewControllerCommon::CreateREViewControllerProperties(QREViewControllerProps * props,
                                                               QtVariantPropertyManager * manager)
{
    props->item = manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("REViewController"));


    props->name = manager->addProperty(QVariant::String, QLatin1String("Name"));
    //props->name->setEnabled(false);
    props->item->addSubProperty(props->name);
}

void QREViewControllerCommon::REViewControllerInfoToProperties()
{
    if (_qreViewControllerProps.name)
    {
        _qreViewControllerProps.name->setValue(_nameQ);
    }
}

QString QREViewControllerCommon::GetREViewControllerInfoXMLString(QString linePrefix)
{
    QString xml;

    if (!this->GetNameQ().isEmpty())
    {
        xml += linePrefix + "<string key=\"name\">" + this->GetNameQ() + "</string>\n";
    }
    xml += linePrefix + "<string key=\"sizepx\">" + QString::number(this->GetSizePxQ().width()) + ";" +  QString::number(this->GetSizePxQ().height()) + "</string>\n";
    xml += linePrefix + "<string key=\""+ REViewController::GetXMLSizeKeyString() +"\">" +
            QString().sprintf(REViewController::GetXMLSizeFormatString(), (float)this->GetSizeFlQ().width(), (float)this->GetSizeFlQ().height()) + "</string>\n";

    return xml;
}

void QREViewControllerCommon::REViewControllerSetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL)
{
    if (TreePropertyManager::IsNotNULL(propertyManager))
    {
        if (_qreViewControllerProps.item == NULL)
        {
            QREViewControllerCommon::CreateREViewControllerProperties(&_qreViewControllerProps, propertyManager->GetVariantManager());
        }

        this->REViewControllerInfoToProperties();

        if (_qreViewControllerProps.item)
        {
            if (parentItemOrNULL) { parentItemOrNULL->addSubProperty(_qreViewControllerProps.item); }
            else { propertyManager->GetPropertyBrowser()->addProperty(_qreViewControllerProps.item); }
        }
    }
}

bool QREViewControllerCommon::REViewControllerOnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue)
{
    if (property && newValue.isValid())
    {
        if (_qreViewControllerProps.name == property)
        {
            _nameQ = newValue.toString();
            return true;
        }
    }
    return false;
}

bool QREViewControllerCommon::REViewControllerOnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos)
{
    return false;
}

QREViewControllerCommon::QREViewControllerCommon()
{
    memset(&_qreViewControllerProps, 0, sizeof(QREViewControllerProps));
}

QREViewControllerCommon::~QREViewControllerCommon()
{
    QREObjectCommon::RemoveAllSubProperties(_qreViewControllerProps.item);
}





void QREViewController::SetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL)
{
    QREObjectCommon::SetProperties(propertyManager, parentItemOrNULL);

    this->REViewSetProperties(propertyManager, parentItemOrNULL);

    if (_reViewProps.color)
    {
        _reViewProps.color->setEnabled(false);
    }

    this->REViewControllerSetProperties(propertyManager, parentItemOrNULL);
}

bool QREViewController::OnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue)
{
    if (QREObjectCommon::OnValueOfPropertyChanged(property, newValue))
    {
        return true;
    }

    if (this->REViewOnValueOfPropertyChanged(property, newValue))
    {
        return true;
    }

    return this->REViewControllerOnValueOfPropertyChanged(property, newValue);
}

bool QREViewController::OnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos)
{
    if (QREObjectCommon::OnPropertyCustomContextMenuRequested(property, pos))
    {
        return true;
    }
    if (this->REViewOnPropertyCustomContextMenuRequested(property, pos))
    {
        return true;
    }
    return this->REViewControllerOnPropertyCustomContextMenuRequested(property, pos);
}

REGUIObject * QREViewController::NewXMLSerializableObjectQ(const char * className, const char * key)
{
    if (strcmp(className, "REView") == 0)
    {
        REGUIObject * newObject = new QREView();
        return newObject;
    }
    else if (strcmp(className, "REButton") == 0)
    {
        REGUIObject * newObject = new QREButton();
        return newObject;
    }
    else if (strcmp(className, "RELabel") == 0)
    {
        REGUIObject * newObject = new QRELabel();
        return newObject;
    }
    else if (strcmp(className, "REFontObject") == 0)
    {
        REGUIObject * newObject = new QREFontObject();
        return newObject;
    }
    else if (strcmp(className, "RETextField") == 0)
    {
        REGUIObject * newObject = new QRETextField();
        return newObject;
    }
    else if (strcmp(className, "REParticleView") == 0)
    {
        REGUIObject * newObject = new QREParticleView();
        return newObject;
    }
    else if (strcmp(className, "RETextureObject") == 0)
    {
        REGUIObject * newObject = new QRETextureObject();
        return newObject;
    }

    return NULL;
}

REGUIObject * QREViewController::CreateXMLSerializableObject(const char * className, const char * key) const
{
    REGUIObject * newObj = QREViewController::NewXMLSerializableObjectQ(className, key);
    if (newObj)
    {
        return newObj;
    }

    if (className)
    {
        CustomClassNameReplace rep;
        rep.className = className;
        const int index = _replaceClasses.indexOf(rep);
        if (index >= 0)
        {
            rep = _replaceClasses.at(index);
        }
        else
        {
            ViewControllerCreateCustomXMLSerializableObject dialog(MainWindow::GetMainWindow());
            dialog.SetCustomObjectClassName(className);
            dialog.setModal(true);
            dialog.exec();
            rep.type = dialog.selectedType;
            QList<CustomClassNameReplace> * list = const_cast<QList<CustomClassNameReplace> *>(&_replaceClasses);
            list->append(rep);
        }

        switch (rep.type)
        {
        case QREGUIObjectType_QREVIEWCONTROLLER:
        {
            QREViewController * o = new QREViewController();
            if (o) { newObj = o; o->SetClassNameQ(className); };
        }
            break;

        case QREGUIObjectType_QREVIEW:
        {
            QREView * o = new QREView();
            if (o) { newObj = o; o->SetClassNameQ(className); };
        }
            break;

        case QREGUIObjectType_QREBUTTON:
        {
            QREButton * o = new QREButton();
            if (o) { newObj = o; o->SetClassNameQ(className); };
        }
            break;

        case QREGUIObjectType_QRELABEL:
        {
            QRELabel * o = new QRELabel();
            if (o) { newObj = o; o->SetClassNameQ(className); };
        }
            break;

        case QREGUIObjectType_QRETEXTFIELD:
        {
            QRETextField * o = new QRETextField();
            if (o) { newObj = o; o->SetClassNameQ(className); };
        }
            break;

        case QREGUIObjectType_QREPARTICLEVIEW:
        {
            QREParticleView * o = new QREParticleView();
            if (o) { newObj = o; o->SetClassNameQ(className); };
        }
            break;

        case QREGUIObjectType_QRETEXTUREOBJECT:
        {
            QRETextureObject * o = new QRETextureObject();
            if (o) { newObj = o; o->SetClassNameQ(className); };
        }
            break;

        case QREGUIObjectType_QREFONTOBJECT:
        {
            QREFontObject * o = new QREFontObject();
            if (o) { newObj = o; o->SetClassNameQ(className); };
        }
            break;

        default: break;
        };

    }
    return newObj;
}

REBOOL QREViewController::AcceptStringParameter(const char * key, const char * value)
{
    const REBOOL r = REViewController::AcceptStringParameter(key, value);
    if (strcmp(key, "sizef") == 0)
    {
        if (value)
        {
            RESize s;
            if (sscanf(value, "%f;%f", &s.width, &s.height) == 2)
            {
                _sizeFlQ.setWidth((qreal)s.width);
                _sizeFlQ.setHeight((qreal)s.height);
                RERenderDevice * device = RERenderDevice::GetDefaultDevice();
                if (device)
                {
                    device->SetRenderSize(s);
                }
                return true;
            }
        }
    }
    else if (strcmp(key, "sizepx") == 0)
    {
        if (value)
        {
            int w = 0, h = 0;
            if (sscanf(value, "%i;%i", &w, &h) == 2)
            {
                _sizePxQ.setWidth(w);
                _sizePxQ.setHeight(h);
                RERenderDevice * device = RERenderDevice::GetDefaultDevice();
                if (device)
                {
                    device->SetScreenSize((REFloat32)w, (REFloat32)h);
                    MainWindow::OnRenderDeviceScreenSizeChanged();
                }
                return true;
            }
        }
    }
    else if (strcmp(key, "name") == 0)
    {
        _nameQ = value;
        return true;
    }
    return r;
}

REBOOL QREViewController::LoadByName(const REString & name, REBOOL isLoadInBackground)
{
    _nameQ.clear();
    _replaceClasses.clear();

    const REBOOL r = REViewController::LoadByName(name, isLoadInBackground);
    if (r)
    {
        _nameQ = name.UTF8String();
    }
    return r;
}

QString QREViewController::GetXMLString(QString linePrefix, QString keyPurpose)
{
    QString xml = linePrefix + "<viewcontroller>\n";

    xml += this->GetREObjectInfoXMLString((linePrefix + "\t"));
    xml += this->GetREViewControllerInfoXMLString((linePrefix + "\t"));

    xml += this->GetREViewSubViewsInfoXMLString((linePrefix + "\t"));

    xml += linePrefix + "</viewcontroller>";

    //TODO: normal float to string optimization
    xml.replace(QString(".0000000<"), QString("<"));
    xml.replace(QString(".000000<"), QString("<"));
    xml.replace(QString(".00000<"), QString("<"));
    xml.replace(QString(".0000<"), QString("<"));
    xml.replace(QString(".000<"), QString("<"));
    xml.replace(QString(".00<"), QString("<"));
    xml.replace(QString(".0<"), QString("<"));
    xml.replace(QString(".0000000;"), QString(";"));
    xml.replace(QString(".000000;"), QString(";"));
    xml.replace(QString(".00000;"), QString(";"));
    xml.replace(QString(".0000;"), QString(";"));
    xml.replace(QString(".000;"), QString(";"));
    xml.replace(QString(".00;"), QString(";"));
    xml.replace(QString(".0;"), QString(";"));
    return xml;
}

void QREViewController::SaveQ(SaveVCInfo * info)
{
    if (info)
    {
        this->SaveREViewSubViewsWithInfo(info);

        QString xml = this->GetXMLString(QString(""));
        if (xml.length() > 0)
        {
            QStringPath p(info->GetViewControllerXMLStoreFullPath());
            REString reStr(REQTConverter::GetREString(xml));
            REBuffer buff;
            buff.Set(reStr.UTF8String(), reStr.Length());
            REQTConverter::REBufferWriteToPath(&buff, p);
        }
    }
}

QREViewController::QREViewController() :
    REViewController(),
    QREObjectCommon(),
    QREViewCommon(),
    QREViewControllerCommon()
{

}

QREViewController::~QREViewController()
{

}


