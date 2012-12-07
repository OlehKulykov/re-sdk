
#ifndef REQTCONVERTER_H
#define REQTCONVERTER_H

#include <recore.h>
#include <QColor>
#include <QRect>
#include <QRectF>
#include <QSizeF>
#include <QSize>
#include <QPointF>
#include <QPoint>
#include <QString>

class REQTConverter
{
public:
    static QColor GetQColor(const REColor & c);
    static REColor GetREColor(const QColor & c);
    static RERect GetRERect(const QRectF & f);
    static QRectF GetQRectF(const RERect & f);
    static QRect GetQRectInt(const RERect & f);

    static QSizeF GetQSizeF(const RESize & s);
    static QPointF GetQPointF(const REPoint2 & p);
    static REPoint2 GetREPoint(const QPointF & p);
    static QString GetQString(const REString & s);
    static REString GetREString(const QString & s);
    static RESize GetRESize(const QSizeF & s);

    static bool QFileReadToBuffer(const QString & path, REBuffer * buff);
    static bool REBufferWriteToPath(REBuffer * buff, const QString & path);
};

#endif // REQTCONVERTER_H

