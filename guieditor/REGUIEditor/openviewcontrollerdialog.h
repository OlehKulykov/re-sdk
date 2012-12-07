#ifndef OPENVIEWCONTROLLERDIALOG_H
#define OPENVIEWCONTROLLERDIALOG_H

#include <QDialog>
#include "qstringpath.h"

namespace Ui {
class OpenViewControllerDialog;
}

class OpenViewControllerDialog : public QDialog
{
    Q_OBJECT
    
public:
    void SetDataFullPath(const QString & path);
    explicit OpenViewControllerDialog(QWidget *parent = 0);
    ~OpenViewControllerDialog();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::OpenViewControllerDialog *ui;
};

#endif // OPENVIEWCONTROLLERDIALOG_H
