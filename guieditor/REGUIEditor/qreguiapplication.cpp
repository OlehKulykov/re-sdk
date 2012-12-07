
#include "qreguiapplication.h"
#include <QDir>

QREGUIApplication * QREGUIApplication::_defaultApplication = NULL;

bool QREGUIApplication::SetFullDataFolderPath(QString path)
{
    _fullDataFolderPath.clear();
    REResourcesStorage s;
    s.Clear();
    QDir dir(path);
    if (dir.exists())
    {
        QStringPath qPath = dir.absolutePath();
        if (s.AddResourcesPath(REQTConverter::GetREString(qPath)))
        {
            _fullDataFolderPath = qPath;
            return true;
        }
    }
    return false;
}

QREGUIApplication::QREGUIApplication() : REGUIApplication()
{

}

QREGUIApplication::~QREGUIApplication()
{

}

QREGUIApplication * QREGUIApplication::GetDefaultApplication()
{
    if (_defaultApplication == NULL)
    {
        _defaultApplication = new QREGUIApplication();
    }
    return _defaultApplication;
}

void QREGUIApplication::ReleaseDefaultApplication()
{
    RE_SAFE_DELETE(_defaultApplication);
}



