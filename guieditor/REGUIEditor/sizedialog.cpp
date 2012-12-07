#include "sizedialog.h"
#include "ui_sizedialog.h"
#include "mainwindow.h"
#include "renderdialog.h"
#include "mainwindow.h"


void SizeDialog::SetType(SizeDialogType newType)
{
    _type = newType;

    ui->widthSpinBox->setVisible(_type == SizeDialogPX);
    ui->heightSpinBox->setVisible(_type == SizeDialogPX);

    ui->widthSpinBoxFL->setVisible(_type == SizeDialogFL);
    ui->heightSpinBoxFL->setVisible(_type == SizeDialogFL);

    if (_type == SizeDialogPX)
    {
        this->setWindowTitle(tr("New renderer size in pixels"));

        ui->widthSpinBox->setValue(_widthPX);
        ui->heightSpinBox->setValue(_heightPX);
    }
    else
    {
        this->setWindowTitle(tr("New renderer size in floats"));

        ui->widthSpinBoxFL->setValue(_widthFL);
        ui->heightSpinBoxFL->setValue(_heightFL);
    }
}

SizeDialog::SizeDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::SizeDialog),
        _widthPX(0),
        _heightPX(0),
        _widthFL(0.0f),
        _heightFL(0.0f),
        _type(SizeDialogPX),
        _isCancelled(false)

{
    ui->setupUi(this);

    this->setStyleSheet(MainWindow::WidgetStyleSheetFontWithSize(12));

    ui->heightSpinBoxFL->hide();
    ui->widthSpinBoxFL->hide();
}

SizeDialog::~SizeDialog()
{
    delete ui;
}

void SizeDialog::SetWidthPX(int width)
{
    _widthPX = width;
    ui->widthSpinBox->setValue(_widthPX);
}

void SizeDialog::SetHeightPX(int height)
{
    _heightPX = height;
    ui->heightSpinBox->setValue(_heightPX);
}

void SizeDialog::SetWidthFL(REFloat32 width)
{
    _widthFL = width;
    ui->widthSpinBoxFL->setValue(_widthFL);
}

void SizeDialog::SetHeightFL(REFloat32 height)
{
    _heightFL = height;
    ui->heightSpinBoxFL->setValue(_heightFL);
}

void SizeDialog::on_pushButton_2_clicked()
{
    _isCancelled = true;
    this->close();
}

void SizeDialog::on_pushButton_clicked()
{
    // Ok button
    _isCancelled = false;

    if (_type == SizeDialogPX)
    {
        //RenderDialog::GetDefaultDialog()->SetRendererSize(_widthPX, _heightPX);
    }
    else if (_type == SizeDialogFL)
    {
        //Renderer::GetDefaultRender()->SetRendererSizeGL(_widthFL, _heightFL);
    }

    this->close();
}

void SizeDialog::showEvent(QShowEvent * event)
{
    QDialog::showEvent(event);
    _isCancelled = false;
}

void SizeDialog::on_widthSpinBox_valueChanged(int newWidth)
{
    _widthPX = newWidth;
}

void SizeDialog::on_heightSpinBox_valueChanged(int newHeight)
{
    _heightPX = newHeight;
}

void SizeDialog::on_widthSpinBoxFL_valueChanged(double newWidth)
{
    _widthFL = newWidth;
}

void SizeDialog::on_heightSpinBoxFL_valueChanged(double newHeight)
{
    _heightFL = newHeight;
}

void SizeDialog::on_exchangeToolButton_clicked()
{
    int newW_px = _heightPX;
    int newH_px = _widthPX;

    ui->widthSpinBox->setValue(newW_px);
    ui->heightSpinBox->setValue(newH_px);

    _widthPX = newW_px;
    _heightPX = newH_px;



    REFloat32 newW_fl = _heightFL;
    REFloat32 newH_fl = _widthFL;

    ui->widthSpinBoxFL->setValue(newW_fl);
    ui->heightSpinBoxFL->setValue(newH_fl);

    _widthFL = newW_fl;
    _heightFL = newH_fl;
}

