
#ifndef QSTRINGPATH_H
#define QSTRINGPATH_H

#include <QString>

class QStringPath : public QString
{
private:
    static void SReplaceSeparators(QString * path);
    bool RemoveLastChar();
    QChar LastChar();
public:
    // default constructors, taked from QString documentation
    QStringPath();
    QStringPath(const QChar * unicode, int size);
    QStringPath(QChar ch);
    QStringPath(int size, QChar ch);
    QStringPath(const QLatin1String & str);
    QStringPath(const QString & other);
    QStringPath(const char * str);
    QStringPath(const QByteArray & ba);

    QString appendPath(const QString & path);
    QString appendPath(const char * path);
    QString addPathExtension(const QString & pathExtension);
    QString removeLastComponent();
    QString removePathExtension();
    QString lastPathComponent();

    QString stringByAppendingPath(const QString & path);
    QString stringByAppendingPath(const char * path);
    QString stringByRemovingLastComponent();

    QString absolutePath();
    QString cleanPath();

    bool isPathExists();
    bool isFilePathExists();
    bool isDirPathExists();
};

#endif // QSTRINGPATH_H

