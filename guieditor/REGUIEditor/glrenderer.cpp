
#include "glrenderer.h"
#include <QMouseEvent>
#include "guiobjectstreeitem.h"
#include "guiobjectstreewidget.h"
#include "mainwindow.h"
#include "qreview.h"
#include "qrelabel.h"
#include "qrebutton.h"
#include "qretextfield.h"
#include "qreparticleview.h"
#include "qrepropertymimedata.h"

REUInt32 GLRenderer::_pauseRenderingCount = 0;

void GLRenderer::PauseRendering()
{
    _pauseRenderingCount++;
}

void GLRenderer::ResumeRendering()
{
    if (_pauseRenderingCount)
    {
        _pauseRenderingCount--;
    }
}

REUInt32 GLRenderer::SearchSubViews(REView * view, const REFloat32 screenX, const REFloat32 screenY, REArray<REView*> * onClickFoundedViews)
{
    if (view)
    {
        if (view->GetScreenFrame().IsPointInRect(screenX, screenY))
        {
            onClickFoundedViews->Add(view);
        }

        const REObjectsArray * sv = view->GetSubViewsArray();
        if (sv)
        {
            REObjectsArray * subViews = const_cast<REObjectsArray *>(sv);
            for (REUInt32 i = 0; i < subViews->Count(); i++)
            {
                REView * subView = (REView *)(*subViews)[i];
                if (GLRenderer::SearchSubViews(subView, screenX, screenY, onClickFoundedViews))
                {
                    return 1;
                }
            }
        }
    }

    return 0;
}

GUIObjectsTreeItem * GLRenderer::GetItemAtScreenPos(const int screenX, const int screenY)
{
    QREGUIApplication * app = QREGUIApplication::GetDefaultApplication();
    if (app)
    {
        RESize ratio( RERenderDevice::GetDefaultDevice()->GetScreenToRenderSizeRatio() );
        const REFloat32 x = (REFloat32)((long double)ratio.width * (long double)screenX);
        const REFloat32 y = (REFloat32)((long double)ratio.height * (long double)screenY);
        REArray<REView*> onClickFoundedViews;
        GLRenderer::SearchSubViews(_rootViewController, x, y, &onClickFoundedViews);

        if (onClickFoundedViews.Count())
        {
            REView * topView = onClickFoundedViews.LastObject();
            QREObjectCommon * comObj = QREGUIObjectsHelper::GetCommonObject(topView);
            const QREGUIObjectType type = QREGUIObjectsHelper::GetType(comObj);
            if (comObj && (type != QREGUIObjectType_NONE))
            {
                GUIObjectsTreeItem * item = comObj->GetAssignedTreeItem();
                return item;
            }
        }
    }
    return NULL;
}

void GLRenderer::mousePressEvent(QMouseEvent * event)
{
    GUIObjectsTreeWidget * treeView = MainWindow::GetGUIObjectsTreeWidget();
    GUIObjectsTreeItem * item = this->GetItemAtScreenPos(event->x(), event->y());
    if (item)
    {
        if (treeView)
        {
            treeView->UnselectAllItems(true);
        }
        item->setSelected(true);
        if (treeView)
        {
            treeView->scrollToItem(item);
        }
    }
}

void GLRenderer::initializeGL()
{
    QREGUIApplication * app = QREGUIApplication::GetDefaultApplication();
    if (app)
    {

        app->SetRootViewController(_rootViewController);

        RERenderDevice::GetDefaultDevice()->Initialize();

        app->Initialize();

        /*
        REData data;
        data.InitFromPath("/Users/residentevil/Library/R2D6/DATA_iphone/data/vc/com/no.png");

        REImage img;
        img.InitFromFileData(data);
        RETextureObject * qtext = RETextureObject::CreateWithImageFilePath(REString("/Users/residentevil/Library/R2D6/DATA_iphone/data/vc/com/no.png"));
        //qtext = RETextureObject::CreateWithImageFilePath(REString("E:/WORK/CPP/R2D6/DATA_iphone/data/vc/com/no.png"));
        //qtext->Update(img.GetImageData(), img.GetPixelFormat(), img.GetWidth(), img.GetHeight());

        QREView * newView = new QREView();
        REViewController * vc = dynamic_cast<REViewController*>(_rootViewController);
        vc->SetTag(-1);
        REView * v = newView->review();
        vc->AddSubView(v);
        v->SetTag(1);
        v->SetFrame(RERect(0.2, 0.2, 0.5, 0.5));
        v->SetTexture(qtext);
        v->Release();

        QRETextureObject * ttttt = new QRETextureObject();
        newView = new QREView();
        v->AddSubView(newView->review());
        v = newView->review();
        v->SetTag(2);
        v->SetTexture(ttttt);
        v->SetFrame(RERect(0.2, 0.2, 0.5, 0.5));
        v->Release();


        QREFontObject * font = new QREFontObject();
        QRELabel * newLabel = new QRELabel();
        v = newLabel->relabel();
        vc->AddSubView(newLabel->relabel());
        v->SetFrame(RERect(1.2, 0.2, 0.5, 0.5));
        v->Release();
        v->SetTag(3);
        newLabel->SetFont(font);


        QREButton * newButton = new QREButton();
        REButton * reButton = newButton->rebutton();
        reButton->SetFrame(RERect(1, 0.3, 0.5, 0.2));
        vc->AddSubView(reButton);
        reButton->SetTexture(ttttt);

        QRETextureObject * ttttt1 = new QRETextureObject();
        QRETextureObject * ttttt2 = new QRETextureObject();
        reButton->SetTextureForState(ttttt1, REButton::StatePressed);
        reButton->SetTextureForState(ttttt2, REButton::StateDisabled);


        QRETextField * qTextField = new QRETextField();
        RETextField * textField = qTextField->retextfield();
        textField->SetFrame(RERect(1.0, 1.0, 0.7, 0.2));
        vc->AddSubView(textField);
        textField->Release();

        QREParticleView * qPart = new QREParticleView();

        vc->AddSubView(qPart);
        qPart->Release();

        newView = new QREView();
         vc = dynamic_cast<REViewController*>(_rootViewController);
        vc->SetTag(-10);
        v = newView->review();
        vc->AddSubView(v);
        v->SetTag(1);
        v->SetFrame(RERect(0.3, 0.7, 0.5, 0.5));
        v->SetTexture(qtext);
        v->Release();
        */
    }
}

void GLRenderer::resizeGL(int width, int height)
{
    RERenderDevice * device = RERenderDevice::GetDefaultDevice();
    if (device)
    {
        device->SetClearColor(REColor(0, 0, 0, 1));
        device->SetScreenSize((REFloat32)width, (REFloat32)height);
    }
}

void GLRenderer::paintGL()
{
    if (_pauseRenderingCount == 0)
    {
        QREGUIApplication * app = QREGUIApplication::GetDefaultApplication();
        if (app)
        {
            app->Update();

            app->Render();
        }
    }
}

void GLRenderer::Init()
{
    this->setAcceptDrops(true);

    _highLightedItem = NULL;
    _scale = 1.0f;

    QREViewController * vc = new QREViewController();
    if (vc)
    {
        _rootViewController = vc;
        vc->SetRespondsForUserAction(true);
        if (_rootViewController)
        {
            _rootViewController->SetFrame(RERect(0.0f, 0.0f, 1.0f, 1.0f));
        }
    }
    else
    {
        _rootViewController = NULL;
        RE_SAFE_DELETE(vc);
    }
}

void GLRenderer::SetScaleValue(REFloat32 v)
{
    _scale = v;
}

void GLRenderer::dropEvent(QDropEvent *de)
{
    QPoint pos = de->pos();

    GUIObjectsTreeItem * item = this->GetItemAtScreenPos(pos.x(), pos.y());
    if (item) item->HighLight(false);
    if (_highLightedItem != item)
    {
        if (_highLightedItem) _highLightedItem->HighLight(false);
        _highLightedItem = NULL;
    }

    const QMimeData * md = de->mimeData();
    if (md)
    {
        const QREPropertyMimeData * mimeData = dynamic_cast<const QREPropertyMimeData *>(md);
        if (mimeData && item)
        {
            QREObjectCommon * com = item->GetGuiObject();
            QREProperty * prop = mimeData->GetProperty();
            if (com && prop)
            {
                REObject * reobj = com->reobject();
                prop->SetObject(reobj);

                com->AssignEditorIDQ();
                if (com->GetEditorIDQ())
                {
                    if (mimeData->GetRadioButton())
                    {
                        mimeData->GetRadioButton()->setChecked(true);
                    }
                    MainWindow::GetGUIObjectsTreeWidget()->ReloadSelectedObjectProperties();
                }
            }
        }
    }
}

void GLRenderer::dragMoveEvent(QDragMoveEvent *de)
{
    QPoint pos = de->pos();
    GUIObjectsTreeItem * item = this->GetItemAtScreenPos(pos.x(), pos.y());

    if (item) item->HighLight(true);
    if (_highLightedItem != item)
    {
        if (_highLightedItem) _highLightedItem->HighLight(false);
        _highLightedItem = item;
    }

    de->accept();
}

void GLRenderer::dragEnterEvent(QDragEnterEvent *event)
{
    if (_highLightedItem)
    {
        _highLightedItem->HighLight(false);
        _highLightedItem = NULL;
    }
    event->acceptProposedAction();
}

void GLRenderer::dragLeaveEvent(QDragLeaveEvent * event)
{
    if (_highLightedItem)
    {
        _highLightedItem->HighLight(false);
        _highLightedItem = NULL;
    }
}

GLRenderer::GLRenderer(QWidget * parent, const QGLWidget * shareWidget, Qt::WindowFlags f) :
    QGLWidget(parent, shareWidget, f)
{
    this->Init();
}

GLRenderer::GLRenderer(QGLContext * context, QWidget * parent, const QGLWidget * shareWidget, Qt::WindowFlags f) :
    QGLWidget(context, parent, shareWidget, f)
{
    this->Init();
}

GLRenderer::GLRenderer(const QGLFormat & format, QWidget * parent, const QGLWidget * shareWidget, Qt::WindowFlags f) :
    QGLWidget(format, parent, shareWidget, f)
{
    this->Init();
}

GLRenderer::~GLRenderer()
{

}




