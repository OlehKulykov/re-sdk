
#ifndef VIEWCONTROLLERCREATECUSTOMXMLSERIALIZABLEOBJECT_H
#define VIEWCONTROLLERCREATECUSTOMXMLSERIALIZABLEOBJECT_H

#include <QDialog>
#include "qreguiobjectshelper.h"

namespace Ui {
class ViewControllerCreateCustomXMLSerializableObject;
}

class ViewControllerCreateCustomXMLSerializableObject : public QDialog
{
    Q_OBJECT
private:
    QString _customObjectClassName;
    void TrySelectTypeByClassName(const QString & name);
public:
    explicit ViewControllerCreateCustomXMLSerializableObject(QWidget *parent = 0);
    ~ViewControllerCreateCustomXMLSerializableObject();
    
    QREGUIObjectType selectedType;

    void SetCustomObjectClassName(const char * className);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ViewControllerCreateCustomXMLSerializableObject *ui;
};

#endif // VIEWCONTROLLERCREATECUSTOMXMLSERIALIZABLEOBJECT_H
