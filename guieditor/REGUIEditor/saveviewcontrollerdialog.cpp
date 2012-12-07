#include "saveviewcontrollerdialog.h"
#include "ui_saveviewcontrollerdialog.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include "mainwindow.h"


void SaveViewControllerDialog::SetViewController(QREViewController * vc)
{
    _isIgnoreOptSizeChanging = true;

    _viewController = vc;
    if (_viewController)
    {
        ui->nameLineEdit->setText(_viewController->GetNameQ());

        QSize s(_viewController->GetSizePxQ());
        _info.SetCurrentVCScreenSize(QSizeF((qreal)s.width(), (qreal)s.height()));
        _info.SetOptimizedVCScreenSize(QSizeF((qreal)s.width(), (qreal)s.height()));

        ui->curWidthSpinBox->setValue(s.width());
        ui->optWidthSpinBox->setValue(s.width());

        ui->curHeightSpinBox->setValue(s.height());
        ui->optHeightSpinBox->setValue(s.height());

    }
    _isIgnoreOptSizeChanging = false;
}

void SaveViewControllerDialog::SetStoreDataPath(const QString & p)
{
    QDir dir(p);
    if (dir.exists())
    {
        _info.SetStoreDataFullPath(p);
        ui->storePathLineEdit->setText(p);
    }
}


SaveViewControllerDialog::SaveViewControllerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveViewControllerDialog)
{
    _viewController = NULL;
    _isIgnoreOptSizeChanging = false;
    ui->setupUi(this);

    this->setStyleSheet(MainWindow::WidgetStyleSheetFontWithSize(12));
}

SaveViewControllerDialog::~SaveViewControllerDialog()
{
    delete ui;
}

void SaveViewControllerDialog::on_savePushButton_clicked()
{
    if (_viewController)
    {
        _viewController->SaveQ(&_info);
    }

    QString s = tr("View controller [");
    s += _info.GetVCName();
    s += tr("] saved.");

    QMessageBox::information(this,
                          tr("REGUI editor"),
                          s,
                          QMessageBox::Ok);
}

void SaveViewControllerDialog::on_cancelPushButton_clicked()
{
    this->close();
}

void SaveViewControllerDialog::on_optWidthSpinBox_valueChanged(int arg1)
{
    if (!_isIgnoreOptSizeChanging)
    {
        QSizeF resSize = _info.GetOptimizedVCScreenSize();
        resSize.setWidth((qreal)arg1);
        if (ui->contSizeProportCheckBox->isChecked())
        {
            QSizeF s(_info.GetCurrentVCScreenSize());
            const long double ratio = (s.width() > 0) ? ((long double)s.height() / (long double)s.width()) : 0;
            const long double h = ratio * (long double)arg1;
            resSize.setHeight((qreal)h);
            _isIgnoreOptSizeChanging = true;
            ui->optHeightSpinBox->setValue((int)h);
            _isIgnoreOptSizeChanging = false;
        }
        _info.SetOptimizedVCScreenSize(resSize);
    }
}

void SaveViewControllerDialog::on_optHeightSpinBox_valueChanged(int arg1)
{
    if (!_isIgnoreOptSizeChanging)
    {
        QSizeF resSize = _info.GetOptimizedVCScreenSize();
        resSize.setHeight((qreal)arg1);
        if (ui->contSizeProportCheckBox->isChecked())
        {
            QSizeF s(_info.GetCurrentVCScreenSize());
            const long double ratio = (s.height() > 0) ? ((long double)s.width() / (long double)s.height()) : 0;
            const long double w = ratio * (long double)arg1;
            resSize.setWidth((qreal)w);
            _isIgnoreOptSizeChanging = true;
            ui->optWidthSpinBox->setValue((int)w);
            _isIgnoreOptSizeChanging = false;
        }
        _info.SetOptimizedVCScreenSize(resSize);
    }
}

void SaveViewControllerDialog::on_optForScrSizeCheckBox_stateChanged(int arg1)
{
    const bool isCh = ui->optForScrSizeCheckBox->isChecked();
    ui->optimForScrSizeGroupBox->setEnabled(isCh);
    _info.SetOptimizeForScrSize(isCh);
}

void SaveViewControllerDialog::on_scaleTexturesCheckBox_stateChanged(int arg1)
{
    _info.SetScaleTextureImageForScreenSize(ui->scaleTexturesCheckBox->isChecked());
}

void SaveViewControllerDialog::on_changeTextureImgFormatCheckBox_stateChanged(int arg1)
{
    _info.SetChangeTextureImageFormatForSmallestFileSize(ui->changeTextureImgFormatCheckBox->isChecked());
}

void SaveViewControllerDialog::on_nameLineEdit_textChanged(const QString &arg1)
{
    _info.SetVCName(arg1);
    if (_viewController)
    {
        _viewController->SetNameQ(arg1);
    }
}

void SaveViewControllerDialog::on_openStorePathToolButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly, true);
    dialog.setNameFilter("\"data\" directory (data)");
    QStringList fileNames;
    if ( dialog.exec() )
    {
        fileNames = dialog.selectedFiles();
        if ( !fileNames.isEmpty() )
        {
            QStringPath path = fileNames.first();
            QString last = path.lastPathComponent();
            if (last.compare("data", Qt::CaseSensitive) == 0)
            {
                this->SetStoreDataPath(path);
            }
            else
            {
                QMessageBox::critical(this,
                                      tr("REGUI editor"),
                                      tr("You are select not \"data\" directory.\nPlease select or create \"data\" directory."),
                                      QMessageBox::Ok);
            }
        }
    }
}
