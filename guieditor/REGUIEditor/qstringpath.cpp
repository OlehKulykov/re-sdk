
#include "qstringpath.h"
#include <QDir>
#include <QFile>

void QStringPath::SReplaceSeparators(QString * path)
{
    if ( path->isEmpty() )
    {
        return;
    }

    QChar separator('/');
    QChar replaceChar('\\');

    path->replace(replaceChar, separator);

    int lastIndex = path->length() - 1;
    QChar lastChar = path->at(lastIndex);
    if (lastChar == separator)
    {
        path->remove(lastIndex, 1);
    }
}

bool QStringPath::RemoveLastChar()
{
    int lastIndex = this->length() - 1;
    if (lastIndex >= 0)
    {
        this->remove(lastIndex, 1);
        return true;
    }
    return false;
}

QChar QStringPath::LastChar()
{
    int lastIndex = this->length() - 1;
    if (lastIndex >= 0)
    {
        return this->at(lastIndex);
    }
    return QChar(0);
}

QStringPath::QStringPath () : QString()
{

}

QStringPath::QStringPath ( const QChar * unicode, int size ) : QString(unicode, size)
{
    QStringPath::SReplaceSeparators(this);
}

QStringPath::QStringPath ( QChar ch ) : QString(ch)
{
    QStringPath::SReplaceSeparators(this);
}

QStringPath::QStringPath ( int size, QChar ch ) : QString(size, ch)
{
    QStringPath::SReplaceSeparators(this);
}

QStringPath::QStringPath ( const QLatin1String & str ) : QString(str)
{
    QStringPath::SReplaceSeparators(this);
}

QStringPath::QStringPath ( const QString & other ) : QString(other)
{
    QStringPath::SReplaceSeparators(this);
}

QStringPath::QStringPath ( const char * str ) : QString(str)
{
    QStringPath::SReplaceSeparators(this);
}

QStringPath::QStringPath ( const QByteArray & ba ) : QString(ba)
{
    QStringPath::SReplaceSeparators(this);
}

QString QStringPath::removeLastComponent()
{
    QChar sep1('/');
    QChar sep2('\\');
    QChar lastChar = this->LastChar();
    while ( (lastChar != sep1) && (lastChar != sep2) )
    {
        if ( !this->RemoveLastChar() )
        {
            QStringPath::SReplaceSeparators(this);
            return *this;
        }
        lastChar = this->LastChar();
    }
    QStringPath::SReplaceSeparators(this);
    return *this;
}

QString QStringPath::removePathExtension()
{
    QString sourceString(*this);
    QChar sep1('/');
    QChar sep2('\\');
    QChar dot('.');
    QChar lastChar = this->LastChar();
    while (this->length() > 0)
    {
        if ((lastChar == sep1) || (lastChar == sep2))
        {
            // error. dot not found
            (*this) = sourceString;
            QStringPath::SReplaceSeparators(this);
            return (*this);
        }
        else if (lastChar == dot)
        {
            if (!this->RemoveLastChar())
            {
                (*this) = sourceString;
            }
            QStringPath::SReplaceSeparators(this);
            return (*this);
        }
        else
        {
            this->RemoveLastChar();
            lastChar = this->LastChar();
        }
    }

    QStringPath::SReplaceSeparators(this);
    return (*this);
}

QString QStringPath::addPathExtension(const QString & pathExtension)
{
    if (pathExtension.length() > 0)
    {
        (*this) += QChar('.');
        (*this) += pathExtension;
        QStringPath::SReplaceSeparators(this);
    }
    return (*this);
}

QString QStringPath::lastPathComponent()
{
    QChar sep1('/');
    QChar sep2('\\');
    int rLen = 0;
    for (int i = (this->length() - 1); i >= 0; i-- )
    {
        QChar c = this->at(i);
        if ( (c != sep1) && (c != sep2) )
        {
            rLen++;
        }
        else
        {
            break;
        }
    }
    return this->right(rLen);
}

QString QStringPath::stringByRemovingLastComponent()
{
    QStringPath resultPath( *this );
    return resultPath.removeLastComponent();
}

QString QStringPath::absolutePath()
{
    QDir dir( *this );
    return dir.absolutePath();
}

QString QStringPath::cleanPath()
{
    return QDir::cleanPath( *this );
}

QString QStringPath::appendPath(const QString & path)
{
    this->append( QDir::separator() );
    this->append( path );
    QStringPath::SReplaceSeparators(this);
    return *this;
}

QString QStringPath::appendPath(const char * path)
{
    if ( path != NULL )
    {
        this->append( QDir::separator() );
        this->append( path );
        QStringPath::SReplaceSeparators(this);
    }
    return *this;
}

QString QStringPath::stringByAppendingPath(const QString & path)
{
    QString resultPath( *this );
    resultPath.append( QDir::separator() );
    resultPath.append( path );
    QStringPath::SReplaceSeparators(&resultPath);
    return resultPath;
}

QString QStringPath::stringByAppendingPath(const char * path)
{
    QString resultPath( *this );
    if ( path != NULL )
    {
        resultPath.append( QDir::separator() );
        resultPath.append( path );
        QStringPath::SReplaceSeparators(&resultPath);
    }
    return resultPath;
}

bool QStringPath::isFilePathExists()
{
    return QFile::exists(*this);
}

bool QStringPath::isDirPathExists()
{
    if ( this->isEmpty() )
    {
        return false;
    }
    QDir dir(*this);
    return dir.exists();
}

bool QStringPath::isPathExists()
{
    if ( this->isFilePathExists() )
    {
        return true;
    }
    return this->isDirPathExists();
}

