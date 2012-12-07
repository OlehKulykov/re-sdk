#ifndef OBJECTPROPSDIALOG_H
#define OBJECTPROPSDIALOG_H

#include <QDialog>
#include "qreobject.h"
#include <QListWidgetItem>
#include "qrepropertylistitem.h"
#include "qrepropertylistwidgetitem.h"
#include <QStringList>

namespace Ui
{
class ObjectPropsDialog;
}

class ObjectPropsDialog : public QDialog
{
    Q_OBJECT
protected:
    QREObjectCommon * _object;
    QREPropertyListWidgetItem * _higthlightedItem;

    void AddProperty(QREProperty * prop);
    QString VariableErrorString(const QString & var);
    static QStringList * GetReservedWords();
    static QList<QChar> * GetUnsupportedChars();
public:
    void SetObject(QREObjectCommon * obj);
    void DeleteItem(QREPropertyListItem * item);


    explicit ObjectPropsDialog(QWidget *parent = 0);
    ~ObjectPropsDialog();
    
private slots:
    void on_listWidget_itemEntered(QListWidgetItem *item);
    void on_pushAddPropertyButton_clicked();
    void on_lineEdit_textChanged(const QString &arg1);
    void on_listWidget_itemSelectionChanged();

private:
    Ui::ObjectPropsDialog *ui;
};

#endif // OBJECTPROPSDIALOG_H
