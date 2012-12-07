
#include "treepropertybrowser.h"
#include "mainwindow.h"

void TreePropertyBrowser::OnCustomContextMenuRequested(const QPoint & pos)
{
    QtBrowserItem * curItem = this->currentItem();
    if (curItem)
    {
        QtProperty * prop = curItem->property();
        if (prop)
        {
            MainWindow::PropertyCustomContextMenuRequested(prop, this->mapToGlobal(pos));
        }
    }
}

TreePropertyBrowser::TreePropertyBrowser(QWidget *parent) :
    QtTreePropertyBrowser(parent)
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(OnCustomContextMenuRequested(QPoint)));
}

TreePropertyBrowser::~TreePropertyBrowser()
{
    disconnect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(OnCustomContextMenuRequested(QPoint)));
}
