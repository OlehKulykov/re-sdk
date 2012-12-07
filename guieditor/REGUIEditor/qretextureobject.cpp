
#include "qretextureobject.h"
#include <QFile>
#include "glrenderer.h"

void QRETextureObjectCommon::CreateRETextureObjectProperties(QRERETextureObjectProps * props,
                                                             QtVariantPropertyManager * manager)
{
    props->item = manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("RETextureObject"));
    if (props->item)
    {
        props->path = manager->addProperty(QVariant::String, QLatin1String("Image File Path"));
        if (props->path) props->item->addSubProperty(props->path);


        QStringList enumNames;
        enumNames << "Undefined" << "Nearest" << "Linear" << "Mipmaped Standart" << "Mipmaped Trilinear";
        props->filter = manager->addProperty(QtVariantPropertyManager::enumTypeId(), QLatin1String("Filter"));
        if (props->filter)
        {
            props->filter->setAttribute(QLatin1String("enumNames"), enumNames);
            props->item->addSubProperty(props->filter);
        }

        props->frame = manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("Frame"));
        if (props->frame)
        {
            props->frameTopLeft = manager->addProperty(QVariant::PointF, QLatin1String("Top Left Coord"));
            if (props->frameTopLeft)
            {
                props->frameTopLeft->setAttribute(QLatin1String("decimals"), 6);
                props->frame->addSubProperty(props->frameTopLeft);
            }

            props->frameTopRight = manager->addProperty(QVariant::PointF, QLatin1String("Top Right Coord"));
            if (props->frameTopRight)
            {
                props->frameTopRight->setAttribute(QLatin1String("decimals"), 6);
                props->frame->addSubProperty(props->frameTopRight);
            }

            props->frameBottomLeft = manager->addProperty(QVariant::PointF, QLatin1String("Bottom Left Coord"));
            if (props->frameBottomLeft)
            {
                props->frameBottomLeft->setAttribute(QLatin1String("decimals"), 6);
                props->frame->addSubProperty(props->frameBottomLeft);
            }

            props->frameBottomRight = manager->addProperty(QVariant::PointF, QLatin1String("Bottom Right Coord"));
            if (props->frameBottomRight)
            {
                props->frameBottomRight->setAttribute(QLatin1String("decimals"), 6);
                props->frame->addSubProperty(props->frameBottomRight);
            }
        }
    }
}

void QRETextureObjectCommon::RETextureObjectInfoToProperties()
{
    if (_reTextureObjectProps.path)
    {
        _reTextureObjectProps.path->setValue(this->GetImageFilePathQ());
    }

    RETextureObject * texture = this->retextureobject();
    if (_reTextureObjectProps.filter)
    {
        switch (texture->GetFilterType())
        {
        case RETextureFilterNearest:
            _reTextureObjectProps.filter->setValue(1);
            break;
        case RETextureFilterLinear:
            _reTextureObjectProps.filter->setValue(2);
            break;
        case RETextureFilterMipmapedStandart:
            _reTextureObjectProps.filter->setValue(3);
            break;
        case RETextureFilterMipmapedTrilinear:
            _reTextureObjectProps.filter->setValue(4);
            break;
        default:
            _reTextureObjectProps.filter->setValue(0);
            break;
        }
    }

    if (_reTextureObjectProps.frameTopLeft)
    {
        RETetragon tetr(texture->GetFrame());
        _reTextureObjectProps.frameTopLeft->setValue(QVariant(REQTConverter::GetQPointF(REPoint2(tetr.topLeftX, tetr.topLeftY))));
    }
    if (_reTextureObjectProps.frameTopRight)
    {
        RETetragon tetr(texture->GetFrame());
        _reTextureObjectProps.frameTopRight->setValue(QVariant(REQTConverter::GetQPointF(REPoint2(tetr.topRightX, tetr.topRightY))));
    }
    if (_reTextureObjectProps.frameBottomLeft)
    {
        RETetragon tetr(texture->GetFrame());
        _reTextureObjectProps.frameBottomLeft->setValue(QVariant(REQTConverter::GetQPointF(REPoint2(tetr.bottomLeftX, tetr.bottomLeftY))));
    }
    if (_reTextureObjectProps.frameBottomRight)
    {
        RETetragon tetr(texture->GetFrame());
        _reTextureObjectProps.frameBottomRight->setValue(QVariant(REQTConverter::GetQPointF(REPoint2(tetr.bottomRightX, tetr.bottomRightY))));
    }
}

QString QRETextureObjectCommon::GetRETextureObjectInfoXMLString(QString linePrefix)
{
    QString xml;

    QString filter(RETextureObject::GetXMLFilterStringByType(this->retextureobject()->GetFilterType()));

    xml += linePrefix + "<string key=\"" + RETextureObject::GetXMLFilterKeyString() + "\">" + filter + "</string>\n";
    xml += linePrefix + "<string key=\"" + RETextureObject::GetXMLPathKeyString() + "\">" + this->GetImageFilePathQ() + "</string>\n";

    RETetragon f(this->retextureobject()->GetFrame());
    QString tetr;
    tetr.sprintf(RETextureObject::GetXMLFrameFormatString(), (float)f.arr[0], (float)f.arr[1], (float)f.arr[2], (float)f.arr[3], (float)f.arr[4], (float)f.arr[5], (float)f.arr[6], (float)f.arr[7]);

    xml += linePrefix + "<string key=\"" + RETextureObject::GetXMLFrameKeyString() + "\">" + tetr + "</string>\n";

    return xml;
}

void QRETextureObjectCommon::RETextureObjectSetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL)
{
    if (TreePropertyManager::IsNotNULL(propertyManager))
    {
        if (_reTextureObjectProps.item == NULL)
        {
            QRETextureObjectCommon::CreateRETextureObjectProperties(&_reTextureObjectProps, propertyManager->GetVariantManager());
        }

        this->RETextureObjectInfoToProperties();

        if (_reTextureObjectProps.item)
        {
            if (parentItemOrNULL) { parentItemOrNULL->addSubProperty(_reTextureObjectProps.item); }
            else { propertyManager->GetPropertyBrowser()->addProperty(_reTextureObjectProps.item); }
        }
        if (_reTextureObjectProps.frame)
        {
            if (parentItemOrNULL) { parentItemOrNULL->addSubProperty(_reTextureObjectProps.frame); }
            else { propertyManager->GetPropertyBrowser()->addProperty(_reTextureObjectProps.frame); }
        }
    }
}

bool QRETextureObjectCommon::RETextureObjectOnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue)
{
    static bool isCanCheck = true;
    if (isCanCheck && property && newValue.isValid())
    {
        if (property == _reTextureObjectProps.filter)
        {
            isCanCheck = false;
            RETextureFilterType filter = RETextureFilterNone;
            switch (newValue.value<int>())
            {
            case 1: filter = RETextureFilterNearest; break;
            case 2: filter = RETextureFilterLinear; break;
            case 3: filter = RETextureFilterMipmapedStandart; break;
            case 4: filter = RETextureFilterMipmapedTrilinear; break;
            default:
                break;
            };
            this->retextureobject()->SetFilterType(filter);
            this->RETextureObjectInfoToProperties();
            isCanCheck = true;
            return true;
        }
        else if (property == _reTextureObjectProps.frameBottomLeft)
        {
            RETetragon f(this->retextureobject()->GetFrame());
            REPoint2 p(REQTConverter::GetREPoint(newValue.toPointF()));
            f.bottomLeftX = p.x;
            f.bottomLeftY = p.y;
            this->retextureobject()->SetFrame(f);
            return true;
        }
        else if (property == _reTextureObjectProps.frameBottomRight)
        {
            RETetragon f(this->retextureobject()->GetFrame());
            REPoint2 p(REQTConverter::GetREPoint(newValue.toPointF()));
            f.bottomRightX = p.x;
            f.bottomRightY = p.y;
            this->retextureobject()->SetFrame(f);
            return true;
        }
        else if (property == _reTextureObjectProps.frameTopLeft)
        {
            RETetragon f(this->retextureobject()->GetFrame());
            REPoint2 p(REQTConverter::GetREPoint(newValue.toPointF()));
            f.topLeftX = p.x;
            f.topLeftY = p.y;
            this->retextureobject()->SetFrame(f);
            return true;
        }
        else if (property == _reTextureObjectProps.frameTopRight)
        {
            RETetragon f(this->retextureobject()->GetFrame());
            REPoint2 p(REQTConverter::GetREPoint(newValue.toPointF()));
            f.topRightX = p.x;
            f.topRightY = p.y;
            this->retextureobject()->SetFrame(f);
            return true;
        }
        else if (property == _reTextureObjectProps.path)
        {
            isCanCheck = false;
            QString path(newValue.toString());
            this->SetImageFilePathQ(path);
            this->RETextureObjectInfoToProperties();
            isCanCheck = true;
            return true;
        }
    }

    return false;
}

bool QRETextureObjectCommon::RETextureObjectOnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos)
{
    return false;
}

void QRETextureObjectCommon::SetImageFilePathQ(const QString & path)
{
    _imageFileBuffer.Clear();
    _imageFilePath.clear();

    REResourcesStorage res;
    REBuffer buff;
    REString rePath(REQTConverter::GetREString(path));
    const  bool isReadedFromResources = res.ReadToBuffer(rePath, &buff);
    if (isReadedFromResources)
    {
        REImage img;
        if (img.InitFromFileDataBuffer((REUByte*)buff.GetBuffer(), buff.GetSize()))
        {
            _imageFilePath = path;
            _imageFileBuffer = buff;
            this->ReloadTextureUsingFileBuffer();
        }
    }


    buff.Clear();
    if (REQTConverter::QFileReadToBuffer(path, &buff))
    {
        REImage img;
        if (img.InitFromFileDataBuffer((REUByte*)buff.GetBuffer(), buff.GetSize()))
        {
            _imageFileBuffer = buff;
            _imageFilePath = path;
            this->ReloadTextureUsingFileBuffer();
        }
    }
}

void QRETextureObjectCommon::SetFilterTypeQ(const RETextureFilterType filter)
{
    _filterTypeQ = filter;
}

void QRETextureObjectCommon::ReloadTextureUsingFileBuffer()
{
    GLRenderer::PauseRendering();

    RETextureObject * texture = this->retextureobject();
    const RETextureFilterType filter = (_filterTypeQ != RETextureFilterNone) ? _filterTypeQ : RETextureFilterLinear;
    const RETetragon frame(texture->GetFrame());

    texture->Clear();

    if (_imageFileBuffer.GetSize())
    {
        REImage img;
        if (img.InitFromFileDataBuffer((REUByte*)_imageFileBuffer.GetBuffer(), _imageFileBuffer.GetSize()))
        {
            if (this->InitBlankTextureQ(img.GetPixelFormat(), img.GetWidth(), img.GetHeight(), filter))
            {
                if (texture->Update(img.GetImageData(), img.GetPixelFormat(), img.GetWidth(), img.GetHeight()))
                {
                    texture->SetFrame(frame);
                }
            }
        }
    }

    GLRenderer::ResumeRendering();
}

void QRETextureObjectCommon::SaveTextureQ(SaveVCInfo * info, REView * view)
{
    if (info && view && (!_imageFilePath.isEmpty()) && (_imageFileBuffer.GetSize() > 0))
    {
        QSizeF viewSize = info->GetREViewResultScreenSize(view);
        if (viewSize.width() <= 0.0) return;

        REBuffer writeBuffer(_imageFileBuffer);
        if (writeBuffer.GetSize() != _imageFileBuffer.GetSize()) return;

        REImage img;
        if (!img.InitFromFileDataBuffer((REUByte*)_imageFileBuffer.GetBuffer(), _imageFileBuffer.GetSize())) return;

        QStringPath p(_imageFilePath);
        QStringPath fileName(p.lastPathComponent());
        if (fileName.isEmpty()) return;

        if (info->IsScaleTextureImageForScreenSize())
        {
            const REUInt32 w = RETextureObject::GetNearestPowerOfTwo((REUInt32)viewSize.width());
            const REUInt32 h = RETextureObject::GetNearestPowerOfTwo((REUInt32)viewSize.height());
            if (!img.ScaleToSize(w, h)) return;
            REImageManager manager;
            REBufferObject * pngBuff = manager.CreatePNGFilePresentation(img.GetImageData(), img.GetWidth(), img.GetHeight(), img.GetPixelFormat());
            if (pngBuff == NULL) return;
            writeBuffer.Set(pngBuff->GetBuffer(), pngBuff->GetSize());
            RE_SAFE_RELEASE(pngBuff);
            fileName.removePathExtension();
            fileName.addPathExtension(QString("png"));
        }

        if (info->IsChangeTextureImageFormatForSmallestFileSize())
        {
            const REImagePixelFormat format = img.GetPixelFormat();
            const bool isHasAlpha = ((format == REImagePixelFormatR8G8B8A8) ||
                                     (format == REImagePixelFormatLuminance8Alpha8) ||
                                     (format == REImagePixelFormatAlpha8));
            REImageManager manager;
            REBufferObject * buff = manager.CreatePNGFilePresentation(img.GetImageData(), img.GetWidth(), img.GetHeight(), format);
            if (buff == NULL) return;
            //if (isHasAlpha)
            //{
                REBufferObject * webp = manager.CreateWebPFilePresentation(img.GetImageData(), img.GetWidth(), img.GetHeight(), format, 90.0f);
                if (webp == NULL) { buff->Release(); return; }
                if (webp->GetSize() < buff->GetSize())
                {
                    buff->Release();
                    buff = webp;
                    fileName.removePathExtension();
                    fileName.addPathExtension(QString("webp"));
                }
            //}
            writeBuffer.Set(buff->GetBuffer(), buff->GetSize());
            buff->Release();
        }

        if (writeBuffer.GetSize() > 0)
        {
            QStringPath storePath = info->GetTextureQStoreFullPath();
            storePath.appendPath(fileName);
            if (REQTConverter::REBufferWriteToPath(&writeBuffer, storePath))
            {
                storePath = info->GetTextureQStoreDataPath();
                storePath.appendPath(fileName);
                _imageFilePath = storePath;
            }
        }
    }
}

QRETextureObjectCommon::QRETextureObjectCommon()
{
    memset(&_reTextureObjectProps, 0, sizeof(QRERETextureObjectProps));
    _filterTypeQ = RETextureFilterNone;
}

QRETextureObjectCommon::~QRETextureObjectCommon()
{
    QREObjectCommon::RemoveAllSubProperties(_reTextureObjectProps.item);
    QREObjectCommon::RemoveAllSubProperties(_reTextureObjectProps.frame);
}





void QRETextureObject::SetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL)
{
    QREObjectCommon::SetProperties(propertyManager, parentItemOrNULL);
    this->RETextureObjectSetProperties(propertyManager, parentItemOrNULL);
}

bool QRETextureObject::OnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue)
{
    if (QREObjectCommon::OnValueOfPropertyChanged(property, newValue))
    {
        return true;
    }
    return this->RETextureObjectOnValueOfPropertyChanged(property, newValue);
}

bool QRETextureObject::OnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos)
{
    if (QREObjectCommon::OnPropertyCustomContextMenuRequested(property, pos))
    {
        return true;
    }
    return this->RETextureObjectOnPropertyCustomContextMenuRequested(property, pos);
}

void QRETextureObject::SetFilterType(const RETextureFilterType filter)
{
    this->SetFilterTypeQ(filter);
    RETextureObject::SetFilterType(filter);
    this->ReloadTextureUsingFileBuffer();
}

QString QRETextureObject::GetXMLString(QString linePrefix, QString keyPurpose)
{
    QString xml;

    xml += linePrefix + "<object class=\"" + this->GetClassNameQ() + "\"";
    if (keyPurpose.isEmpty()) xml += ">\n";
    else xml += " key=\"" + keyPurpose + "\">\n";

    xml += this->GetREObjectInfoXMLString((linePrefix + "\t"));
    xml += this->GetRETextureObjectInfoXMLString((linePrefix + "\t"));

    xml += linePrefix + "</object>\n";

    return xml;
}

REBOOL QRETextureObject::UpdateFromImageFilePath(const REString & imageFilePath, const RETextureFilterType filterType)
{
    QRETextureObject * qretexture = dynamic_cast<QRETextureObject*>(this);
    if (qretexture)
    {
        qretexture->SetImageFilePathQ(QString(imageFilePath.UTF8String()));
        qretexture->SetFilterTypeQ(filterType);
        return true;
    }
    return false;
}

QRETextureObject::QRETextureObject() :
    RETextureObject(),
    QREObjectCommon(),
    QRETextureObjectCommon()
{

}

QRETextureObject::~QRETextureObject()
{

}

