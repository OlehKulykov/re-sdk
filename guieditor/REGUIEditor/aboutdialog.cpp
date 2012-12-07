#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "mainwindow.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    QString s;
    s += "Editor created using Qt: ";
    s += qVersion();
    s += "\nhttp://qt.nokia.com\n";

#ifdef QT_VERSION
    s += "QT_VERSION: ";
    s += QString::number(QT_VERSION);
    s += "\n";
#endif

#ifdef QT_VERSION_STR
    s += "QT_VERSION_STR: ";
    s += QT_VERSION_STR;
    s += "\n";
#endif

    s += "\nBuildInfo: \n";
    s += RECore::BuildInfo();

    this->setStyleSheet(MainWindow::WidgetStyleSheetFontWithSize(12));

    /*
    QFont font14("Monaco", 14);
    ui->textEdit->setFont(font14);
    ui->pushButton->setFont(font14);
    */

    ui->textEdit->setText(s);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_pushButton_clicked()
{
    this->close();
}
