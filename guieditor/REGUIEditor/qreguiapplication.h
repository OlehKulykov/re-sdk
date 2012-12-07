
#ifndef QREGUIAPPLICATION_H
#define QREGUIAPPLICATION_H

#include "qreobject.h"

class QREGUIApplication : public REGUIApplication
{
protected:
    QStringPath _fullDataFolderPath;

    static QREGUIApplication * _defaultApplication;
public:
    QStringPath GetFullDataFolderPath() const { return _fullDataFolderPath; }
    bool SetFullDataFolderPath(QString path);

    QREGUIApplication();
    virtual ~QREGUIApplication();


    static QREGUIApplication * GetDefaultApplication();
    static void ReleaseDefaultApplication();
};

#endif // QREGUIAPPLICATION_H

