
#include "renderdialog.h"
#include "ui_renderdialog.h"
#include <QTimer>
#include "mainwindow.h"

void RenderDialog::UpdateRenderer()
{
    GLRenderer * r = this->GetRenderer();
    if (r)
    {
        r->updateGL();
    }

    int fps = 60;
    if (this->isHidden())
    {
        fps = 30;
    }

    const int milisec = 1000 / fps;

    QTimer::singleShot(milisec, this, SLOT(UpdateRenderer()));
}

void RenderDialog::UpdateGeometry()
{
    QSize size(MainWindow::GetRootViewControllerSizePX());
    const int w = (int)(((long double)size.width()) * _scale);
    const int h = (int)(((long double)size.height()) * _scale);
    if ( (w <= 0) || (h <= 0) ) return;

    QRect f(this->geometry());
    f.setWidth(w + 20);
    f.setHeight(h + 35);
    this->setMinimumSize(f.size());
    this->setMaximumSize(f.size());
    this->setGeometry(f);

    ui->infoLabel->setGeometry(QRect(10, 5, w, 16));

    GLRenderer * renderer = this->GetRenderer();
    f.setX(10);
    f.setY(25);
    f.setWidth(w);
    f.setHeight(h);

    renderer->setMinimumSize(f.size());
    renderer->setMaximumSize(f.size());
    renderer->setGeometry(f);

    renderer->resize(w, h);

    QString s("Render:[");
    s += QString::number((double)RERenderDevice::GetDefaultDevice()->GetRenderSize().width);
    s += "x";
    s += QString::number((double)RERenderDevice::GetDefaultDevice()->GetRenderSize().height);
    s += "]  View Controller:[";
    s += QString::number((int)size.width());
    s += "x";
    s += QString::number((int)size.height());
    s += "]  Window:[";
    s += QString::number((int)w);
    s += "x";
    s += QString::number((int)h);
    s += "]";
    ui->infoLabel->setText(s);
}

void RenderDialog::OnRenderDeviceScreenSizeChanged()
{
    this->UpdateGeometry();
}

void RenderDialog::SetScaleValue(REFloat32 v)
{
    _scale = v;
    this->GetRenderer()->SetScaleValue(_scale);
    this->UpdateGeometry();
}

GLRenderer * RenderDialog::GetRenderer()
{
    return ui->widget;
}

RenderDialog::RenderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenderDialog)
{
    ui->setupUi(this);

    //ui->infoLabel->setFont(QFont("Monaco", 11));

    this->setStyleSheet(MainWindow::WidgetStyleSheetFontWithSize(12));

    this->UpdateRenderer();
    _scale = 1.0f;
    this->UpdateGeometry();
}

RenderDialog::~RenderDialog()
{
    delete ui;
}

