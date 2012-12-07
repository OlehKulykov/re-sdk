#include "reqtconverter.h"
#include <QFile>
#include <QDir>
#include "qstringpath.h"

#ifdef __RE_OS_WINDOWS__
#include <math.h>
#include <float.h>
double nearbyint( double x )
{
    if(FLT_ROUNDS == 0)         // towards zero
    {
        return (double)(((int)x) + 1);
    }
    else if(FLT_ROUNDS == 1)    // nearest
    {
        if(x < ((double)((int)x) + 0.5))
            return (double)((int)x);
        else
            return (double)(((int)x) + 1);
    }
}
#endif

QColor REQTConverter::GetQColor(const REColor & c)
{
    return QColor::fromRgbF((qreal)c.red, (qreal)c.green, (qreal)c.blue, (qreal)c.alpha);
}

REColor REQTConverter::GetREColor(const QColor & c)
{
    qreal r = 0, g = 0, b = 0, a = 0;
    c.getRgbF(&r, &g, &b, &a);
    return REColor((REFloat32)r, (REFloat32)g, (REFloat32)b, (REFloat32)a);
}

RERect REQTConverter::GetRERect(const QRectF & f)
{
    return RERect((REFloat32)f.x(), (REFloat32)f.y(), (REFloat32)f.width(), (REFloat32)f.height());
}

QRectF REQTConverter::GetQRectF(const RERect & f)
{
    QRectF qf;
    qf.setX((qreal)f.x);
    qf.setY((qreal)f.y);
    qf.setWidth((qreal)f.width);
    qf.setHeight((qreal)f.height);
    return qf;
}

QRect REQTConverter::GetQRectInt(const RERect & f)
{
    QRect resRect;
    resRect.setX((int)nearbyint(f.x));
    resRect.setY((int)nearbyint(f.y));
    resRect.setWidth((int)nearbyint(f.width));
    resRect.setHeight((int)nearbyint(f.height));
    return resRect;
}

QSizeF REQTConverter::GetQSizeF(const RESize & s)
{
    QSizeF qs;
    qs.setWidth((qreal)s.width);
    qs.setHeight((qreal)s.height);
    return qs;
}

QPointF REQTConverter::GetQPointF(const REPoint2 & p)
{
    QPointF qp;
    qp.setX((qreal)p.x);
    qp.setY((qreal)p.y);
    return qp;
}


REPoint2 REQTConverter::GetREPoint(const QPointF & p)
{
    return REPoint2((REFloat32)p.x(), (REFloat32)p.y());
}

QString REQTConverter::GetQString(const REString & s)
{
    REStringPresentation p(s);
    return QString::fromWCharArray(p.WideString(), (int)p.GetWideLength());
}

REString REQTConverter::GetREString(const QString & s)
{
    const REUInt32 len = s.length();
    const REUInt32 needSize = (len + 1) * sizeof(wchar_t);
    REBuffer buff(needSize);
    if (buff.GetSize() == needSize)
    {
        wchar_t * wstr = (wchar_t*)buff.GetBuffer();
        wstr[len] = (wchar_t)0;
        for (REUInt32 i = 0; i < len; i++)
        {
            QChar c(s.at((int)i));
            wstr[i] = (wchar_t)c.unicode();
        }
        return REString(wstr, len);
    }
    return REString();
}

RESize REQTConverter::GetRESize(const QSizeF & s)
{
    RESize rs;
    rs.width = (REFloat32)s.width();
    rs.height = (REFloat32)s.height();
    return rs;
}

bool REQTConverter::QFileReadToBuffer(const QString & path, REBuffer * buff)
{
    if (buff && (!path.isEmpty()))
    {
        buff->Clear();
        QFile f(path);
        if (f.open(QIODevice::ReadOnly))
        {
            const qint64 size = f.size();
            if (size > 0)
            {
                if (buff->Resize((REUInt32)size, false))
                {
                    const qint64 readed = f.read((char*)buff->GetBuffer(), (qint64)buff->GetSize());
                    if ((readed > 0) && (readed == size))
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool REQTConverter::REBufferWriteToPath(REBuffer * buff, const QString & path)
{
    if (buff && (!path.isEmpty()))
    {
        QFile f1(path);
        f1.remove();

        QStringPath p(path);
        p.removeLastComponent();
        QDir dir(p);
        dir.mkpath(p);

        QFile f(path);
        if (f.open(QIODevice::WriteOnly))
        {
            const qint64 needWrite = (qint64)buff->GetSize();
            const quint64 writed = f.write((const char*)buff->GetBuffer(), needWrite);
            f.close();
            if (needWrite == writed)
            {
                return true;
            }
            else
            {
                f.remove();
            }
        }
    }
    return false;
}
