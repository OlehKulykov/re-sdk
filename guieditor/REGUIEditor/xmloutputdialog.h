#ifndef XMLOUTPUTDIALOG_H
#define XMLOUTPUTDIALOG_H

#include <QDialog>

namespace Ui {
class XMLOutPutDialog;
}

class XMLOutPutDialog : public QDialog
{
    Q_OBJECT
    
public:
    void SetXMLString(QString xmlString);

    explicit XMLOutPutDialog(QWidget *parent = 0);
    ~XMLOutPutDialog();

    
private slots:
    void on_pushCloseButton_clicked();

private:
    Ui::XMLOutPutDialog *ui;
};

#endif // XMLOUTPUTDIALOG_H
