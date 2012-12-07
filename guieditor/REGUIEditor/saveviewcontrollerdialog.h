#ifndef SAVEVIEWCONTROLLERDIALOG_H
#define SAVEVIEWCONTROLLERDIALOG_H

#include <QDialog>
#include "savevcinfo.h"
#include "qreviewcontroller.h"

namespace Ui {
class SaveViewControllerDialog;
}

class SaveViewControllerDialog : public QDialog
{
    Q_OBJECT
protected:
    SaveVCInfo _info;
    QREViewController * _viewController;
    bool _isIgnoreOptSizeChanging;
public:
    void SetViewController(QREViewController * vc);
    void SetStoreDataPath(const QString & p);
    SaveVCInfo * GetSaveInfo() { return (&_info); }


    explicit SaveViewControllerDialog(QWidget *parent = 0);
    ~SaveViewControllerDialog();
private slots:
    void on_savePushButton_clicked();
    void on_cancelPushButton_clicked();
    void on_optWidthSpinBox_valueChanged(int arg1);
    void on_optHeightSpinBox_valueChanged(int arg1);
    void on_optForScrSizeCheckBox_stateChanged(int arg1);
    void on_scaleTexturesCheckBox_stateChanged(int arg1);
    void on_changeTextureImgFormatCheckBox_stateChanged(int arg1);
    void on_nameLineEdit_textChanged(const QString &arg1);
    void on_openStorePathToolButton_clicked();

private:
    Ui::SaveViewControllerDialog *ui;
};

#endif // SAVEVIEWCONTROLLERDIALOG_H
