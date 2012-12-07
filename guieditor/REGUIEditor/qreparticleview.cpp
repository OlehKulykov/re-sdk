
#include "qreparticleview.h"
#include <QFileDialog>

void QREParticleViewCommon::CreateREParticleViewProperties(QREREParticleViewProps * props,
                                                           QtVariantPropertyManager * manager)
{
    props->item = manager->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String("REParticleView"));

    props->path = manager->addProperty(QVariant::String, QLatin1String("File Path"));
    props->item->addSubProperty(props->path);

    props->screenCenter = manager->addProperty(QVariant::Point, QLatin1String("Screen Center"));
    props->item->addSubProperty(props->screenCenter);

    props->center = manager->addProperty(QVariant::PointF, QLatin1String("Center"));
    props->item->addSubProperty(props->center);
}

void QREParticleViewCommon::REParticleViewInfoToProperties()
{
    if (_reParticleViewProps.path)
    {
        _reParticleViewProps.path->setValue(_particleFilePathQ);
    }
    if (_reParticleViewProps.center)
    {
        REPoint2 c(this->reparticleview()->GetCenter());
        QPointF p(REQTConverter::GetQPointF(c));
        _reParticleViewProps.center->setValue(QVariant(p));
    }
}

void QREParticleViewCommon::REParticleViewSetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL)
{
    if (TreePropertyManager::IsNotNULL(propertyManager))
    {
        if (_reParticleViewProps.item == NULL)
        {
            QREParticleView::CreateREParticleViewProperties(&_reParticleViewProps, propertyManager->GetVariantManager());
        }

        this->REParticleViewInfoToProperties();

        if (_reParticleViewProps.item)
        {
            if (parentItemOrNULL) { parentItemOrNULL->addSubProperty(_reParticleViewProps.item); }
            else { propertyManager->GetPropertyBrowser()->addProperty(_reParticleViewProps.item); }
        }
    }
}

bool QREParticleViewCommon::REParticleViewOnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue)
{
    static bool isCanUpdate = true;
    if (isCanUpdate && property && newValue.isValid())
    {
        if (property == _reParticleViewProps.path)
        {
            QString qp(newValue.toString());
            return this->SetParticlePathQ(qp);
        }
        else if (property == _reParticleViewProps.center)
        {
            QPointF q(newValue.toPointF());
            REPoint2 r(REQTConverter::GetREPoint(q));
            this->reparticleview()->SetCenter(r);
            isCanUpdate = false;
            this->REParticleViewInfoToProperties();
            isCanUpdate = true;
            return true;
        }
        else if (property == _reParticleViewProps.screenCenter)
        {
            QPoint q(newValue.toPoint());
            REPoint2 c(QREView::ConvertScreenPointIInPixelsToRenderPoint(q));
            this->reparticleview()->SetCenter(c);
            isCanUpdate = false;
            this->REParticleViewInfoToProperties();
            isCanUpdate = true;
            return true;
        }
    }
    return false;
}

bool QREParticleViewCommon::REParticleViewOnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos)
{
    return false;
}

bool QREParticleViewCommon::SetParticlePathQ(const QString & path)
{
    _particleFileBuffer.Clear();
    _particleFilePathQ.clear();
    REBuffer buff;
    if (REQTConverter::QFileReadToBuffer(path, &buff))
    {
        if (this->reparticleview()->InitFromFileBuffer(buff))
        {
            _particleFileBuffer = buff;
            _particleFilePathQ = path;
            return true;
        }
    }
    return false;
}

bool QREParticleViewCommon::OpenParticleFileQ()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setOption(QFileDialog::ReadOnly, true);
    dialog.setNameFilter("Particle Files (*.pyro *.prt)");
    QStringList fileNames;
    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();
        if (!fileNames.isEmpty())
        {
            QString p(fileNames.first());
            return this->SetParticlePathQ(p);
        }
    }
    return false;
}

QString QREParticleViewCommon::GetREParticleViewSubViewsInfoXMLString(QString linePrefix)
{
    QString xml;

    xml += linePrefix + "<string key=\""+ REParticleView::GetXMLPathKeyString() +"\">" + _particleFilePathQ + "</string>\n";

    REPoint2 c(this->reparticleview()->GetCenter());
    xml += linePrefix + "<string key=\"" + REParticleView::GetXMLCenterKeyString() + "\">" +
            QString().sprintf(REParticleView::GetXMLCenterFormatString(), (float)c.x, (float)c.y) + "</string>\n";

    return xml;
}

void QREParticleViewCommon::SaveREParticleViewWithInfo(SaveVCInfo * info)
{

}

QREParticleViewCommon::QREParticleViewCommon()
{
    memset(&_reParticleViewProps, 0, sizeof(QREREParticleViewProps));
}

QREParticleViewCommon::~QREParticleViewCommon()
{
    QREObjectCommon::RemoveAllSubProperties(_reParticleViewProps.item);
}






void QREParticleView::SetProperties(TreePropertyManager * propertyManager, QtProperty * parentItemOrNULL)
{
    QREObjectCommon::SetProperties(propertyManager, parentItemOrNULL);
    this->REViewSetProperties(propertyManager, parentItemOrNULL);
    if (_reViewProps.color)
    {
        _reViewProps.color->setEnabled(false);
    }
    if (_reViewProps.textureItem)
    {
        _reViewProps.textureItem->setEnabled(false);
    }
    this->REParticleViewSetProperties(propertyManager, parentItemOrNULL);
}

bool QREParticleView::OnValueOfPropertyChanged(QtProperty * property, const QVariant & newValue)
{
    if (QREObjectCommon::OnValueOfPropertyChanged(property, newValue))
    {
        return true;
    }
    if (this->REViewOnValueOfPropertyChanged(property, newValue))
    {
        this->REParticleViewInfoToProperties();
        return true;
    }

    return this->REParticleViewOnValueOfPropertyChanged(property, newValue);
}

bool QREParticleView::OnPropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos)
{
    return false;
}

QString QREParticleView::GetXMLString(QString linePrefix, QString keyPurpose)
{
    QString xml;

    xml += linePrefix + "<object class=\"" + this->GetClassNameQ() + "\"";
    if (keyPurpose.isEmpty()) xml += ">\n";
    else xml += " key=\"" + keyPurpose + "\">\n";

    xml += this->GetREObjectInfoXMLString((linePrefix + "\t"));
    xml += this->GetREViewInfoXMLString((linePrefix + "\t"));
    xml += this->GetREParticleViewSubViewsInfoXMLString((linePrefix + "\t"));


    xml += this->GetREViewSubViewsInfoXMLString((linePrefix + "\t"));

    xml += linePrefix + "</object>\n";

    return xml;
}

void QREParticleView::Render()
{
    REParticleView::Render();
    if (this->IsHighLighted()) this->RenderHighLight();
}

void QREParticleView::RenderWithOffset(const REFloat32 offsetX, const REFloat32 offsetY)
{
    REParticleView::RenderWithOffset(offsetX, offsetY);
    if (this->IsHighLighted()) this->RenderHighLight();
}

void QREParticleView::SaveQ(SaveVCInfo * info)
{
    this->SaveREParticleViewWithInfo(info);
    this->SaveREViewSubViewsWithInfo(info);
}

QREParticleView::QREParticleView() :
    REParticleView(),
    QREObjectCommon(),
    QREViewCommon(),
    QREParticleViewCommon()
{

}

QREParticleView::~QREParticleView()
{

}

