#include "xmloutputdialog.h"
#include "ui_xmloutputdialog.h"
#include "mainwindow.h"

void XMLOutPutDialog::SetXMLString(QString xmlString)
{
    ui->textBrowser->setText(xmlString);
}

XMLOutPutDialog::XMLOutPutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::XMLOutPutDialog)
{
    ui->setupUi(this);

    this->setStyleSheet(MainWindow::WidgetStyleSheetFontWithSize(12));

    /*
    QFont font12("Monaco", 12);
    ui->textBrowser->setFont(font12);
    ui->pushCloseButton->setFont(font12);
    */
}

XMLOutPutDialog::~XMLOutPutDialog()
{
    delete ui;
}

void XMLOutPutDialog::on_pushCloseButton_clicked()
{
    this->close();
}
