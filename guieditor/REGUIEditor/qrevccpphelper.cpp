
#include "qrevccpphelper.h"
#include "ui_qrevccpphelper.h"
#include "qrepropertymanager.h"

QString QREVCCPPHelper::WriteObject(QREObjectCommon * obj, QStringList * writedClassNames)
{
    QString s;
    QREPropertyManager m;
    QList<QREProperty *> props;
    m.GetPropertiesForObject(obj, &props);

    QString name = obj->GetClassNameQ();
    const QREGUIObjectType type = QREGUIObjectsHelper::GetType(obj);
    QString baseName = QREGUIObjectsHelper::GetREClassNameStringForType(type);
    if (type == QREGUIObjectType_NONE)
    {
        s += "/// Error: can\'t determine base class.\n";
    }
    else if (type == QREGUIObjectType_QREVIEWCONTROLLER)
    {
        QREViewController * vc = dynamic_cast<QREViewController *>(obj);
        if (vc)
        {
            name = vc->GetNameQ();
            if (name.isEmpty())
            {
                s += "#error(\"Error: class name is empty.\")\n";
            }
        }
    }
    if (name.isEmpty())
    {
        s += "#error(\"Error: class name is empty.\")\n";
        return s;
    }

    if (writedClassNames->contains(name))
    {
        return QString("");
    }

    if (name.compare(baseName) == 0)
    {
        return s;
    }

    s += "/// Begin class " + name + ".\n/// Can place this code in separate file \""+ name +".h\" and than include if needed.\n";
    s += "#ifndef __" + name.toUpper() + "_H__\n";
    s += "#define __" + name.toUpper() + "_H__\n\n";
    s += "#include <regui.h>\n\n";
    s += "class " + name + " : public " + baseName + "\n{\n\n";

    s += "public:\n";

    if (name.compare(baseName, Qt::CaseInsensitive) != 0)
    {
        s += "\tvirtual const REUInt32 GetClassIdentifier() const\n\t{\n\t\t";
        s += "return " + name + "::ClassIdentifier();\n\t}\n";
        s += "\tstatic const REUInt32 ClassIdentifier()\n\t{\n\t\t";
        QString varName = name;
        QChar fc = varName.at(0);
        varName[0] = fc.toLower();
        varName += "ClassIdentif";
        s += "static const REUInt32 " + varName + " = REObject::GenerateClassIdentifierFromClassName(\"" + name + "\");\n\t\t";
        s += "return " + varName + ";\n\t}\n";
        s += "\tvirtual REBOOL IsImplementsClass(const REUInt32 classIdentifier) const\n\t{\n\t\t";
        s += "return ((" + name + "::ClassIdentifier() == classIdentifier) || " + baseName + "::IsImplementsClass(classIdentifier));\n\t}\n";
    }

    QStringList propsNames;
    for (int i = 0; i < props.count(); i++)
    {
        QREProperty * prop = props.at(i);
        QREObjectCommon * propPointedObj = prop->GetQREObjectCommon();
        if (propPointedObj)
        {
            QString pObjName = propPointedObj->GetClassNameQ();
            propsNames.append(prop->GetKeyStringQ());
            s += "\tRERetainProperty<" + pObjName + " *> " + prop->GetKeyStringQ() + ";\n";
        }
    }

    if (propsNames.count() > 0)
    {
        s += "\tIREObjectProperty * GetPropertyForKey(const char * key)\n\t{\n";
        for (int i = 0; i < propsNames.count(); i++)
        {
            QString pn = propsNames.at(i);
            if (i)
            {
                s += "\t\telse if (strcmp(key, \"" + pn + "\") == 0) { return (&" + pn + "); }\n";
            }
            else
            {
                s += "\t\tif (strcmp(key, \"" + pn + "\") == 0) { return (&" + pn + "); }\n";
            }
        }
        s += "\t\treturn " + baseName + "::GetPropertyForKey(key);\n\t}\n";
    }

    s += "\t" + name + "();\n";
    s += "\tvirtual ~" + name + "();\n";
    s += "};\n\n";
    s += "#endif /* __" + name.toUpper() + "_H__ */\n\n\n";

    writedClassNames->append(name);


    REView * view = dynamic_cast<REView *>(obj);
    if (view)
    {
        REObjectsArray * arr = view->GetSubViewsArray();
        if (arr)
        {
            if (arr->Count() > 0)
            {
                for (int i = 0; i < arr->Count(); i++)
                {
                    REObject * reObj = arr->At(i);
                    QREObjectCommon * reComObj = QREGUIObjectsHelper::GetCommonObject(reObj);
                    if (reComObj)
                    {
                        s += QREVCCPPHelper::WriteObject(reComObj, writedClassNames);
                    }
                }
            }
        }
    }


    return s;
}

QString QREVCCPPHelper::HelperString(QREViewController * vc)
{
    QString s;

    QStringList writed;

    s += QREVCCPPHelper::WriteObject(vc, &writed);

    return s;
}

void QREVCCPPHelper::SetViewController(QREViewController * vc)
{
    if (vc)
    {
        ui->textEdit->setText(QREVCCPPHelper::HelperString(vc));
    }
    else
    {
        ui->textEdit->setText(QString(""));
    }
}

QREVCCPPHelper::QREVCCPPHelper(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QREVCCPPHelper)
{
    ui->setupUi(this);

    QFont font12("Monaco", 12);
    ui->textEdit->setFont(font12);
    ui->pushButton->setFont(font12);
}

QREVCCPPHelper::~QREVCCPPHelper()
{
    delete ui;
}

void QREVCCPPHelper::on_pushButton_clicked()
{
    this->close();
}

