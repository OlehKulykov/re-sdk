
#include "qrebutton.h"

void QREButtonCommon::CreateREButtonProperties(QREREButtonProps * props,
                                               QtVariantPropertyManager * manager)
{
    props->item = manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("REButton"));

    props->pressedTexture = manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("Pressed Texture"));
    props->disabledTexture = manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("Disabled Texture"));

    QStringList enumNames;
    enumNames << "Default" << "Pressed" << "Disabled";
    props->state = manager->addProperty(QtVariantPropertyManager::enumTypeId(), QLatin1String("State"));
    props->state->setAttribute(QLatin1String("enumNames"), enumNames);
    props->item->addSubProperty(props->state);

    props->isDisabled = manager->addProperty(QVariant::Bool, QLatin1String("Is Disabled"));
    props->item->addSubProperty(props->isDisabled);

    props->item->addSubProperty(props->pressedTexture);
    props->item->addSubProperty(props->disabledTexture);
}

void QREButtonCommon::REButtonInfoToProperties()
{
    if (_reButtonProps.state)
    {
        const REButton::StateType state = this->GetStateQ();
        switch (state)
        {
        case REButton::StateDefault: _reButtonProps.state->setValue(0); break;
        case REButton::StatePressed: _reButtonProps.state->setValue(1); break;
        case REButton::StateDisabled: _reButtonProps.state->setValue(2); break;
        default: break;
        }
    }

    if (_reButtonProps.isDisabled)
    {
        _reButtonProps.isDisabled->setValue(QVariant((bool)this->rebutton()->IsDisabled()));
    }
}

void QREButtonCommon::REButtonSetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL)
{
    if (TreePropertyManager::IsNotNULL(propertyManager))
    {
        if (_reButtonProps.item == NULL)
        {
            QREButton::CreateREButtonProperties(&_reButtonProps, propertyManager->GetVariantManager());
        }

        this->REButtonInfoToProperties();

        if (_reButtonProps.item)
        {
            if (parentItemOrNULL) { parentItemOrNULL->addSubProperty(_reButtonProps.item); }
            else { propertyManager->GetPropertyBrowser()->addProperty(_reButtonProps.item); }
        }
    }
}

bool QREButtonCommon::REButtonOnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue)
{
    if (property && newValue.isValid())
    {
        if (property == _reButtonProps.state)
        {
            switch (newValue.value<int>())
            {
            case 0: this->SetStateQ(REButton::StateDefault); break;
            case 1: this->SetStateQ(REButton::StatePressed); break;
            case 2: this->SetStateQ(REButton::StateDisabled); break;
            default: break;
            }
            return true;
        }
        else if (property == _reButtonProps.isDisabled)
        {
            this->rebutton()->SetDisabled((REBOOL)newValue.toBool());
            return true;
        }
    }
    return false;
}

bool QREButtonCommon::REButtonOnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos)
{
    return false;
}

bool QREButtonCommon::SetNewDefaultTextureQ()
{
    return this->qreviewcommon()->SetNewTextureQ();
}

bool QREButtonCommon::OpenDefaultTextureImageQ()
{
    return this->qreviewcommon()->OpenTextureImageQ();
}

void QREButtonCommon::RemoveDefaultTextureQ()
{
    return this->qreviewcommon()->RemoveTextureQ();
}

bool QREButtonCommon::SetNewPressedTextureQ()
{
    this->RemovePressedTextureQ();

    QRETextureObject * newText = new QRETextureObject();
    if (newText)
    {
        this->rebutton()->SetTextureForState(newText, REButton::StatePressed);
        newText->Release();
        return true;
    }
    return false;
}

bool QREButtonCommon::OpenPressedTextureImageQ()
{
    QRETextureObject * t = this->GetPressedTextureQ();
    if (t == NULL)
    {
        this->SetNewPressedTextureQ();
    }
    t = this->GetPressedTextureQ();
    return QREViewCommon::OpenTextureImageQ(t);
}

void QREButtonCommon::RemovePressedTextureQ()
{
    this->rebutton()->SetTextureForState(NULL, REButton::StatePressed);
    QREObjectCommon::RemoveAllSubProperties(_reButtonProps.pressedTexture);
}

bool QREButtonCommon::SetNewDisabledTextureQ()
{
    this->RemoveDisabledTextureQ();

    QRETextureObject * newText = new QRETextureObject();
    if (newText)
    {
        this->rebutton()->SetTextureForState(newText, REButton::StateDisabled);
        newText->Release();
        return true;
    }
    return false;
}

bool QREButtonCommon::OpenDisabledTextureImageQ()
{
    QRETextureObject * t = this->GetDisabledTextureQ();
    if (t == NULL)
    {
        this->SetNewDisabledTextureQ();
    }
    t = this->GetDisabledTextureQ();
    return QREViewCommon::OpenTextureImageQ(t);
}

void QREButtonCommon::RemoveDisabledTextureQ()
{
    this->rebutton()->SetTextureForState(NULL, REButton::StateDisabled);
    QREObjectCommon::RemoveAllSubProperties(_reButtonProps.disabledTexture);
}

QREButtonCommon::QREButtonCommon()
{
    memset(&_reButtonProps, 0, sizeof(QREREButtonProps));
}

QREButtonCommon::~QREButtonCommon()
{
    QREObjectCommon::RemoveAllSubProperties(_reButtonProps.item);
    QREObjectCommon::RemoveAllSubProperties(_reButtonProps.pressedTexture);
    QREObjectCommon::RemoveAllSubProperties(_reButtonProps.disabledTexture);
}






void QREButton::SetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL)
{
    QREObjectCommon::SetProperties(propertyManager, parentItemOrNULL);
    this->REViewSetProperties(propertyManager, parentItemOrNULL);

    QRETextureObject * qTexture = this->GetTextureQ();
    if (qTexture)
    {
        qTexture->SetProperties(propertyManager, _reViewProps.textureItem);
    }

    this->REButtonSetProperties(propertyManager, parentItemOrNULL);

    qTexture = this->GetPressedTextureQ();
    if (qTexture)
    {
        qTexture->SetProperties(propertyManager, _reButtonProps.pressedTexture);
    }

    qTexture = this->GetDisabledTextureQ();
    if (qTexture)
    {
        qTexture->SetProperties(propertyManager, _reButtonProps.disabledTexture);
    }
}

bool QREButton::OnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue)
{
    if (QREObjectCommon::OnValueOfPropertyChanged(property, newValue))
    {
        return true;
    }

    if (this->REViewOnValueOfPropertyChanged(property, newValue))
    {
        return true;
    }

    if (this->REButtonOnValueOfPropertyChanged(property, newValue))
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

    qTexture = this->GetPressedTextureQ();
    if (qTexture)
    {
        if (qTexture->OnValueOfPropertyChanged(property, newValue))
        {
            return true;
        }
    }

    qTexture = this->GetDisabledTextureQ();
    if (qTexture)
    {
        if (qTexture->OnValueOfPropertyChanged(property, newValue))
        {
            return true;
        }
    }
    return false;
}

bool QREButton::OnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos)
{
    return false;
}

QRETextureObject * QREButton::GetPressedTextureQ()
{
    RETextureObject * t = this->GetTextureForState(REButton::StatePressed);
    if (t)
    {
        return dynamic_cast<QRETextureObject *>(t);
    }
    return NULL;
}

QRETextureObject * QREButton::GetDisabledTextureQ()
{
    RETextureObject * t = this->GetTextureForState(REButton::StateDisabled);
    if (t)
    {
        return dynamic_cast<QRETextureObject *>(t);
    }
    return NULL;
}

QString QREButton::GetXMLString(QString linePrefix, QString keyPurpose)
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

    texture = this->GetPressedTextureQ();
    if (texture)
    {
        if (!texture->IsQRETextureObjectNull())
        {
            xml += texture->GetXMLString((linePrefix + "\t"), QString(REButton::GetXMLPressedTextureObjectKeyString()));
        }
    }

    texture = this->GetDisabledTextureQ();
    if (texture)
    {
        if (!texture->IsQRETextureObjectNull())
        {
            xml += texture->GetXMLString((linePrefix + "\t"), QString(REButton::GetXMLDisabledTextureObjectKeyString()));
        }
    }

    xml += this->GetREViewSubViewsInfoXMLString((linePrefix + "\t"));

    xml += linePrefix + "</object>\n";

    return xml;
}

void QREButton::Render()
{
    REButton::Render();
    if (this->IsHighLighted()) this->RenderHighLight();
}

void QREButton::RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
    REButton::RenderWithOffset(offsetX,offsetY);
    if (this->IsHighLighted()) this->RenderHighLight();
}


void QREButton::SaveQ(SaveVCInfo * info)
{
    QRETextureObject * texture = this->GetTextureQ();
    if (texture)
    {
        texture->SaveTextureQ(info, this);
    }

    texture = this->GetPressedTextureQ();
    if (texture)
    {
        texture->SaveTextureQ(info, this);
    }

    texture = this->GetDisabledTextureQ();
    if (texture)
    {
        texture->SaveTextureQ(info, this);
    }

    this->SaveREViewSubViewsWithInfo(info);
}

QREButton::QREButton() :
    REButton(),
    QREObjectCommon(),
    QREViewCommon(),
    QREButtonCommon()
{

}

QREButton::~QREButton()
{

}

