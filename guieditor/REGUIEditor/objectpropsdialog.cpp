
#include "objectpropsdialog.h"
#include "ui_objectpropsdialog.h"
#include <QMimeData>
#include <QMouseEvent>
#include <QDrag>
#include "mainwindow.h"

void ObjectPropsDialog::DeleteItem(QREPropertyListItem * item)
{
    if (item)
    {
        QListWidgetItem * listItem = item->GetListWidgetItem();
        QREObjectCommon * obj = item->GetObjectThatContainesProperty();
        QREProperty * prop = item->GetProperty();
        if (listItem && obj && prop)
        {
            if (obj->DeleteProperty(prop))
            {
                delete listItem;
            }
        }
    }
}

void ObjectPropsDialog::AddProperty(QREProperty * prop)
{
    if (prop == NULL) return;

    if (!prop->GetKeyStringQ().isEmpty())
    {
        QREPropertyListWidgetItem * newItem = new QREPropertyListWidgetItem(ui->listWidget);
        QREPropertyListItem * newPropItem = new QREPropertyListItem(this);
        if (newItem && newPropItem)
        {
            newItem->setSizeHint(QSize(0, 24));
            ui->listWidget->addItem(newItem);
            ui->listWidget->setItemWidget(newItem, newPropItem);
            newPropItem->SetListWidgetItem(newItem);
            newPropItem->SetProperty(prop);
            newPropItem->SetObjectThatContainesProperty(_object);
            newItem->SetProperty(prop);
        }
        else
        {
            RE_SAFE_DELETE(newItem);
            RE_SAFE_DELETE(newPropItem);
        }
    }
}

void ObjectPropsDialog::SetObject(QREObjectCommon * obj)
{
    _object = obj;

    if (_higthlightedItem)
    {
        _higthlightedItem->HighLight(false);
        _higthlightedItem = NULL;
    }

    ui->listWidget->clear();
    ui->lineEdit->clear();
    ui->errorLabel->clear();

    if (_object)
    {
        QList<QREProperty*> allProps(_object->GetProps());
        if (!allProps.isEmpty())
        {
            for (int i = 0; i < allProps.count(); i++)
            {
                QREProperty * prop = allProps.at(i);
                this->AddProperty(prop);
            }
        }
    }
}

ObjectPropsDialog::ObjectPropsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ObjectPropsDialog)
{
    _object = NULL;
    _higthlightedItem = NULL;

    ui->setupUi(this);

    this->setStyleSheet(MainWindow::WidgetStyleSheetFontWithSize(12));

    /*
    QFont font12("Monaco", 12);
    ui->groupBox->setFont(font12);
    ui->lineEdit->setFont(font12);
    ui->pushAddPropertyButton->setFont(font12);
    ui->errorLabel->setFont(font12);
    ui->listWidget->setFont(font12);
*/

    ui->listWidget->setSelectionMode(QAbstractItemView::ContiguousSelection);
}

ObjectPropsDialog::~ObjectPropsDialog()
{
    delete ui;
}

void ObjectPropsDialog::on_listWidget_itemEntered(QListWidgetItem *item)
{

}

void ObjectPropsDialog::on_pushAddPropertyButton_clicked()
{
    if (_object)
    {
        QString propName(ui->lineEdit->text());
        if (!propName.isEmpty())
        {
            if (!_object->IsPropertyExistsWithNameQ(propName))
            {
                QREProperty * newProp = _object->CreateNewPropertyWithNameQ(propName);
                this->AddProperty(newProp);
            }

            ui->lineEdit->clear();
        }
    }
}

QStringList * ObjectPropsDialog::GetReservedWords()
{
    static QStringList list;
    if (list.isEmpty())
    {
        list << "alignas" <<
                "alignof"
                "and" <<
                "and_eq" <<
                "asm" <<
                "auto" <<
                "bitand" <<
                "bitor" <<
                "bool" <<
                "break" <<
                "case" <<
                "catch" <<
                "char" <<
                "char16_t" <<
                "char32_t" <<
                "class" <<
                "compl" <<
                "const" <<
                "constexpr" <<
                "const_cast" <<
                "continue" <<
                "decltype" <<
                "default" <<
                "delete" <<
                "do" <<
                "double" <<
                "dynamic_cast" <<
                "else" <<
                "enum" <<
                "explicit" <<
                "export" <<
                "extern" <<
                "false" <<
                "float" <<
                "for" <<
                "friend" <<
                "goto" <<
                "if" <<
                "inline" <<
                "int" <<
                "long" <<
                "mutable" <<
                "namespace" <<
                "new" <<
                "noexcept" <<
                "not" <<
                "not_eq" <<
                "nullptr" <<
                "operator" <<
                "or" <<
                "or_eq" <<
                "private" <<
                "protected" <<
                "public" <<
                "register" <<
                "reinterpret_cast" <<
                "return" <<
                "short" <<
                "signed" <<
                "sizeof" <<
                "static" <<
                "static_assert" <<
                "static_cast" <<
                "struct" <<
                "switch" <<
                "template" <<
                "this" <<
                "thread_local" <<
                "throw" <<
                "true" <<
                "try" <<
                "typedef" <<
                "typeid" <<
                "typename" <<
                "union" <<
                "unsigned" <<
                "using" <<
                "virtual" <<
                "void" <<
                "volatile" <<
                "wchar_t" <<
                "while" <<
                "xor" <<
                "xor_eq" <<
                "override" <<
                "final";
    }
    return (&list);
}

QList<QChar> * ObjectPropsDialog::GetUnsupportedChars()
{
    static QList<QChar> list;
    if (list.isEmpty())
    {
        list << '!' << '±' << '@' << '#' << '$' << '%' << '^' << '&' << '*' << '(' << ')' << '-' << '+' <<
                '=' << '{' << '}' << '[' << ']' << ',' << '.' << '\'' << '"' << '<' << '>' << '\\' << '/' <<
                '§' << '?' << ':' << ';' << '`' << '~';
    }
    return (&list);
}

QString ObjectPropsDialog::VariableErrorString(const QString & var)
{
    if (var.isEmpty()) return tr("Empty name");

    QStringList * list = ObjectPropsDialog::GetReservedWords();
    if (list)
    {
        if (list->contains(var, Qt::CaseSensitive)) return tr("Reserved keyword");
    }

    if (var.contains(QChar(' '), Qt::CaseSensitive)) return tr("Spaces not allowed");

    QChar ch = var.at(0);
    if (ch.isDigit()) return tr("Must not begin with a digit");

    QList<QChar> * notChars = ObjectPropsDialog::GetUnsupportedChars();
    if (notChars)
    {
        for (int i = 0; i < var.length(); i++)
        {
            ch = var.at(i);
            if (notChars->contains(ch) || (ch.unicode() > 255))
            {
                return tr("Containes unsupported char");
            }
        }
    }

    if (_object)
    {
        if (_object->IsPropertyExistsWithNameQ(var)) return tr("Allready exists");
    }

    return QString();
}

void ObjectPropsDialog::on_lineEdit_textChanged(const QString & arg1)
{
    QString errStr = VariableErrorString(arg1);
    ui->errorLabel->setText(errStr);
    ui->pushAddPropertyButton->setEnabled(errStr.isEmpty());
}

void ObjectPropsDialog::on_listWidget_itemSelectionChanged()
{
    if (_higthlightedItem)
    {
        _higthlightedItem->HighLight(false);
        _higthlightedItem = NULL;
    }

    QList<QListWidgetItem*> items(ui->listWidget->selectedItems());
    if (!items.isEmpty())
    {
        QREPropertyListWidgetItem * propItem = dynamic_cast<QREPropertyListWidgetItem *>(items.at(0));
        if (propItem)
        {
            propItem->HighLight(true);
            _higthlightedItem = propItem;
        }
    }
}
