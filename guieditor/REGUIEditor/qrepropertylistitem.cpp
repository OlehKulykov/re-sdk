
#include "qrepropertylistitem.h"
#include "ui_qrepropertylistitem.h"
#include "objectpropsdialog.h"

void QREPropertyListItem::SetListWidgetItem(QListWidgetItem * item)
{
    _listWidgetItem = item;
}

void QREPropertyListItem::SetObjectThatContainesProperty(QREObjectCommon * o)
{
    _objectThatContainesProperty = o;
}

void QREPropertyListItem::SetProperty(QREProperty * p)
{
    _property = p;

    ui->propNameLabel->clear();
    ui->radioButton->setChecked(false);
    ui->radioButton->dragText.clear();
    ui->radioButton->property = p;

    if (_property)
    {
        ui->propNameLabel->setText(_property->GetKeyStringQ());
        ui->radioButton->dragText = _property->GetKeyStringQ();

        if (_property->GetObject())
        {
            ui->radioButton->setChecked(true);
        }
    }
}

QREPropertyListItem::QREPropertyListItem(QWidget * parent) :
    QWidget(parent),
    ui(new Ui::QREPropertyListItem)
{
    _property = NULL;
    _listWidgetItem = NULL;
    _objectThatContainesProperty = NULL;

    ui->setupUi(this);

    QFont font12("Monaco", 12);
    ui->deletePropButton->setFont(font12);
    ui->propNameLabel->setFont(font12);
    ui->radioButton->setFont(font12);
}

QREPropertyListItem::~QREPropertyListItem()
{
    delete ui;
}


void QREPropertyListItem::on_deletePropButton_clicked()
{
    if (this->parent())
    {
        ObjectPropsDialog * dialog = (ObjectPropsDialog *)this->parent();
        if (dialog)
        {
            dialog = dynamic_cast<ObjectPropsDialog *>(dialog);
            if (dialog)
            {
                dialog->DeleteItem(this);
            }
        }
    }
}
