#include "viewcontrollercreatecustomxmlserializableobject.h"
#include "ui_viewcontrollercreatecustomxmlserializableobject.h"
#include "mainwindow.h"

void ViewControllerCreateCustomXMLSerializableObject::TrySelectTypeByClassName(const QString & name)
{
    if (name.contains(QString("view"), Qt::CaseInsensitive)) ui->BaseObjectComboBox->setCurrentIndex(2);
    if (name.contains(QString("button"), Qt::CaseInsensitive)) ui->BaseObjectComboBox->setCurrentIndex(3);
    if (name.contains(QString("viewcontroller"), Qt::CaseInsensitive)) ui->BaseObjectComboBox->setCurrentIndex(1);
    if (name.contains(QString("label"), Qt::CaseInsensitive)) ui->BaseObjectComboBox->setCurrentIndex(4);
    if (name.contains(QString("textfield"), Qt::CaseInsensitive)) ui->BaseObjectComboBox->setCurrentIndex(5);
    if (name.contains(QString("particle"), Qt::CaseInsensitive)) ui->BaseObjectComboBox->setCurrentIndex(6);
    if (name.contains(QString("texture"), Qt::CaseInsensitive)) ui->BaseObjectComboBox->setCurrentIndex(7);
    if (name.contains(QString("font"), Qt::CaseInsensitive)) ui->BaseObjectComboBox->setCurrentIndex(8);
}

void ViewControllerCreateCustomXMLSerializableObject::SetCustomObjectClassName(const char * className)
{
    _customObjectClassName = className;
    ui->customClassNameLabel->setText(_customObjectClassName);
    this->TrySelectTypeByClassName(_customObjectClassName);
}

ViewControllerCreateCustomXMLSerializableObject::ViewControllerCreateCustomXMLSerializableObject(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewControllerCreateCustomXMLSerializableObject)
{
    ui->setupUi(this);

    /*
    QFont font12("Monaco", 12);
    ui->label->setFont(font12);
    ui->label_2->setFont(font12);
    ui->label_3->setFont(font12);
    ui->customClassNameLabel->setFont(font12);
    ui->BaseObjectComboBox->setFont(font12);
    ui->pushButton->setFont(font12);
    ui->pushButton_2->setFont(font12);
*/
    this->setStyleSheet(MainWindow::WidgetStyleSheetFontWithSize(12));

    selectedType = QREGUIObjectType_NONE;

    ui->BaseObjectComboBox->addItem("Undefined");           // 0
    ui->BaseObjectComboBox->addItem("REViewController");    // 1
    ui->BaseObjectComboBox->addItem("REView");              // 2
    ui->BaseObjectComboBox->addItem("REButton");            // 3
    ui->BaseObjectComboBox->addItem("RELabel");             // 4
    ui->BaseObjectComboBox->addItem("RETextField");         // 5
    ui->BaseObjectComboBox->addItem("REParticleView");      // 6
    ui->BaseObjectComboBox->addItem("RETextureObject");     // 7
    ui->BaseObjectComboBox->addItem("REFontObject");        // 8
}

ViewControllerCreateCustomXMLSerializableObject::~ViewControllerCreateCustomXMLSerializableObject()
{
    delete ui;
}

void ViewControllerCreateCustomXMLSerializableObject::on_pushButton_clicked()
{
    switch (ui->BaseObjectComboBox->currentIndex())
    {
    case 0: selectedType = QREGUIObjectType_NONE; break;
    case 1: selectedType = QREGUIObjectType_QREVIEWCONTROLLER; break;
    case 2: selectedType = QREGUIObjectType_QREVIEW; break;
    case 3: selectedType = QREGUIObjectType_QREBUTTON; break;
    case 4: selectedType = QREGUIObjectType_QRELABEL; break;
    case 5: selectedType = QREGUIObjectType_QRETEXTFIELD; break;
    case 6: selectedType = QREGUIObjectType_QREPARTICLEVIEW; break;
    case 7: selectedType = QREGUIObjectType_QRETEXTUREOBJECT; break;
    case 8: selectedType = QREGUIObjectType_QREFONTOBJECT; break;
    default:selectedType = QREGUIObjectType_NONE; break;
    };

    this->close();
}

void ViewControllerCreateCustomXMLSerializableObject::on_pushButton_2_clicked()
{
    this->close();
}
