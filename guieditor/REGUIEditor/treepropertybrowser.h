
#ifndef TREEPROPERTYBROWSER_H
#define TREEPROPERTYBROWSER_H

#include <QWidget>
#include "QtTreePropertyBrowser"

class TreePropertyBrowser : public QtTreePropertyBrowser
{
    Q_OBJECT
public:
    explicit TreePropertyBrowser(QWidget *parent = 0);
    virtual ~TreePropertyBrowser();
signals:
    
public slots:
    void OnCustomContextMenuRequested(const QPoint & pos);
};

#endif // TREEPROPERTYBROWSER_H
