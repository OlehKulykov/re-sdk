
#include "qretextfield.h"
#include "glrenderer.h"

void QRETextFieldCommon::CreateRETextFieldProperties(QRERETextFieldProps * props,
                                                     QtVariantPropertyManager * manager)
{
    props->item = manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("RETextField"));
    props->cursorView = manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("Cursor View"));
    props->item->addSubProperty(props->cursorView);

    props->cursorBlinkTime = manager->addProperty(QVariant::Double, QLatin1String("Cursor Blink Time (sec)"));
    props->cursorBlinkTime->setAttribute(QLatin1String("decimals"), 6);
    props->item->addSubProperty(props->cursorBlinkTime);

    props->maximumInputedTextLength = manager->addProperty(QVariant::Int, QLatin1String("Maximum Inputed Text Length"));
    props->item->addSubProperty(props->maximumInputedTextLength);

    props->isHasMaximumInputedTextLength = manager->addProperty(QVariant::Bool, QLatin1String("Has Maximum Inputed Text Length"));
    props->item->addSubProperty(props->isHasMaximumInputedTextLength);

    props->isClearPreviousTextInputText = manager->addProperty(QVariant::Bool, QLatin1String("Clear Previous Text Input Text"));
    props->item->addSubProperty(props->isClearPreviousTextInputText);

    props->isShowCursor = manager->addProperty(QVariant::Bool, QLatin1String("Show Cursor"));
    props->item->addSubProperty(props->isShowCursor);
}

void QRETextFieldCommon::RETextFieldInfoToProperties()
{
    if (_reTextFieldProps.cursorBlinkTime)
    {
        _reTextFieldProps.cursorBlinkTime->setValue(QVariant((double)this->retextfield()->GetCursorBlinkTime()));
    }
    if (_reTextFieldProps.maximumInputedTextLength)
    {
        _reTextFieldProps.maximumInputedTextLength->setValue(QVariant((int)this->retextfield()->GetTextInputResponderMaximumTextLength()));
    }
    if (_reTextFieldProps.isHasMaximumInputedTextLength)
    {
        _reTextFieldProps.isHasMaximumInputedTextLength->setValue(QVariant((bool)this->retextfield()->IsTextInputResponderHasMaximumTextLength()));
    }
    if (_reTextFieldProps.isClearPreviousTextInputText)
    {
        _reTextFieldProps.isClearPreviousTextInputText->setValue(QVariant((bool)this->retextfield()->IsClearPreviousInputedText()));
    }
    if (_reTextFieldProps.isShowCursor)
    {
        _reTextFieldProps.isShowCursor->setValue(QVariant((bool)this->retextfield()->IsShowCursor()));
    }
}


QString QRETextFieldCommon::GetRETextFieldInfoXMLString(QString linePrefix)
{
    QString xml;

    RETextField * textField = this->retextfield();

    xml += linePrefix + "<string key=\""+ RETextField::GetXMLCursorBlinkTimeKeyString() +"\">" +
            QString().sprintf(RETextField::GetXMLCursorBlinkTimeFormatString(), (float)textField->GetCursorBlinkTime()) + "</string>\n";

    xml += linePrefix + "<integer key=\""+ RETextField::GetXMLMaximumTextLengthKeyString() +"\">" +
            QString().sprintf(RETextField::GetXMLMaximumTextLengthFormatString(), (int)textField->GetTextInputResponderMaximumTextLength()) + "</integer>\n";

    xml += linePrefix + "<integer key=\""+ RETextField::GetXMLHasMaximumTextLengthKeyString() +"\">" +
            QString().sprintf(RETextField::GetXMLHasMaximumTextLengthFormatString(), (textField->IsTextInputResponderHasMaximumTextLength() ? (int)1 : (int)0))
            + "</integer>\n";

    xml += linePrefix + "<integer key=\""+ RETextField::GetXMLClearPreviousTextKeyString() +"\">" +
            QString().sprintf(RETextField::GetXMLClearPreviousTextFormatString(), (textField->IsClearPreviousInputedText() ? (int)1 : (int)0))
             + "</integer>\n";

    xml += linePrefix + "<integer key=\""+ RETextField::GetXMLShowCursorKeyString() +"\">" +
            QString().sprintf(RETextField::GetXMLShowCursorFormatString(), (textField->IsShowCursor() ? (int)1 : (int)0)) + "</integer>\n";

    return xml;
}

void QRETextFieldCommon::RETextFieldSetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL)
{
    if (TreePropertyManager::IsNotNULL(propertyManager))
    {
        if (_reTextFieldProps.item == NULL)
        {
            QRETextField::CreateRETextFieldProperties(&_reTextFieldProps, propertyManager->GetVariantManager());
        }

        this->RETextFieldInfoToProperties();

        if (_reTextFieldProps.item)
        {
            propertyManager->GetPropertyBrowser()->addProperty(_reTextFieldProps.item);
        }
    }
}

bool QRETextFieldCommon::RETextFieldOnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue)
{
    if (property && newValue.isValid())
    {
        if (property == _reTextFieldProps.cursorBlinkTime)
        {
            bool isOk = false;
            const double v = newValue.toDouble(&isOk);
            if (isOk) { this->retextfield()->SetCursorBlinkTime((RETimeInterval)v); }
            return true;
        }
        else if (property == _reTextFieldProps.isClearPreviousTextInputText)
        {
            this->retextfield()->SetClearPreviousInputedText((REBOOL)newValue.toBool());
            return true;
        }
        else if (property == _reTextFieldProps.isHasMaximumInputedTextLength)
        {
            this->retextfield()->SetHasMaximumInputedTextLength((REBOOL)newValue.toBool());
            return true;
        }
        else if (property == _reTextFieldProps.isShowCursor)
        {
            this->retextfield()->SetShowCursor((REBOOL)newValue.toBool());
            return true;
        }
        else if (property == _reTextFieldProps.maximumInputedTextLength)
        {
            bool isOk = false;
            const int v = newValue.toInt(&isOk);
            if (isOk) { this->retextfield()->SetMaximumInputedTextLength((REUInt32)v); }
            return true;
        }
    }
    return false;
}

bool QRETextFieldCommon::RETextFieldOnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos)
{
    return false;
}

QRETextFieldCommon::QRETextFieldCommon()
{
    memset(&_reTextFieldProps, 0, sizeof(QRERETextFieldProps));
}

QRETextFieldCommon::~QRETextFieldCommon()
{
    QREObjectCommon::RemoveAllSubProperties(_reTextFieldProps.item);
    QREObjectCommon::RemoveAllSubProperties(_reTextFieldProps.cursorView);
}





void QRETextField::SetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL)
{
    QREObjectCommon::SetProperties(propertyManager, parentItemOrNULL);
    this->REViewSetProperties(propertyManager, parentItemOrNULL);
    this->RELabelSetProperties(propertyManager, parentItemOrNULL);
    this->RETextFieldSetProperties(propertyManager, parentItemOrNULL);
}

bool QRETextField::OnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue)
{
    if (QREObjectCommon::OnValueOfPropertyChanged(property, newValue))
    {
        return true;
    }
    if (this->REViewOnValueOfPropertyChanged(property, newValue))
    {
        //TODO: check frame
        return true;
    }
    if (this->RELabelOnValueOfPropertyChanged(property, newValue))
    {
        return true;
    }
    if (this->RETextFieldOnValueOfPropertyChanged(property, newValue))
    {
        return true;
    }
    return false;
}

bool QRETextField::OnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos)
{
    return false;
}

QString QRETextField::GetXMLString(QString linePrefix, QString keyPurpose)
{
    QString xml;

    xml += linePrefix + "<object class=\"" + this->GetClassNameQ() + "\"";
    if (keyPurpose.isEmpty()) xml += ">\n";
    else xml += " key=\"" + keyPurpose + "\">\n";

    xml += this->GetREObjectInfoXMLString((linePrefix + "\t"));
    xml += this->GetREViewInfoXMLString((linePrefix + "\t"));
    xml += this->GetRELabelInfoXMLString((linePrefix + "\t"));
    xml += this->GetRETextFieldInfoXMLString((linePrefix + "\t"));

    QRETextureObject * texture = this->GetTextureQ();
    if (texture)
    {
        if (!texture->IsQRETextureObjectNull())
        {
            xml += texture->GetXMLString((linePrefix + "\t"), QString(REView::GetXMLTextureObjectKeyString()));
        }
    }

    QREFontObject * font = this->GetFontQ();
    if (font)
    {
        if (!font->IsQREFontObjectNull())
        {
            xml += font->GetXMLString((linePrefix + "\t"), QString(RELabel::GetXMLFontObjectKeyString()));
        }
    }

    xml += this->GetREViewSubViewsInfoXMLString((linePrefix + "\t"));

    xml += linePrefix + "</object>\n";

    return xml;
}

bool QRETextField::IsCanAddSubViewToXMLOutPut(REView * subView)
{
    if (QREViewCommon::IsCanAddSubViewToXMLOutPut(subView))
    {
        REView * cursor = this->GetCursorView();
        if (cursor)
        {
            return (subView->GetObjectIdentifier() != cursor->GetObjectIdentifier());
        }
        return true;
    }
    return false;
}

void QRETextField::Render()
{
    RETextField::Render();
    if (this->IsHighLighted()) this->RenderHighLight();
}

void QRETextField::RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
    RETextField::RenderWithOffset(offsetX,offsetY);
    if (this->IsHighLighted()) this->RenderHighLight();
}

void QRETextField::SaveQ(SaveVCInfo * info)
{
    QRETextureObject * texture = this->GetTextureQ();
    if (texture)
    {
        texture->SaveTextureQ(info, this);
    }

    QREFontObject * font = this->GetFontQ();
    if (font)
    {
        font->SaveFontQ(info, this);
    }

    this->SaveREViewSubViewsWithInfo(info);
}

QRETextField::QRETextField() :
    RETextField(),
    QREObjectCommon(),
    QREViewCommon(),
    QRELabelCommon(),
    QRETextFieldCommon()
{
    if (_cursorView)
    {
        RE_SAFE_RELEASE(_cursorView);
    }

    _cursorView = new QREView();
    if (_cursorView)
    {
        if (this->AddSubView(_cursorView))
        {
            _cursorView->SetColor(REColor(0.0f, 0.0f, 0.0f, 1.0f));
        }
        else
        {
            RE_SAFE_RELEASE(_cursorView);
        }
    }
}

QRETextField::~QRETextField()
{

}
