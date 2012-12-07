
#include "qrefontobject.h"
#include <QFont>


void QREFontObjectCommon::CreateREFontObjectProperties(QREREFontObjectProps * props,
                                                       QtVariantPropertyManager * manager)
{
    props->item = manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("REFontObject"));

    props->path = manager->addProperty(QVariant::String, QLatin1String("Font File Path"));
    props->item->addSubProperty(props->path);

    props->height = manager->addProperty(QVariant::Double, QLatin1String("Height"));
    props->height->setAttribute(QLatin1String("decimals"), 6);
    props->item->addSubProperty(props->height);

    props->charScaleRatio = manager->addProperty(QVariant::SizeF, QLatin1String("Char Scale Ratio"));
    props->charScaleRatio->setAttribute(QLatin1String("decimals"), 6);
    props->charScaleRatio->setEnabled(false);
    props->item->addSubProperty(props->charScaleRatio);
}

void QREFontObjectCommon::REFontObjectInfoToProperties()
{
    if (_reFontObjProps.path)
    {
        _reFontObjProps.path->setValue(this->GetFontFilePathQ());
    }
    if (_reFontObjProps.height)
    {
        _reFontObjProps.height->setValue(QVariant((double)this->GetFontHeightQ()));
    }
    if (_reFontObjProps.charScaleRatio)
    {
        RESize reSize(this->refontobject()->GetCharsScaleRatio());
        QSizeF s;
        s.setWidth(reSize.width);
        s.setHeight(reSize.height);
        _reFontObjProps.charScaleRatio->setValue(QVariant(s));
    }
}

QString QREFontObjectCommon::GetREFontObjectInfoXMLString(QString linePrefix)
{
    QString xml;

    xml += linePrefix + "<string key=\"" + REFontObject::GetXMLHeightKeyString() + "\">" +
            QString().sprintf(REFontObject::GetXMLHeightFormatString(), (float)this->GetFontHeightQ()) + "</string>\n";

    xml += linePrefix + "<string key=\"" + REFontObject::GetXMLPathKeyString() + "\">" + this->GetFontFilePathQ() + "</string>\n";

    return xml;
}

void QREFontObjectCommon::REFontObjectCommonSetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL)
{
    if (TreePropertyManager::IsNotNULL(propertyManager))
    {
        if (_reFontObjProps.item == NULL)
        {
            QREFontObject::CreateREFontObjectProperties(&_reFontObjProps, propertyManager->GetVariantManager());
        }

        this->REFontObjectInfoToProperties();

        if (_reFontObjProps.item)
        {
            if (parentItemOrNULL) { parentItemOrNULL->addSubProperty(_reFontObjProps.item); }
            else { propertyManager->GetPropertyBrowser()->addProperty(_reFontObjProps.item); }
        }
    }
}

bool QREFontObjectCommon::REFontObjectCommonOnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue)
{
    if (property && newValue.isValid())
    {
        if (property == _reFontObjProps.path)
        {
            QString p(newValue.toString());
            this->SetFontFilePathQ(p);
            return true;
        }
        else if (property == _reFontObjProps.height)
        {
            bool isOk = false;
            const double v = newValue.toDouble(&isOk);
            if (isOk) { this->SetFontHeightQ((REFloat32)v); }
            return true;
        }
    }
    return false;
}

bool QREFontObjectCommon::REFontObjectCommonOnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos)
{
    return false;
}

void QREFontObjectCommon::SetFontFilePathQ(const QString & path)
{
    _fontFilePathQ.clear();
    _fontFileBuffer.Clear();

    REBuffer buff;
    if (REQTConverter::QFileReadToBuffer(path, &buff))
    {
        _fontFileBuffer = buff;
        _fontFilePathQ = path;
    }
    else
    {
        REData data;
        REString rePath(REQTConverter::GetREString(path));
        if (data.InitFromPath(rePath))
        {
            _fontFileBuffer.Set(data.GetBytes(), data.GetSize());
            _fontFilePathQ = path;
        }
    }
    this->ReloadFont();
}

void QREFontObjectCommon::SetFontHeightQ(const REFloat32 height)
{
    _fontHeightQ = height;
    this->ReloadFont();
}

void QREFontObjectCommon::ReloadFont()
{
    if (_fontHeightQ > 0.0f)
    {
        this->SetPathQ(REQTConverter::GetREString(_fontFilePathQ));
        this->SetHeightQ(_fontHeightQ);
    }
}

void QREFontObjectCommon::SaveFontQ(SaveVCInfo * info, RELabel * label)
{
    if (info && (_fontFileBuffer.GetSize() > 0) && (_fontFilePathQ.length() > 0))
    {
        QStringPath p(info->GetFontQStoreFullPath());
        QStringPath fileName = QStringPath(_fontFilePathQ).lastPathComponent();
        fileName = fileName.toLower();
        p.appendPath(fileName);
        if (REQTConverter::REBufferWriteToPath(&_fontFileBuffer, p))
        {
            p = info->GetFontQStoreDataPath();
            p.appendPath(fileName);
            _fontFilePathQ = p;
        }
    }
}

QREFontObjectCommon::QREFontObjectCommon()
{
    memset(&_reFontObjProps, 0, sizeof(QREREFontObjectProps));
    _fontHeightQ = 0.0f;
}

QREFontObjectCommon::~QREFontObjectCommon()
{
    QREObjectCommon::RemoveAllSubProperties(_reFontObjProps.item);
}






void QREFontObject::SetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL)
{
    QREObjectCommon::SetProperties(propertyManager, parentItemOrNULL);
    this->REFontObjectCommonSetProperties(propertyManager, parentItemOrNULL);
}

bool QREFontObject::OnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue)
{
    if (QREObjectCommon::OnValueOfPropertyChanged(property, newValue))
    {
        return true;
    }

    return this->REFontObjectCommonOnValueOfPropertyChanged(property, newValue);
}

bool QREFontObject::OnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos)
{
    if (QREObjectCommon::OnPropertyCustomContextMenuRequested(property, pos))
    {
        return true;
    }

    return this->REFontObjectCommonOnPropertyCustomContextMenuRequested(property, pos);
}

QString QREFontObject::GetXMLString(QString linePrefix, QString keyPurpose)
{
    QString xml;

    xml += linePrefix + "<object class=\"" + this->GetClassNameQ() + "\"";
    if (keyPurpose.isEmpty()) xml += ">\n";
    else xml += " key=\"" + keyPurpose + "\">\n";

    xml += this->GetREObjectInfoXMLString((linePrefix + "\t"));
    xml += this->GetREFontObjectInfoXMLString((linePrefix + "\t"));

    xml += linePrefix + "</object>\n";

    return xml;
}

REBOOL QREFontObject::AcceptStringParameter(const char * key, const char * value)
{
    if (REFontObject::AcceptStringParameter(key, value))
    {
        if (strcmp(key, REFontObject::GetXMLHeightKeyString()) == 0)
        {
            float h = 0.0f;
            if (sscanf(value, REFontObject::GetXMLHeightFormatString(), &h) == 1)
            {
                _fontHeightQ = h;
                return true;
            }
        }
        else if (strcmp(key, REFontObject::GetXMLPathKeyString()) == 0)
        {
            _fontFilePathQ.clear();
            _fontFileBuffer.Clear();

            QString path(value);

            REBuffer buff;
            if (REQTConverter::QFileReadToBuffer(path, &buff))
            {
                _fontFileBuffer = buff;
                _fontFilePathQ = path;
            }
            else
            {
                REData data;
                REString rePath(REQTConverter::GetREString(path));
                if (data.InitFromPath(rePath))
                {
                    _fontFileBuffer.Set(data.GetBytes(), data.GetSize());
                    _fontFilePathQ = path;
                }
            }
            return true;
        }
        return true;
    }
    return false;
}

QREFontObject::QREFontObject() :
    REFontObject(),
    QREObjectCommon(),
    QREFontObjectCommon()
{

}

QREFontObject::~QREFontObject()
{

}

