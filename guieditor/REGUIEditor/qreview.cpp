
#include "qreview.h"
#include <QFileDialog>

void QREViewCommon::CreateREViewProperties(QREREViewProps * props,
                                           QtVariantPropertyManager * manager)
{
    props->item = manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("REView"));

    props->screenFramePx = manager->addProperty(QVariant::Rect, QLatin1String("Screen Frame (pixels)"));
    props->item->addSubProperty(props->screenFramePx);

    props->framePx = manager->addProperty(QVariant::Rect, QLatin1String("Frame (pixels)"));
    props->item->addSubProperty(props->framePx);

    props->screenFrameFl = manager->addProperty(QVariant::RectF, QLatin1String("Screen Frame (floats)"));
    props->screenFrameFl->setAttribute(QLatin1String("decimals"), 6);
    props->screenFrameFl->setEnabled(false);
    props->item->addSubProperty(props->screenFrameFl);

    props->frameFl = manager->addProperty(QVariant::RectF, QLatin1String("Frame (floats)"));
    props->frameFl->setAttribute(QLatin1String("decimals"), 6);
    props->frameFl->setEnabled(false);
    props->item->addSubProperty(props->frameFl);

    props->color = manager->addProperty(QVariant::Color, QLatin1String("Color"));
    props->item->addSubProperty(props->color);

    props->tag = manager->addProperty(QVariant::Int, QLatin1String("Tag"));
    props->item->addSubProperty(props->tag);

    props->isVisible = manager->addProperty(QVariant::Bool, QLatin1String("Is Visible"));
    props->item->addSubProperty(props->isVisible);

    props->isRespondsForUserAction = manager->addProperty(QVariant::Bool, QLatin1String("Is Responds User Action"));
    props->item->addSubProperty(props->isRespondsForUserAction);

    props->interceptsUserAction = manager->addProperty(QVariant::Bool, QLatin1String("Is Intercepts User Action"));
    props->item->addSubProperty(props->interceptsUserAction);

    props->textureItem = manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("Texture"));
    props->item->addSubProperty(props->textureItem);
}

void QREViewCommon::REViewInfoToProperties()
{
    REView * view = this->review();
    if (_reViewProps.color)
    {
        QColor c(REQTConverter::GetQColor(view->GetColor()));
        _reViewProps.color->setValue(c);
    }
    if (_reViewProps.tag)
    {
        _reViewProps.tag->setValue(QVariant(view->GetTag()));
    }
    if (_reViewProps.isVisible)
    {
        _reViewProps.isVisible->setValue(QVariant((bool)view->IsVisible()));
    }
    if (_reViewProps.isRespondsForUserAction)
    {
        _reViewProps.isRespondsForUserAction->setValue(QVariant((bool)view->IsRespondsForUserAction()));
    }
    if (_reViewProps.interceptsUserAction)
    {
        _reViewProps.interceptsUserAction->setValue(QVariant((bool)view->IsInterceptsUserAction()));
    }
    if (_reViewProps.frameFl)
    {
        _reViewProps.frameFl->setValue(QVariant(REQTConverter::GetQRectF(view->GetFrame())));
    }
    if (_reViewProps.screenFrameFl)
    {
        _reViewProps.screenFrameFl->setValue(QVariant(REQTConverter::GetQRectF(view->GetScreenFrame())));
    }
    if (_reViewProps.screenFramePx)
    {
        RERect scrFrame(QREView::ConvertRenderFrameToScreenFrameInPixels(view->GetScreenFrame()));
        _reViewProps.screenFramePx->setValue(QVariant(REQTConverter::GetQRectInt(scrFrame)));
    }
    if (_reViewProps.framePx)
    {
        RERect scrFrame(QREView::ConvertRenderFrameToScreenFrameInPixels(view->GetFrame()));
        _reViewProps.framePx->setValue(QVariant(REQTConverter::GetQRectInt(scrFrame)));
    }
}

QString QREViewCommon::GetREViewInfoXMLString(QString linePrefix)
{
    QString xml;

    REView * v = this->review();

    RERect frame(v->GetFrame());
    xml += linePrefix + "<string key=\"" + REView::GetXMLFrameKeyString() + "\">" +
            QString().sprintf(REView::GetXMLFrameFormatString(), (float)frame.x, (float)frame.y, (float)frame.width, (float)frame.height) + "</string>\n";

    REColor color(v->GetColor());
    xml += linePrefix + "<string key=\"" + REView::GetXMLColorKeyString() + "\">" +
            QString().sprintf(REView::GetXMLColorFormatString(), (float)color.red, (float)color.green, (float)color.blue, (float)color.alpha) + "</string>\n";

    xml += linePrefix + "<integer key=\"" + REView::GetXMLTagKeyString() + "\">" +
            QString().sprintf(REView::GetXMLTagFormatString(), (int)v->GetTag()) + "</integer>\n";


    xml += linePrefix + "<integer key=\"" + REView::GetXMLVisibilityKeyString() + "\">" +
            QString().sprintf(REView::GetXMLVisibilityFormatString(), (v->IsVisible() ? (int)1 : (int)0))  + "</integer>\n";

    xml += linePrefix + "<integer key=\"" + REView::GetXMLRespondsUserActionKeyString() + "\">" +
            QString().sprintf(REView::GetXMLRespondsUserActionFormatString(), (v->IsRespondsForUserAction() ? (int)1 : (int)0)) + "</integer>\n";

    xml += linePrefix + "<integer key=\"" + REView::GetXMLInterceptsUserActionKeyString() + "\">" +
            QString().sprintf(REView::GetXMLInterceptsUserActionFormatString(), (v->IsInterceptsUserAction() ? (int)1 : (int)0)) + "</integer>\n";

    return xml;
}

QString QREViewCommon::SubViewsInfoXMLString(QString linePrefix, REObjectsArray * arr)
{
    QString xml;

    if (arr)
    {
        for (REUInt32 i = 0; i < arr->Count(); i++)
        {
            GUIObjectsTreeItem * newItem = new GUIObjectsTreeItem();
            REView * view = dynamic_cast<REView*>(arr->At(i));
            if (this->IsCanAddSubViewToXMLOutPut(view))
            {
                QREObjectCommon * comObj = QREGUIObjectsHelper::GetCommonObject(arr->At(i));
                const QREGUIObjectType type = QREGUIObjectsHelper::GetType(comObj);
                if (newItem && comObj && (type != QREGUIObjectType_NONE))
                {
                    xml += comObj->GetXMLString((linePrefix + "\t"), QString(REView::GetXMLSubViewObjectKeyString()));
                }
            }
        }
    }

    return xml;
}

bool QREViewCommon::IsCanAddSubViewToXMLOutPut(REView * subView)
{
    if (subView)
    {
        return true;
    }
    return false;
}

QString QREViewCommon::GetREViewSubViewsInfoXMLString(QString linePrefix)
{
    QString xml;

    xml += this->SubViewsInfoXMLString(linePrefix, this->review()->GetSubViewsArray());

    return xml;
}

void QREViewCommon::REViewSetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL)
{
    if (TreePropertyManager::IsNotNULL(propertyManager))
    {
        if (_reViewProps.item == NULL)
        {
            QREViewCommon::CreateREViewProperties(&_reViewProps, propertyManager->GetVariantManager());
        }

        this->REViewInfoToProperties();

        if (_reViewProps.item)
        {
            if (parentItemOrNULL) { parentItemOrNULL->addSubProperty(_reViewProps.item); }
            else { propertyManager->GetPropertyBrowser()->addProperty(_reViewProps.item); }
        }
    }
}

bool QREViewCommon::REViewOnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue)
{
    static bool isCanProcessNewPropertyValue = true;
    if (property && newValue.isValid() && isCanProcessNewPropertyValue)
    {
        if (property == _reViewProps.color)
        {
            QColor q_color = newValue.value<QColor>();
            this->review()->SetColor(REQTConverter::GetREColor(q_color));
            return true;
        }
        else if (property == _reViewProps.tag)
        {
            bool isOk = false;
            const int t = newValue.toInt(&isOk);
            if (isOk) { this->review()->SetTag((REInt32)t); }
            return true;
        }
        else if (property == _reViewProps.interceptsUserAction)
        {
            this->review()->SetInterceptsUserAction((REBOOL)newValue.toBool());
            return true;
        }
        else if (property == _reViewProps.isRespondsForUserAction)
        {
            this->review()->SetRespondsForUserAction((REBOOL)newValue.toBool());
            return true;
        }
        else if (property == _reViewProps.isVisible)
        {
            this->review()->SetVisible((REBOOL)newValue.toBool());
            return true;
        }
        else if (property == _reViewProps.framePx)
        {
            isCanProcessNewPropertyValue = false;
            RERect f = QREView::ConvertScreenFrameIInPixelsToRenderFrame(newValue.toRect());
            this->review()->SetFrame(f);
            this->REViewInfoToProperties();
            isCanProcessNewPropertyValue = true;
            return true;
        }
        else if (property == _reViewProps.screenFramePx)
        {
            isCanProcessNewPropertyValue = false;
            RERect scrFrame = QREView::ConvertScreenFrameIInPixelsToRenderFrame(newValue.toRect());
            RERect viewFrame(scrFrame);
            REView * parentView = this->review()->GetParentSubViewsContainer();
            if (parentView)
            {
                RERect parentScrFrame(parentView->GetScreenFrame());
                viewFrame.x = (REFloat32)((long double)scrFrame.x - (long double)parentScrFrame.x);
                viewFrame.y = (REFloat32)((long double)scrFrame.y - (long double)parentScrFrame.y);
            }
            this->review()->SetFrame(viewFrame);
            this->REViewInfoToProperties();
            isCanProcessNewPropertyValue = true;
            return true;
        }
    }

    return false;
}

bool QREViewCommon::REViewOnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos)
{
    //TODO:
    return false;
}

QRETextureObject * QREViewCommon::GetTextureQ()
{
    RETextureObject * texture = this->review()->GetTexture();
    if (texture)
    {
        QRETextureObject * qTexture = dynamic_cast<QRETextureObject *>(texture);
        return qTexture;
    }
    return NULL;
}

bool QREViewCommon::SetNewTextureQ()
{
    this->RemoveTextureQ();

    QRETextureObject * newText = new QRETextureObject();
    if (newText)
    {
        this->review()->SetTexture(newText);
        newText->Release();
        return true;
    }
    return false;
}

bool QREViewCommon::OpenTextureImageQ(QRETextureObject * qTexture)
{
    if (qTexture)
    {
        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::ExistingFile);
        dialog.setOption(QFileDialog::ReadOnly, true);
        dialog.setNameFilter("All supported images (*.png *.jpg *.jpeg *.jp2 *.jpeg2000 *.webp)");
        QStringList fileNames;
        if (dialog.exec())
        {
            fileNames = dialog.selectedFiles();
            if (!fileNames.isEmpty())
            {
                QString path(fileNames.first());
                qTexture->SetImageFilePathQ(path);
                return true;
            }
        }
    }
    return false;
}

bool QREViewCommon::OpenTextureImageQ()
{
    QRETextureObject * t = this->GetTextureQ();
    if (t == NULL)
    {
        this->SetNewTextureQ();
    }
    t = this->GetTextureQ();
    return QREViewCommon::OpenTextureImageQ(t);
}

void QREViewCommon::RemoveTextureQ()
{
    this->review()->SetTexture(NULL);
    QREObjectCommon::RemoveAllSubProperties(_reViewProps.textureItem);
}

void QREViewCommon::RenderHighLight()
{
    RERect f(this->review()->GetFrame());

    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

    const GLboolean isTexture = glIsEnabled(GL_TEXTURE_2D);
    const GLboolean isBlend = glIsEnabled(GL_BLEND);
    const GLboolean isLIGHTING = glIsEnabled(GL_LIGHTING);


    if (isTexture) glDisable(GL_TEXTURE_2D);
    if (isBlend) glDisable(GL_BLEND);
    if (isLIGHTING) glDisable(GL_LIGHTING);

    glLineWidth(2);

    glBegin(GL_LINE_LOOP);
    glVertex2f(f.x, f.y);
    glVertex2f(f.x + f.width, f.y);
    glVertex2f(f.x + f.width, f.y + f.height);
    glVertex2f(f.x, f.y + f.height);
    glEnd();

    glLineWidth(1);

    if (isTexture) glEnable(GL_TEXTURE_2D);
    if (isBlend) glEnable(GL_BLEND);
    if (isLIGHTING) glEnable(GL_LIGHTING);
}

void QREViewCommon::SaveREViewSubViewsWithInfo(SaveVCInfo * info)
{
    REObjectsArray * arr = this->review()->GetSubViewsArray();
    if (arr)
    {
        for (REUInt32 i = 0; i < arr->Count(); i++)
        {
            GUIObjectsTreeItem * newItem = new GUIObjectsTreeItem();
            REView * view = dynamic_cast<REView*>(arr->At(i));
            if (view)
            {
                QREObjectCommon * comObj = QREGUIObjectsHelper::GetCommonObject(arr->At(i));
                const QREGUIObjectType type = QREGUIObjectsHelper::GetType(comObj);
                if (newItem && comObj && (type != QREGUIObjectType_NONE))
                {
                    comObj->SaveQ(info);
                }
            }
        }
    }
}

QREViewCommon::QREViewCommon()
{
    memset(&_reViewProps, 0, sizeof(QREREViewProps));
}

QREViewCommon::~QREViewCommon()
{
    QREObjectCommon::RemoveAllSubProperties(_reViewProps.item);
    QREObjectCommon::RemoveAllSubProperties(_reViewProps.textureItem);
}



void QREView::SetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL)
{
    QREObjectCommon::SetProperties(propertyManager, parentItemOrNULL);

    this->REViewSetProperties(propertyManager, parentItemOrNULL);

    QRETextureObject * texture = this->GetTextureQ();
    if (texture)
    {
        texture->SetProperties(propertyManager, _reViewProps.textureItem);
    }
}

bool QREView::OnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue)
{
    if (QREObjectCommon::OnValueOfPropertyChanged(property, newValue))
    {
        return true;
    }

    if (this->REViewOnValueOfPropertyChanged(property, newValue))
    {
        return true;
    }

    QRETextureObject * qTexture = this->GetTextureQ();
    if (qTexture)
    {
        if (qTexture->OnValueOfPropertyChanged(property, newValue))
        {
            return true;
        }
    }
    return false;
}

bool QREView::OnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos)
{
    if (QREObjectCommon::OnPropertyCustomContextMenuRequested(property, pos))
    {
        return true;
    }

    if (this->REViewOnPropertyCustomContextMenuRequested(property, pos))
    {
        return true;
    }

    QRETextureObject * qTexture = this->GetTextureQ();
    if (qTexture)
    {
        if (qTexture->OnPropertyCustomContextMenuRequested(property, pos))
        {
            return true;
        }
    }

    return false;
}

QString QREView::GetXMLString(QString linePrefix, QString keyPurpose)
{
    QString xml;

    xml += linePrefix + "<object class=\"" + this->GetClassNameQ() + "\"";
    if (keyPurpose.isEmpty()) xml += ">\n";
    else xml += " key=\"" + keyPurpose + "\">\n";

    xml += this->GetREObjectInfoXMLString((linePrefix + "\t"));
    xml += this->GetREViewInfoXMLString((linePrefix + "\t"));
    QRETextureObject * texture = this->GetTextureQ();
    if (texture)
    {
        if (!texture->IsQRETextureObjectNull())
        {
            xml += texture->GetXMLString((linePrefix + "\t"), QString(REView::GetXMLTextureObjectKeyString()));
        }
    }

    xml += this->GetREViewSubViewsInfoXMLString((linePrefix + "\t"));

    xml += linePrefix + "</object>\n";

    return xml;
}

void QREView::Render()
{
    REView::Render();
    if (this->IsHighLighted()) this->RenderHighLight();
}

void QREView::RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
    REView::RenderWithOffset(offsetX, offsetY);
    if (this->IsHighLighted()) this->RenderHighLight();
}

void QREView::SaveQ(SaveVCInfo * info)
{
    QRETextureObject * texture = this->GetTextureQ();
    if (texture)
    {
        texture->SaveTextureQ(info, this);
    }
    this->SaveREViewSubViewsWithInfo(info);
}

QREView::QREView() :
    REView(),
    QREObjectCommon(),
    QREViewCommon()
{

}

QREView::~QREView()
{

}

RERect QREView::ConvertRenderFrameToScreenFrameInPixels(const RERect & f)
{
    RERect resFrame;
    RERenderDevice * device = RERenderDevice::GetDefaultDevice();
    if (device)
    {
        RESize ratio(device->GetRenderToScreenSizeRatio());

        resFrame.x = (REFloat32)((long double)f.x * (long double)ratio.width);
        resFrame.width = (REFloat32)((long double)f.width * (long double)ratio.width);

        resFrame.y = (REFloat32)((long double)f.y * (long double)ratio.height);
        resFrame.height = (REFloat32)((long double)f.height * (long double)ratio.height);
    }
    return resFrame;
}

RERect QREView::ConvertScreenFrameFInPixelsToRenderFrame(const QRectF & f)
{
    RERect rr;
    RERenderDevice * device = RERenderDevice::GetDefaultDevice();
    if (device)
    {
        RESize ratio(device->GetScreenToRenderSizeRatio());

        rr.x = (REFloat32)((long double)f.x() * (long double)ratio.width);
        rr.width = (REFloat32)((long double)f.width() * (long double)ratio.width);

        rr.y = (REFloat32)((long double)f.y() * (long double)ratio.height);
        rr.height = (REFloat32)((long double)f.height() * (long double)ratio.height);
    }
    return rr;
}

RERect QREView::ConvertScreenFrameIInPixelsToRenderFrame(const QRect & f)
{
    RERect rr;
    RERenderDevice * device = RERenderDevice::GetDefaultDevice();
    if (device)
    {
        RESize ratio(device->GetScreenToRenderSizeRatio());

        rr.x = (REFloat32)((long double)f.x() * (long double)ratio.width);
        rr.width = (REFloat32)((long double)f.width() * (long double)ratio.width);

        rr.y = (REFloat32)((long double)f.y() * (long double)ratio.height);
        rr.height = (REFloat32)((long double)f.height() * (long double)ratio.height);
    }
    return rr;
}

REPoint2 QREView::ConvertScreenPointIInPixelsToRenderPoint(const QPoint & p)
{
    REPoint2 rr;
    RERenderDevice * device = RERenderDevice::GetDefaultDevice();
    if (device)
    {
        RESize ratio(device->GetScreenToRenderSizeRatio());
        rr.x = (REFloat32)((long double)p.x() * (long double)ratio.width);
        rr.y = (REFloat32)((long double)p.y() * (long double)ratio.height);
    }
    return rr;
}
