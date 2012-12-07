
#include "savevcinfo.h"
#include <regui.h>

void SaveVCInfo::SetOptimizedVCScreenSize(const QSizeF & s)
{
    _optVCScrSize = s;
}

void SaveVCInfo::SetCurrentVCScreenSize(const QSizeF & s)
{
    _curVCScrSize = s;
}

QStringPath SaveVCInfo::GetTextureQStoreFullPath() const
{
    if (!_vcName.isEmpty())
    {
        QStringPath p(_storeDataFullPath);
        p.appendPath("vc");
        p.appendPath(_vcName);
        p.appendPath("img");
        return p;
    }
    return QStringPath();
}

QStringPath SaveVCInfo::GetTextureQStoreDataPath() const
{
    if (!_vcName.isEmpty())
    {
        QStringPath p("data");
        p.appendPath("vc");
        p.appendPath(_vcName);
        p.appendPath("img");
        return p;
    }
    return QStringPath();
}

QStringPath SaveVCInfo::GetViewControllerXMLStoreFullPath() const
{
    if (!_vcName.isEmpty())
    {
        QStringPath p(_storeDataFullPath);
        p.appendPath("vc");
        p.appendPath(_vcName);
        p.appendPath("vc.xml");
        return p;
    }
    return QStringPath();
}

QStringPath SaveVCInfo::GetFontQStoreFullPath() const
{
    QStringPath p(_storeDataFullPath);
    p.appendPath("fnt");
    return p;
}

QStringPath SaveVCInfo::GetFontQStoreDataPath() const
{
    QStringPath p("data");
    p.appendPath("fnt");
    return p;
}

QSizeF SaveVCInfo::GetREViewResultScreenSize(REView * view)
{
    QSizeF resultSize(-1, -1);
    if (view)
    {
        RESize ratio(RERenderDevice::GetDefaultDevice()->GetRenderToScreenSizeRatio());
        RERect frame(view->GetFrame());
        frame.width *= ratio.width;
        frame.height *= ratio.height;
        if (_isOptimizeForScrSize)
        {
            const long double w = ( ((long double)frame.width * (long double)_optVCScrSize.width()) / (long double)_curVCScrSize.width() );
            const long double h = ( ((long double)frame.height * (long double)_optVCScrSize.height()) / (long double)_curVCScrSize.height() );
            resultSize.setWidth((qreal)w);
            resultSize.setHeight((qreal)h);
        }
        else
        {
            resultSize.setWidth((qreal)frame.width);
            resultSize.setHeight((qreal)frame.height);
        }
    }
    return resultSize;
}

SaveVCInfo::SaveVCInfo() :
    _isOptimizeForScrSize(false),
    _isScaleTextureImgForScrSize(false),
    _isChangeTextureImgFormatForSmalestSize(false)
{

}

SaveVCInfo::~SaveVCInfo()
{

}
