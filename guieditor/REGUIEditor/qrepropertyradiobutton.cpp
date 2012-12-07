
#include "qrepropertyradiobutton.h"
#include "qrepropertymimedata.h"
#include "qreproperty.h"
#include "mainwindow.h"

void QREPropertyRadioButton::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        QREPropertyMimeData * data = new QREPropertyMimeData();
        QDrag * drag = new QDrag(this);
        if (data && drag)
        {
            data->SetRadioButton(this);
            data->SetProperty(this->property);
            data->setText(this->dragText);
            data->SetSenderObject(MainWindow::GetSelectedGUIObject());
            //data->setData(QString("text/plain"), ba);
            drag->setMimeData(data);
            //drag->start();
            drag->exec(Qt::CopyAction | Qt::MoveAction);
        }
    }
}

QREPropertyRadioButton::QREPropertyRadioButton(QWidget *parent) :
    QRadioButton(parent)
{
    property = NULL;
}


QREPropertyRadioButton::~QREPropertyRadioButton()
{

}
