
#ifndef SAVEVCINFO_H
#define SAVEVCINFO_H

#include "qstringpath.h"
#include <QSize>
#include <QSizeF>

class REView;

class SaveVCInfo
{
private:
    QString _vcName;
    QStringPath _storeDataFullPath;
    QSizeF _curVCScrSize;
    QSizeF _optVCScrSize;

    bool _isOptimizeForScrSize;
    bool _isScaleTextureImgForScrSize;
    bool _isChangeTextureImgFormatForSmalestSize;
public:
    const QString & GetVCName() const { return _vcName; }
    void SetVCName(const QString & vcname) { _vcName = vcname; }

    const QStringPath & GetStoreDataFullPath() const { return _storeDataFullPath; }
    void SetStoreDataFullPath(const QString & p) { _storeDataFullPath = p; }

    bool IsOptimizeForScrSize() const { return _isOptimizeForScrSize; }
    void SetOptimizeForScrSize(bool isOpt) { _isOptimizeForScrSize = isOpt; }

    const QSizeF & GetCurrentVCScreenSize() const { return _curVCScrSize; }
    void SetCurrentVCScreenSize(const QSizeF & s);

    const QSizeF & GetOptimizedVCScreenSize() const { return _optVCScrSize; }
    void SetOptimizedVCScreenSize(const QSizeF & s);

    bool IsScaleTextureImageForScreenSize() const { return _isScaleTextureImgForScrSize; }
    bool IsChangeTextureImageFormatForSmallestFileSize() const { return _isChangeTextureImgFormatForSmalestSize; }

    void SetScaleTextureImageForScreenSize(bool isSc) { _isScaleTextureImgForScrSize = isSc; }
    void SetChangeTextureImageFormatForSmallestFileSize(bool isCha) { _isChangeTextureImgFormatForSmalestSize = isCha; }

    QStringPath GetTextureQStoreFullPath() const;
    QStringPath GetTextureQStoreDataPath() const;

    QStringPath GetFontQStoreFullPath() const;
    QStringPath GetFontQStoreDataPath() const;

    QStringPath GetViewControllerXMLStoreFullPath() const;

    QSizeF GetREViewResultScreenSize(REView * view);

    SaveVCInfo();
    virtual ~SaveVCInfo();
};

#endif // SAVEVCINFO_H

