
#include "qrelabel.h"
#include <QFileDialog>
#include "glrenderer.h"

void QRELabelCommon::CreateRELabelProperties(QRERELabelProps * props,
                                             QtVariantPropertyManager * manager)
{
    props->item = manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("RELabel"));

    props->text = manager->addProperty(QVariant::String, QLatin1String("Text"));
    props->item->addSubProperty(props->text);

    props->textColor = manager->addProperty(QVariant::Color, QLatin1String("Text Color"));
    props->item->addSubProperty(props->textColor);

    props->textInsets = manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("Text Insets"));
    props->item->addSubProperty(props->textInsets);

    props->textInsetsTop = manager->addProperty(QVariant::Double, QLatin1String("Top"));
    props->textInsetsTop->setAttribute(QLatin1String("decimals"), 6);
    props->textInsets->addSubProperty(props->textInsetsTop);

    props->textInsetsLeft = manager->addProperty(QVariant::Double, QLatin1String("Left"));
    props->textInsetsLeft->setAttribute(QLatin1String("decimals"), 6);
    props->textInsets->addSubProperty(props->textInsetsLeft);

    props->textInsetsBottom = manager->addProperty(QVariant::Double, QLatin1String("Bottom"));
    props->textInsetsBottom->setAttribute(QLatin1String("decimals"), 6);
    props->textInsets->addSubProperty(props->textInsetsBottom);

    props->textInsetsRight = manager->addProperty(QVariant::Double, QLatin1String("Right"));
    props->textInsetsRight->setAttribute(QLatin1String("decimals"), 6);
    props->textInsets->addSubProperty(props->textInsetsRight);

    props->charsSpaceRatio = manager->addProperty(QVariant::Double, QLatin1String("Chars Space Ratio"));
    props->charsSpaceRatio->setAttribute(QLatin1String("decimals"), 6);
    props->item->addSubProperty(props->charsSpaceRatio);

    props->shadowOffset = manager->addProperty(QVariant::PointF, QLatin1String("Shadow Offset"));
    props->shadowOffset->setAttribute(QLatin1String("decimals"), 6);
    props->item->addSubProperty(props->shadowOffset);

    props->shadowColor = manager->addProperty(QVariant::Color, QLatin1String("Shadow Color"));
    props->item->addSubProperty(props->shadowColor);

    props->isUsingShadow = manager->addProperty(QVariant::Bool, QLatin1String("Use Shadow"));
    props->item->addSubProperty(props->isUsingShadow);

    props->isShowBackground = manager->addProperty(QVariant::Bool, QLatin1String("Show Background"));
    props->item->addSubProperty(props->isShowBackground);

    QStringList enumNames;
    enumNames << "Left" << "Center" << "Right";
    props->textAlignment = manager->addProperty(QtVariantPropertyManager::enumTypeId(), QLatin1String("Text Alignment"));
    props->textAlignment->setAttribute(QLatin1String("enumNames"), enumNames);
    props->item->addSubProperty(props->textAlignment);

    enumNames.clear();
    enumNames << "None" << "Truncate Tail" << "Truncate Middle" << "Truncate Head";
    props->lineBreakMode = manager->addProperty(QtVariantPropertyManager::enumTypeId(), QLatin1String("Line Break Mode"));
    props->lineBreakMode->setAttribute(QLatin1String("enumNames"), enumNames);
    props->item->addSubProperty(props->lineBreakMode);

    props->textLineTruncationString = manager->addProperty(QVariant::String, QLatin1String("Line Truncation String"));
    props->item->addSubProperty(props->textLineTruncationString);

    props->font = manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("Font"));
    if (props->item && props->font) props->item->addSubProperty(props->font);
}

void QRELabelCommon::RELabelInfoToProperties()
{
    RELabel * label = this->relabel();
    if (_reLabelProps.text)
    {
        REString t(label->GetText());
        QString s(REQTConverter::GetQString(t));
        _reLabelProps.text->setValue(QVariant(s));
    }
    if (_reLabelProps.textColor)
    {
        QColor c(REQTConverter::GetQColor(label->GetTextColor()));
        _reLabelProps.textColor->setValue(QVariant(c));
    }
    if (_reLabelProps.textInsetsTop)
    {
        REEdgeInsets insets(label->GetTextInsets());
        _reLabelProps.textInsetsTop->setValue(QVariant((double)insets.top));
    }
    if (_reLabelProps.textInsetsLeft)
    {
        REEdgeInsets insets(label->GetTextInsets());
        _reLabelProps.textInsetsLeft->setValue(QVariant((double)insets.left));
    }
    if (_reLabelProps.textInsetsBottom)
    {
        REEdgeInsets insets(label->GetTextInsets());
        _reLabelProps.textInsetsBottom->setValue(QVariant((double)insets.bottom));
    }
    if (_reLabelProps.textInsetsRight)
    {
        REEdgeInsets insets(label->GetTextInsets());
        _reLabelProps.textInsetsRight->setValue(QVariant((double)insets.right));
    }
    if (_reLabelProps.charsSpaceRatio)
    {
        _reLabelProps.charsSpaceRatio->setValue((QVariant((double)label->GetCharsSpaceRatio())));
    }
    if (_reLabelProps.shadowOffset)
    {
        QPointF o(REQTConverter::GetQPointF(label->GetShadowOffset()));
        _reLabelProps.shadowOffset->setValue(o);
    }
    if (_reLabelProps.shadowColor)
    {
        QColor c(REQTConverter::GetQColor(label->GetShadowColor()));
        _reLabelProps.shadowColor->setValue(QVariant(c));
    }
    if (_reLabelProps.isUsingShadow)
    {
        _reLabelProps.isUsingShadow->setValue(QVariant((bool)label->IsUsingShadow() ? true : false));
    }
    if (_reLabelProps.isShowBackground)
    {
        _reLabelProps.isShowBackground->setValue(QVariant((bool)label->IsShowBackground() ? true : false));
    }
    if (_reLabelProps.textAlignment)
    {
        switch (label->GetTextAlignment())
        {
        case RETextAlignmentLeft: _reLabelProps.textAlignment->setValue(0); break;
        case RETextAlignmentCenter: _reLabelProps.textAlignment->setValue(1); break;
        case RETextAlignmentRight: _reLabelProps.textAlignment->setValue(2); break;
        default: break;
        }
    }
    if (_reLabelProps.lineBreakMode)
    {
        switch (label->GetLineBreak())
        {
        case RETextLineBreakNone: _reLabelProps.lineBreakMode->setValue(0);  break;
        case RETextLineBreakTruncateTail: _reLabelProps.lineBreakMode->setValue(1);  break;
        case RETextLineBreakTruncateMiddle: _reLabelProps.lineBreakMode->setValue(2);  break;
        case RETextLineBreakTruncateHead: _reLabelProps.lineBreakMode->setValue(3);  break;
        default: break;
        }
    }
    if (_reLabelProps.textLineTruncationString)
    {
        REString t(label->GetTextLineTruncationString());
        QString s(REQTConverter::GetQString(t));
        _reLabelProps.textLineTruncationString->setValue(QVariant(s));
    }
}

void QRELabelCommon::RELabelSetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL)
{
    if (TreePropertyManager::IsNotNULL(propertyManager))
    {
        if (_reLabelProps.item == NULL)
        {
            QRELabel::CreateRELabelProperties(&_reLabelProps, propertyManager->GetVariantManager());
        }

        this->RELabelInfoToProperties();

        if (_reLabelProps.item)
        {
            if (parentItemOrNULL) { parentItemOrNULL->addSubProperty(_reLabelProps.item); }
            else { propertyManager->GetPropertyBrowser()->addProperty(_reLabelProps.item); }
        }
    }
}

bool QRELabelCommon::RELabelOnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue)
{
    if (property && newValue.isValid())
    {
        if (property == _reLabelProps.text)
        {
            QString t(newValue.toString());
            REString s(REQTConverter::GetREString(t));
            this->relabel()->SetText(s);
            return true;
        }
        else if (property == _reLabelProps.textColor)
        {
            QColor q_color = newValue.value<QColor>();
            this->relabel()->SetTextColor(REQTConverter::GetREColor(q_color));
            return true;
        }
        else if (property == _reLabelProps.textAlignment)
        {
            /// "Left" "Center"  "Right";
            switch (newValue.value<int>())
            {
            case 0: this->relabel()->SetTextAlignment(RETextAlignmentLeft); break;
            case 1: this->relabel()->SetTextAlignment(RETextAlignmentCenter); break;
            case 2: this->relabel()->SetTextAlignment(RETextAlignmentRight); break;
            default: break;
            }
            return true;
        }
        else if (property == _reLabelProps.textInsetsBottom)
        {
            REEdgeInsets insets(this->relabel()->GetTextInsets());
            bool isOk = false;
            const double v = newValue.toDouble(&isOk);
            if (isOk) { insets.bottom = (REFloat32)v; this->relabel()->SetTextInsets(insets); }
            return true;
        }
        else if (property == _reLabelProps.textInsetsLeft)
        {
            REEdgeInsets insets(this->relabel()->GetTextInsets());
            bool isOk = false;
            const double v = newValue.toDouble(&isOk);
            if (isOk) { insets.left = (REFloat32)v; this->relabel()->SetTextInsets(insets); }
            return true;
        }
        else if (property == _reLabelProps.textInsetsTop)
        {
            REEdgeInsets insets(this->relabel()->GetTextInsets());
            bool isOk = false;
            const double v = newValue.toDouble(&isOk);
            if (isOk) { insets.top = (REFloat32)v; this->relabel()->SetTextInsets(insets); }
            return true;
        }
        else if (property == _reLabelProps.textInsetsRight)
        {
            REEdgeInsets insets(this->relabel()->GetTextInsets());
            bool isOk = false;
            const double v = newValue.toDouble(&isOk);
            if (isOk) { insets.right = (REFloat32)v; this->relabel()->SetTextInsets(insets); }
            return true;
        }
        else if (property == _reLabelProps.isShowBackground)
        {
            this->relabel()->SetShowBackground((REBOOL)newValue.toBool());
            return true;
        }
        else if (property == _reLabelProps.isUsingShadow)
        {
            this->relabel()->SetUsingShadow((REBOOL)newValue.toBool());
            return true;
        }
        else if (property == _reLabelProps.charsSpaceRatio)
        {
            bool isOk = false;
            const double v = newValue.toDouble(&isOk);
            if (isOk) { this->relabel()->SetCharsSpaceRatio((REFloat32)v); }
            return true;
        }
        else if (property == _reLabelProps.lineBreakMode)
        {
            /// "None" << "Truncate Tail" << "Truncate Middle" << "Truncate Head";
            switch (newValue.value<int>())
            {
            case 0: this->relabel()->SetLineBreak(RETextLineBreakNone); break;
            case 1: this->relabel()->SetLineBreak(RETextLineBreakTruncateTail); break;
            case 2: this->relabel()->SetLineBreak(RETextLineBreakTruncateMiddle); break;
            case 3: this->relabel()->SetLineBreak(RETextLineBreakTruncateHead); break;
            default: break;
            }
            return true;
        }
        else if (property == _reLabelProps.shadowColor)
        {
            QColor q_color = newValue.value<QColor>();
            this->relabel()->SetShadowColor(REQTConverter::GetREColor(q_color));
            return true;
        }
        else if (property == _reLabelProps.shadowOffset)
        {
            QPointF p(newValue.toPointF());
            this->relabel()->SetShadowOffset(REQTConverter::GetREPoint(p));
            return true;
        }
        else if (property == _reLabelProps.textLineTruncationString)
        {
            QString t(newValue.toString());
            REString s(REQTConverter::GetREString(t));
            this->relabel()->SetTextLineTruncationString(s);
            s = this->relabel()->GetText();
            this->relabel()->SetText(s);
            return true;
        }
    }
    return false;
}

QString QRELabelCommon::GetRELabelInfoXMLString(QString linePrefix)
{
    QString xml;

    RELabel * label = this->relabel();

    xml += linePrefix + "<string key=\""+ RELabel::GetXMLTextKeyString() + "\">" + REQTConverter::GetQString(label->GetText()) + "</string>\n";

    REColor color(label->GetTextColor());
    xml += linePrefix + "<string key=\"" + RELabel::GetXMLTextColorKeyString() + "\">" +
            QString().sprintf(RELabel::GetXMLTextColorFormatString(), (float)color.red, (float)color.green, (float)color.blue, (float)color.alpha) +
            + "</string>\n";

    REEdgeInsets ti(label->GetTextInsets());
    xml += linePrefix + "<string key=\"" + RELabel::GetXMLTextInsetsKeyString() + "\">" +
            QString().sprintf(RELabel::GetXMLTextInsetsFormatString(), (float)ti.top, (float)ti.left, (float)ti.bottom, (float)ti.right) +
            "</string>\n";

    xml += linePrefix + "<string key=\"" + RELabel::GetXMLCharsSpaceRatioKeyString() + "\">" +
            QString().sprintf(RELabel::GetXMLCharsSpaceRatioFormatString(), (float)label->GetCharsSpaceRatio()) + "</string>\n";

    REPoint2 point(label->GetShadowOffset());
    xml += linePrefix + "<string key=\"" + RELabel::GetXMLShadowOffsetKeyString() + "\">" +
            QString().sprintf(RELabel::GetXMLShadowOffsetFormatString(), (float)point.x, (float)point.y) + "</string>\n";

    color = label->GetShadowColor();
    xml += linePrefix + "<string key=\"" + RELabel::GetXMLShadowColorKeyString() + "\">" +
            QString().sprintf(RELabel::GetXMLShadowColorFormatString(), (float)color.red, (float)color.green, (float)color.blue, (float)color.alpha) +
            + "</string>\n";

    xml += linePrefix + "<integer key=\""+ RELabel::GetXMLUsingShadowKeyString() + "\">" +
            QString().sprintf(RELabel::GetXMLUsingShadowFormatString(), (label->IsUsingShadow() ? (int)1 : (int)0)) + "</integer>\n";

    xml += linePrefix + "<integer key=\"" + RELabel::GetXMLShowBackgroundKeyString() + "\">" +
            QString().sprintf(RELabel::GetXMLShowBackgroundFormatString(), (label->IsShowBackground() ? (int)1 : (int)0)) + "</integer>\n";

    xml += linePrefix + "<string key=\"" + RELabel::GetXMLTextAlignmentKeyString() + "\">" +
            RELabel::GetXMLTextAlignmentStringByType(label->GetTextAlignment()) + "</string>\n";

    xml += linePrefix + "<string key=\""+ RELabel::GetXMLLineBreakKeyString() +"\">" +
            RELabel::GetXMLLineBreakStringByType(label->GetLineBreak()) + "</string>\n";

    xml += linePrefix + "<string key=\"" + RELabel::GetXMLTextLineTruncationStringKeyString() + "\">" + REQTConverter::GetQString(label->GetTextLineTruncationString()) + "</string>\n";

    return xml;
}

bool QRELabelCommon::RELabelOnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos)
{
    //TODO:
    return false;
}

QREFontObject * QRELabelCommon::GetFontQ()
{
    REFontObject * f = this->relabel()->GetFont();
    if (f)
    {
        QREFontObject * qf = dynamic_cast<QREFontObject *>(f);
        return qf;
    }
    return NULL;
}

bool QRELabelCommon::SetNewFontQ()
{
    REString t(this->relabel()->GetText());
    this->RemoveFontQ();
    bool r = false;
    QREFontObject * newFont = new QREFontObject();
    if (newFont)
    {
        newFont->SetFontHeightQ(this->relabel()->GetFrame().height);
        this->relabel()->SetFont(newFont);
        newFont->Release();
        newFont->REFontObjectInfoToProperties();
        r = true;
    }
    if (r)
    {
        this->relabel()->SetText(t);
    }
    return r;
}

bool QRELabelCommon::OpenFontFileQ()
{
    GLRenderer::PauseRendering();
    QREFontObject * font = this->GetFontQ();
    if (font == NULL)
    {
        this->SetNewFontQ();
    }
    font = this->GetFontQ();
    const bool r = QRELabelCommon::OpenFontFileQ(font);
    GLRenderer::ResumeRendering();
    return r;
}

void QRELabelCommon::RemoveFontQ()
{
    GLRenderer::PauseRendering();
    this->relabel()->SetFont(NULL);
    QREObjectCommon::RemoveAllSubProperties(_reLabelProps.font);
    GLRenderer::ResumeRendering();
}

QRELabelCommon::QRELabelCommon()
{
    memset(&_reLabelProps, 0, sizeof(QRERELabelProps));
}

QRELabelCommon::~QRELabelCommon()
{
    QREObjectCommon::RemoveAllSubProperties(_reLabelProps.item);
    QREObjectCommon::RemoveAllSubProperties(_reLabelProps.textInsets);
    QREObjectCommon::RemoveAllSubProperties(_reLabelProps.font);
}

bool QRELabelCommon::OpenFontFileQ(QREFontObject * font)
{
    GLRenderer::PauseRendering();
    bool r = false;
    if (font)
    {
        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::ExistingFile);
        dialog.setOption(QFileDialog::ReadOnly, true);
        dialog.setNameFilter("TrueType font files (*.ttf *.TTF)");
        QStringList fileNames;
        if (dialog.exec())
        {
            fileNames = dialog.selectedFiles();
            if (!fileNames.isEmpty())
            {
                QString path(fileNames.first());
                font->SetFontFilePathQ(path);
                r = true;
            }
        }
    }
    GLRenderer::ResumeRendering();
    return r;
}




void QRELabel::SetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL)
{
    QREObjectCommon::SetProperties(propertyManager, parentItemOrNULL);

    this->REViewSetProperties(propertyManager, parentItemOrNULL);

    this->RELabelSetProperties(propertyManager, parentItemOrNULL);

    QRETextureObject * texture = this->GetTextureQ();
    if (texture)
    {
        texture->SetProperties(propertyManager, _reViewProps.textureItem);
    }

    QREFontObject * font = this->GetFontQ();
    if (font)
    {
        font->SetProperties(propertyManager, _reLabelProps.font);
    }
}

bool QRELabel::OnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue)
{
    static bool isUpdating = false;
    if (isUpdating) return false;

    if (QREObjectCommon::OnValueOfPropertyChanged(property, newValue))
    {
        return true;
    }

    RERect oldFrame(this->relabel()->GetFrame());
    if (this->REViewOnValueOfPropertyChanged(property, newValue))
    {
        RERect newFrame(this->relabel()->GetFrame());
        if (oldFrame.height != newFrame.height)
        {
            isUpdating = true;
            QREFontObject * font = this->GetFontQ();
            if (font)
            {
                REString t(this->relabel()->GetText());
                font->SetFontHeightQ(newFrame.height);
                this->relabel()->SetText(t);
                font->REFontObjectInfoToProperties();
            }
            isUpdating = false;
        }
        return true;
    }

    if (this->RELabelOnValueOfPropertyChanged(property, newValue))
    {
        REString t(this->GetText());
        this->relabel()->SetText(t);
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

    QREFontObject * font = this->GetFontQ();
    if (font)
    {
        if (font->OnValueOfPropertyChanged(property, newValue))
        {
            REString t(this->GetText());
            this->relabel()->SetText(t);
            return true;
        }
    }

    return false;
}

bool QRELabel::OnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos)
{

    return false;
}

QString QRELabel::GetXMLString(QString linePrefix, QString keyPurpose)
{
    QString xml;

    xml += linePrefix + "<object class=\"" + this->GetClassNameQ() + "\"";
    if (keyPurpose.isEmpty()) xml += ">\n";
    else xml += " key=\"" + keyPurpose + "\">\n";

    xml += this->GetREObjectInfoXMLString((linePrefix + "\t"));
    xml += this->GetREViewInfoXMLString((linePrefix + "\t"));
    xml += this->GetRELabelInfoXMLString((linePrefix + "\t"));

    QRETextureObject * texture = this->GetTextureQ();
    if (texture)
    {
        if (!texture->IsQRETextureObjectNull())
        {
            xml += texture->GetXMLString((linePrefix + "\t"), QString(RELabel::GetXMLTextureObjectKeyString()));
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

void QRELabel::Render()
{
    RELabel::Render();
    if(this->IsHighLighted())this->RenderHighLight();
}

void QRELabel::RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
    RELabel::RenderWithOffset(offsetX,offsetY);
    if(this->IsHighLighted())this->RenderHighLight();
}

void QRELabel::SaveQ(SaveVCInfo * info)
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

QRELabel::QRELabel() :
    RELabel(),
    QREObjectCommon(),
    QREViewCommon(),
    QRELabelCommon()
{

}

QRELabel::~QRELabel()
{

}

