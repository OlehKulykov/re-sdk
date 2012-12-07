#ifndef QREVCCPPHELPER_H
#define QREVCCPPHELPER_H

#include <QDialog>
#include "qreviewcontroller.h"
#include <QStringList>

namespace Ui
{
class QREVCCPPHelper;
}

class QREVCCPPHelper : public QDialog
{
    Q_OBJECT

private:
    static QString WriteObject(QREObjectCommon * obj, QStringList * writedClassNames);
    static QString HelperString(QREViewController * vc);
public:
    void SetViewController(QREViewController * vc);
    explicit QREVCCPPHelper(QWidget *parent = 0);
    ~QREVCCPPHelper();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::QREVCCPPHelper *ui;
};

#endif // QREVCCPPHELPER_H
