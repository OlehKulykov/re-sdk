
#include "openviewcontrollerdialog.h"
#include "ui_openviewcontrollerdialog.h"
#include <QDir>
#include <QFile>
#include <QDebug>
#include "mainwindow.h"

void OpenViewControllerDialog::SetDataFullPath(const QString & path)
{
    ui->listWidget->clear();

    QStringPath p(path);
    p.appendPath("vc");
    QDir dir(p);
    if (dir.exists())
    {
        QStringList list = dir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
        for (int i = 0; i < list.count(); i++)
        {
            QStringPath vcPath(p);
            QString vcName(list.at(i));
            vcPath.appendPath(vcName);
            vcPath.appendPath("vc.xml");
            QFile file(vcPath);
            if (file.exists())
            {
                QListWidgetItem * newItem = new QListWidgetItem();
                if (newItem)
                {
                    newItem->setText(vcName);
                    newItem->setData(1, QVariant((QString)vcPath));
                    ui->listWidget->addItem(newItem);
                }
            }
        }
    }
}

OpenViewControllerDialog::OpenViewControllerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenViewControllerDialog)
{
    ui->setupUi(this);

    /*
    QFont font12("Monaco", 12);
    ui->listWidget->setFont(font12);
    ui->pushButton->setFont(font12);
    ui->pushButtonCancel->setFont(font12);
    */

    this->setStyleSheet(MainWindow::WidgetStyleSheetFontWithSize(12));
}

OpenViewControllerDialog::~OpenViewControllerDialog()
{
    delete ui;
}

void OpenViewControllerDialog::on_pushButton_clicked()
{
    QList<QListWidgetItem *> items = ui->listWidget->selectedItems();
    if (!items.isEmpty())
    {
        QListWidgetItem * item = items.first();
        MainWindow::OpenViewController(item->text(), item->data(1).toString());
        this->close();
    }
}

void OpenViewControllerDialog::on_pushButtonCancel_clicked()
{
    this->close();
}

