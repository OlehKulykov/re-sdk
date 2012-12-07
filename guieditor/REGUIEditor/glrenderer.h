
#ifndef GLRENDERER_H
#define GLRENDERER_H

#include <QWidget>
#include <QGLWidget>
#include <QRect>
#include <QRectF>
#include <recore.h>
#include <regui.h>
#include <math.h>

#include "qreviewcontroller.h"
#include "qreguiobjectshelper.h"
#include "QtTreePropertyBrowser"
#include "qreguiapplication.h"

class GUIObjectsTreeWidget;
class GUIObjectsTreeItem;

class GLRenderer : public QGLWidget
{
    Q_OBJECT

protected:
    REFloat32 _scale;
    GUIObjectsTreeItem * _highLightedItem;
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
    virtual void paintGL();
    virtual void dropEvent(QDropEvent *de);
    virtual void dragMoveEvent(QDragMoveEvent *de);
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dragLeaveEvent (QDragLeaveEvent * event);

    GUIObjectsTreeItem * GetItemAtScreenPos(const int screenX, const int screenY);

    QREViewController * _rootViewController;

    void Init();

    static REUInt32 SearchSubViews(REView * view, const REFloat32 screenX, const REFloat32 screenY, REArray<REView*> * onClickFoundedViews);
    static REUInt32 _pauseRenderingCount;

public:
    void SetScaleValue(REFloat32 v);
    QREViewController * GetRootQVC() { return _rootViewController; }
    REViewController * GetRootREVC() { return _rootViewController; }

    GLRenderer(QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);
    GLRenderer(QGLContext * context, QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);
    GLRenderer(const QGLFormat & format, QWidget * parent = 0, const QGLWidget * shareWidget = 0, Qt::WindowFlags f = 0);
    virtual ~GLRenderer();

    static void PauseRendering();
    static void ResumeRendering();
signals:
    
public slots:
    
};

#endif // GLRENDERER_H
