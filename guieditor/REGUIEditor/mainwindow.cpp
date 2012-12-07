
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "qreguiapplication.h"
#include <QMessageBox>
#include "qrebutton.h"
#include "qrelabel.h"
#include "qretextfield.h"
#include "qreparticleview.h"
#include "xmloutputdialog.h"
#include <QWidgetAction>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>
#include "saveviewcontrollerdialog.h"
#include "qrevccpphelper.h"
#include "aboutdialog.h"


class RendererScaleSliderAction : public QWidgetAction
{
public:
    RendererScaleSliderAction(const QString & title) :
        QWidgetAction(NULL)
    {
        QWidget * pWidget = new QWidget(NULL);
        QHBoxLayout * pLayout = new QHBoxLayout();
        QLabel * pLabel = new QLabel(title);
        pLayout->addWidget(pLabel);
        _slider = new QSlider(NULL);
        _slider->setOrientation(Qt::Horizontal);
        _slider->setMinimum(10);
        _slider->setMaximum(200);
        _slider->setMinimumWidth(200);
        _slider->setValue(100);
        pLayout->addWidget(_slider);
        _persentLabel = new QLabel(title);
        _persentLabel->setStyleSheet(MainWindow::WidgetStyleSheetFontWithSize(12));
        _persentLabel->setText("100 %");
        pLayout->addWidget(_persentLabel);
        pWidget->setLayout(pLayout);
        setDefaultWidget(pWidget);
    }

    QSlider * slider() { return _slider; }
    QLabel * persentLabel() { return _persentLabel; }
private:
    QSlider * _slider;
    QLabel * _persentLabel;
};

QString MainWindow::WidgetStyleSheetFontWithSize(int size)
{   
    const char * fontName = "Tahoma";

#ifdef __RE_OS_MACOSX__
    static bool isMacFontChecked = false;
    static bool isMacFontAvaiable = false;
    if (!isMacFontChecked)
    {
        QFont f("Monaco", 12);
        isMacFontAvaiable = f.rawName().contains("Monaco", Qt::CaseInsensitive);
        isMacFontChecked = true;
    }
    if (isMacFontAvaiable)
    {
        fontName = "Monaco";
    }
#endif

    return QString().sprintf("QWidget { font-family:%s; font-style:normal; font-size:%ipx; font-weight:400; font-variant:normal; }", fontName, size);
}

MainWindow * MainWindow::_mainWindow = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setAcceptDrops(true);

    _mainWindow = this;


    this->setStyleSheet(MainWindow::WidgetStyleSheetFontWithSize(12));

    _renderDialog = new RenderDialog();
    _renderDialog->show();
    _renderDialog->hide();

    ui->treeWidget->SetRootVC(_renderDialog->GetRenderer()->GetRootQVC());

    _scaleRendererAction = new RendererScaleSliderAction(tr("Scale"));
    ui->menuRendererScale->addAction(_scaleRendererAction);

    connect(_scaleRendererAction->slider(), SIGNAL(valueChanged(int)), this, SLOT(OnScaleRendererValueChanged(int)));

    MainWindow::OnSelectedNewTreeObject();
}

MainWindow::~MainWindow()
{
    RE_SAFE_DELETE(_renderDialog);
    delete ui;
}

MainWindow * MainWindow::GetMainWindow()
{
    return _mainWindow;
}

QtTreePropertyBrowser * MainWindow::GetPropertyBrowser()
{
    if (_mainWindow) return _mainWindow->ui->widget;
    return NULL;
}

GUIObjectsTreeWidget * MainWindow::GetGUIObjectsTreeWidget()
{
    if (_mainWindow) return _mainWindow->ui->treeWidget;
    return NULL;
}

GUIObjectsTreeItem * MainWindow::GetSelectedTreeItem()
{
    GUIObjectsTreeWidget * tree = MainWindow::GetGUIObjectsTreeWidget();
    if (tree) return tree->GetSelectedGUITreeItem();
    return NULL;
}

QREObjectCommon * MainWindow::GetSelectedGUIObject()
{
    GUIObjectsTreeItem * item = MainWindow::GetSelectedTreeItem();
    if (item) return item->GetGuiObject();
    return NULL;
}

QREGUIObjectType MainWindow::GetSelectedGUIObjectType()
{
    QREObjectCommon * selObject = MainWindow::GetSelectedGUIObject();
    if (selObject) return QREGUIObjectsHelper::GetType(selObject);
    return QREGUIObjectType_NONE;
}

void MainWindow::PropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos)
{
    GUIObjectsTreeWidget * tree = MainWindow::GetGUIObjectsTreeWidget();
    if (tree) tree->PropertyCustomContextMenuRequested(property, pos);
}

void MainWindow::OnSelectedNewTreeObject()
{
    MainWindow * mainWnd = MainWindow::GetMainWindow();
    if (mainWnd)
    {
        Ui::MainWindow * ui = mainWnd->ui;
        if (ui)
        {
            bool view = false, button = false, label = false, textField = false, particle = false;
            bool newObj = false, selObj = false, selObjMoveUp = false, selObjMoveDown = false, selObjDelete = false;
            const QREGUIObjectType type = MainWindow::GetSelectedGUIObjectType();
            switch (type)
            {
            case QREGUIObjectType_QREVIEW:
                view = true;
                selObjDelete = true;
                newObj = true;
                break;

            case QREGUIObjectType_QREVIEWCONTROLLER:
                newObj = true;
                break;

            case QREGUIObjectType_QREBUTTON:
                button = true;
                selObjDelete = true;
                newObj = true;
                break;

            case QREGUIObjectType_QREFONTOBJECT:
                selObjDelete = true;
                break;

            case QREGUIObjectType_QRELABEL:
                label = true;
                selObjDelete = true;
                newObj = true;
                break;

            case QREGUIObjectType_QREPARTICLEVIEW:
                particle = true;
                selObjDelete = true;
                newObj = true;
                break;

            case QREGUIObjectType_QRETEXTFIELD:
                textField = true;
                selObjDelete = true;
                break;

            case QREGUIObjectType_QRETEXTUREOBJECT:
                selObjDelete = true;
                break;

            case QREGUIObjectType_QREOBJECT:
                break;
            default: break;
            }

            if (selObjMoveUp || selObjMoveDown || selObjDelete)
            {
                selObj = true;
            }


            ui->menuView_2->setEnabled(view);
            ui->menuButton->setEnabled(button);
            ui->menuLabel->setEnabled(label);
            ui->menuTextField->setEnabled(textField);
            ui->menuParticleView->setEnabled(particle);
            ui->menuNewObject->setEnabled(newObj);
            ui->menuSelectedObject->setEnabled(selObj);
            //ui->actionSelectedObjectMoveUp->setEnabled(selObjMoveUp);
            //ui->actionSelectedObjectMoveDown->setEnabled(selObjMoveDown);
            ui->actionSelectedObjectDelete->setEnabled(selObjDelete);
        }
    }
}

void MainWindow::on_actionOpen_data_folder_triggered()
{
    _dataFullPath.clear();

    QREGUIApplication * app = QREGUIApplication::GetDefaultApplication();
    if (app)
    {
        QFileDialog dialog(this);
        dialog.setFileMode(QFileDialog::Directory);
        dialog.setOption(QFileDialog::ShowDirsOnly, true);
        dialog.setNameFilter("\"data\" directory (data)");
        QStringList fileNames;
        if ( dialog.exec() )
        {
            fileNames = dialog.selectedFiles();
            if ( !fileNames.isEmpty() )
            {
                QStringPath path = fileNames.first();
                QString last = path.lastPathComponent();
                if (last.compare("data", Qt::CaseSensitive) == 0)
                {
                    path = path.removeLastComponent();
                    if (app->SetFullDataFolderPath(path))
                    {
                        MainWindow::GetPropertyBrowser()->setEnabled(true);
                        MainWindow::GetGUIObjectsTreeWidget()->setEnabled(true);
                        this->ui->actionOpenViewController->setEnabled(true);
                        this->ui->actionSaveViewController->setEnabled(true);
                        this->ui->menuRootViewController->setEnabled(true);
                        _dataFullPath = fileNames.first();
                    }
                    else
                    {
                        QMessageBox::critical(this,
                                              tr("REGUI editor"),
                                              tr("Can't open directory\n") + path + tr("\n Make sure this directory is readable."),
                                              QMessageBox::Ok);
                    }
                }
                else
                {
                    QMessageBox::critical(this,
                                          tr("REGUI editor"),
                                          tr("You are select not \"data\" directory.\nPlease select or create \"data\" directory."),
                                          QMessageBox::Ok);
                }
            }
        }
    }
    else
    {
        QMessageBox::critical(this,
                              tr("REGUI editor"),
                              tr("Can't create main GUI application."),
                              QMessageBox::Ok);
    }

}

void MainWindow::ReloadSelectedObjectProperties()
{
    GUIObjectsTreeWidget * tree = MainWindow::GetGUIObjectsTreeWidget();
    if (tree)
        tree->ReloadSelectedObjectProperties();
}

void MainWindow::on_actionViewTextureSetNew_triggered()
{
    QREObjectCommon * obj = MainWindow::GetSelectedGUIObject();
    if (obj)
    {
        QREView * view = NULL;
        const QREGUIObjectType type = QREGUIObjectsHelper::GetType(obj, (void**)&view);
        if ((type == QREGUIObjectType_QREVIEW) && view)
        {
            if (view->SetNewTextureQ()) this->ReloadSelectedObjectProperties();
        }
    }
}

void MainWindow::on_actionViewTextureOpenImage_triggered()
{
    QREObjectCommon * obj = MainWindow::GetSelectedGUIObject();
    if (obj)
    {
        QREView * view = NULL;
        const QREGUIObjectType type = QREGUIObjectsHelper::GetType(obj, (void**)&view);
        if ((type == QREGUIObjectType_QREVIEW) && view)
        {
            if (view->OpenTextureImageQ()) this->ReloadSelectedObjectProperties();
        }
    }
}

void MainWindow::on_actionDefaultTextureSetNew_triggered()
{
    QREObjectCommon * obj = MainWindow::GetSelectedGUIObject();
    if (obj)
    {
        QREButton * button = NULL;
        const QREGUIObjectType type = QREGUIObjectsHelper::GetType(obj, (void**)&button);
        if ((type == QREGUIObjectType_QREBUTTON) && button)
        {
            if (button->SetNewDefaultTextureQ()) this->ReloadSelectedObjectProperties();
        }
    }
}

void MainWindow::on_actionDefaultTextureOpenImage_triggered()
{
    QREObjectCommon * obj = MainWindow::GetSelectedGUIObject();
    if (obj)
    {
        QREButton * button = NULL;
        const QREGUIObjectType type = QREGUIObjectsHelper::GetType(obj, (void**)&button);
        if ((type == QREGUIObjectType_QREBUTTON) && button)
        {
            if (button->OpenDefaultTextureImageQ()) this->ReloadSelectedObjectProperties();
        }
    }
}

void MainWindow::on_actionBtnPressedTextureSetNew_triggered()
{
    QREObjectCommon * obj = MainWindow::GetSelectedGUIObject();
    if (obj)
    {
        QREButton * button = NULL;
        const QREGUIObjectType type = QREGUIObjectsHelper::GetType(obj, (void**)&button);
        if ((type == QREGUIObjectType_QREBUTTON) && button)
        {
            if (button->SetNewPressedTextureQ()) this->ReloadSelectedObjectProperties();
        }
    }
}

void MainWindow::on_actionBtnPressedTextureOpenImage_triggered()
{
    QREObjectCommon * obj = MainWindow::GetSelectedGUIObject();
    if (obj)
    {
        QREButton * button = NULL;
        const QREGUIObjectType type = QREGUIObjectsHelper::GetType(obj, (void**)&button);
        if ((type == QREGUIObjectType_QREBUTTON) && button)
        {
            if (button->OpenPressedTextureImageQ()) this->ReloadSelectedObjectProperties();
        }
    }
}

void MainWindow::on_actionBtnDisabledTextureSetNew_triggered()
{
    QREObjectCommon * obj = MainWindow::GetSelectedGUIObject();
    if (obj)
    {
        QREButton * button = NULL;
        const QREGUIObjectType type = QREGUIObjectsHelper::GetType(obj, (void**)&button);
        if ((type == QREGUIObjectType_QREBUTTON) && button)
        {
            if (button->SetNewDisabledTextureQ()) this->ReloadSelectedObjectProperties();
        }
    }
}

void MainWindow::on_actionBtnDisabledTextureOpenImage_triggered()
{
    QREObjectCommon * obj = MainWindow::GetSelectedGUIObject();
    if (obj)
    {
        QREButton * button = NULL;
        const QREGUIObjectType type = QREGUIObjectsHelper::GetType(obj, (void**)&button);
        if ((type == QREGUIObjectType_QREBUTTON) && button)
        {
            if (button->OpenDisabledTextureImageQ())
            {
                this->ReloadSelectedObjectProperties();
            }
        }
    }
}

void MainWindow::on_actionLabelTextureSetNew_triggered()
{
    QREObjectCommon * obj = MainWindow::GetSelectedGUIObject();
    if (obj)
    {
        QRELabel * label = NULL;
        const QREGUIObjectType type = QREGUIObjectsHelper::GetType(obj, (void**)&label);
        if ((type == QREGUIObjectType_QRELABEL) && label)
        {
            if (label->SetNewTextureQ())
            {
                this->ReloadSelectedObjectProperties();
            }
        }
    }
}

void MainWindow::on_actionLabelTextureOpenImage_triggered()
{
    QREObjectCommon * obj = MainWindow::GetSelectedGUIObject();
    if (obj)
    {
        QRELabel * label = NULL;
        const QREGUIObjectType type = QREGUIObjectsHelper::GetType(obj, (void**)&label);
        if ((type == QREGUIObjectType_QRELABEL) && label)
        {
            if (label->OpenTextureImageQ())
            {
                this->ReloadSelectedObjectProperties();
            }
        }
    }
}

void MainWindow::on_actionLabelFontSetNew_triggered()
{
    QREObjectCommon * obj = MainWindow::GetSelectedGUIObject();
    if (obj)
    {
        QRELabel * label = NULL;
        const QREGUIObjectType type = QREGUIObjectsHelper::GetType(obj, (void**)&label);
        if ((type == QREGUIObjectType_QRELABEL) && label)
        {
            if (label->SetNewFontQ())
            {
                this->ReloadSelectedObjectProperties();
            }
        }
    }
}

void MainWindow::on_actionLabelFontOpenFont_triggered()
{
    QREObjectCommon * obj = MainWindow::GetSelectedGUIObject();
    if (obj)
    {
        QRELabel * label = NULL;
        const QREGUIObjectType type = QREGUIObjectsHelper::GetType(obj, (void**)&label);
        if ((type == QREGUIObjectType_QRELABEL) && label)
        {
            if (label->OpenFontFileQ())
            {
                this->ReloadSelectedObjectProperties();
            }
        }
    }
}

void MainWindow::on_actionTextFieldTextureSetNew_triggered()
{
    QREObjectCommon * obj = MainWindow::GetSelectedGUIObject();
    if (obj)
    {
        QRETextField * textField = NULL;
        const QREGUIObjectType type = QREGUIObjectsHelper::GetType(obj, (void**)&textField);
        if ((type == QREGUIObjectType_QRETEXTFIELD) && textField)
        {
            if (textField->SetNewTextureQ())
            {
                this->ReloadSelectedObjectProperties();
            }
        }
    }
}

void MainWindow::on_actionTextFieldTextureOpenImage_triggered()
{
    QREObjectCommon * obj = MainWindow::GetSelectedGUIObject();
    if (obj)
    {
        QRETextField * textField = NULL;
        const QREGUIObjectType type = QREGUIObjectsHelper::GetType(obj, (void**)&textField);
        if ((type == QREGUIObjectType_QRETEXTFIELD) && textField)
        {
            if (textField->OpenTextureImageQ())
            {
                this->ReloadSelectedObjectProperties();
            }
        }
    }
}

void MainWindow::on_actionTextFieldFontSetNew_triggered()
{
    QREObjectCommon * obj = MainWindow::GetSelectedGUIObject();
    if (obj)
    {
        QRETextField * textField = NULL;
        const QREGUIObjectType type = QREGUIObjectsHelper::GetType(obj, (void**)&textField);
        if ((type == QREGUIObjectType_QRETEXTFIELD) && textField)
        {
            if (textField->SetNewFontQ())
            {
                this->ReloadSelectedObjectProperties();
            }
        }
    }
}

void MainWindow::on_actionTextFieldFontOpenFont_triggered()
{
    QREObjectCommon * obj = MainWindow::GetSelectedGUIObject();
    if (obj)
    {
        QRETextField * textField = NULL;
        const QREGUIObjectType type = QREGUIObjectsHelper::GetType(obj, (void**)&textField);
        if ((type == QREGUIObjectType_QRETEXTFIELD) && textField)
        {
            if (textField->OpenFontFileQ())
            {
                this->ReloadSelectedObjectProperties();
            }
        }
    }
}

void MainWindow::on_actionPartViewOpenParticleFile_triggered()
{
    QREObjectCommon * obj = MainWindow::GetSelectedGUIObject();
    if (obj)
    {
        QREParticleView * particle = NULL;
        const QREGUIObjectType type = QREGUIObjectsHelper::GetType(obj, (void**)&particle);
        if ((type == QREGUIObjectType_QREPARTICLEVIEW) && particle)
        {
            if (particle->OpenParticleFileQ())
            {
                this->ReloadSelectedObjectProperties();
            }
        }
    }
}

void MainWindow::on_actionOpenViewController_triggered()
{
    OpenViewControllerDialog dialog(this);
    dialog.setModal(true);
    dialog.SetDataFullPath(_dataFullPath);
    dialog.exec();
}

void MainWindow::OpenViewController(const QString & name, const QString & fullXMLPath)
{
    GUIObjectsTreeWidget * tree = MainWindow::GetGUIObjectsTreeWidget();
    if (tree)
    {
        QREViewController * qvc = tree->GetRootVC();
        if (qvc)
        {
            REViewController * vc = qvc->reviewcontroller();
            if (vc)
            {
                vc->RemoveAllSubViews();
                REString reName(REQTConverter::GetREString(name));
                vc->LoadByName(reName, false);
                tree->ReloadRootVCTreeViews();

                MainWindow::OnRenderDeviceScreenSizeChanged();
                MainWindow::GetMainWindow()->OnScaleRendererValueChanged(100);
            }
        }
    }
}

RenderDialog * MainWindow::GetRenderDialog()
{
    MainWindow * mw = MainWindow::GetMainWindow();
    if (mw)
    {
        return mw->_renderDialog;
    }
    return NULL;
}

void MainWindow::OnRenderDeviceScreenSizeChanged()
{
    RenderDialog * renderer = MainWindow::GetRenderDialog();
    if (renderer)
    {
        renderer->OnRenderDeviceScreenSizeChanged();
    }
}

void MainWindow::OnScaleRendererValueChanged(int v)
{
    QString s(QString::number((int)v));
    s.append(" %");
    _scaleRendererAction->persentLabel()->setText(s);
    _renderDialog->SetScaleValue(0.01f * (REFloat32)v);
}

QSize MainWindow::GetRootViewControllerSizePX()
{
    GUIObjectsTreeWidget * tree = MainWindow::GetGUIObjectsTreeWidget();
    if (tree)
    {
        QREViewController * vc = tree->GetRootVC();
        if (vc)
        {
            return vc->GetSizePxQ();
        }
    }
    return QSize(0, 0);
}


void MainWindow::on_actionNewObjectView_triggered()
{
    GUIObjectsTreeWidget * tree = MainWindow::GetGUIObjectsTreeWidget();
    if (tree) tree->AddNewSubViewToSelected(QREGUIObjectType_QREVIEW);
}

void MainWindow::on_actionNewObjectButton_triggered()
{
    GUIObjectsTreeWidget * tree = MainWindow::GetGUIObjectsTreeWidget();
    if (tree) tree->AddNewSubViewToSelected(QREGUIObjectType_QREBUTTON);
}

void MainWindow::on_actionNewObjectLabel_triggered()
{
    GUIObjectsTreeWidget * tree = MainWindow::GetGUIObjectsTreeWidget();
    if (tree) tree->AddNewSubViewToSelected(QREGUIObjectType_QRELABEL);
}

void MainWindow::on_actionNewObjectTextField_triggered()
{
    GUIObjectsTreeWidget * tree = MainWindow::GetGUIObjectsTreeWidget();
    if (tree) tree->AddNewSubViewToSelected(QREGUIObjectType_QRETEXTFIELD);
}

void MainWindow::on_actionNewObjectParticleView_triggered()
{
    GUIObjectsTreeWidget * tree = MainWindow::GetGUIObjectsTreeWidget();
    if (tree) tree->AddNewSubViewToSelected(QREGUIObjectType_QREPARTICLEVIEW);
}

void MainWindow::on_actionRendererShowHide_triggered()
{
    if (_renderDialog->isHidden()) _renderDialog->show();
    else _renderDialog->hide();
}

void MainWindow::on_actionRootVCRenderSize_triggered()
{
    GUIObjectsTreeWidget * tree = MainWindow::GetGUIObjectsTreeWidget();
    if (tree == NULL) return;
    QREViewController * vc = tree->GetRootVC();
    if (vc == NULL) return;

    RESize s(RERenderDevice::GetDefaultDevice()->GetRenderSize());
    SizeDialog dialog(this);
    dialog.SetWidthFL(s.width);
    dialog.SetHeightFL(s.height);
    dialog.SetType(SizeDialogFL);
    dialog.exec();
    if (!dialog.IsCancelled())
    {
        s.width = dialog.GetWidthFL();
        s.height = dialog.GetHeightFL();

        QSizeF qs((qreal)s.width, (qreal)s.height);
        vc->SetSizeFlQ(qs);

        RERenderDevice::GetDefaultDevice()->SetRenderSize(s);
        MainWindow::OnRenderDeviceScreenSizeChanged();
        this->OnScaleRendererValueChanged(100);
    }
}

void MainWindow::on_actionRootVCScreenSize_triggered()
{
    GUIObjectsTreeWidget * tree = MainWindow::GetGUIObjectsTreeWidget();
    if (tree == NULL) return;
    QREViewController * vc = tree->GetRootVC();
    if (vc == NULL) return;
    RESize s(vc->GetSizePxQ().width(), vc->GetSizePxQ().height());
    SizeDialog dialog(this);
    dialog.SetWidthPX((int)s.width);
    dialog.SetHeightPX((int)s.height);
    dialog.SetType(SizeDialogPX);
    dialog.exec();
    if (!dialog.IsCancelled())
    {
        s.width = (REFloat32)dialog.GetWidthPX();
        s.height = (REFloat32)dialog.GetHeightPX();

        QSize qs((int)s.width, (int)s.height);
        vc->SetSizePxQ(qs);

        RERenderDevice::GetDefaultDevice()->SetScreenSize(s);
        MainWindow::OnRenderDeviceScreenSizeChanged();
        this->OnScaleRendererValueChanged(100);
    }
}

void MainWindow::on_actionRootVCXMLOutput_triggered()
{
    GUIObjectsTreeWidget * tree = MainWindow::GetGUIObjectsTreeWidget();
    if (tree == NULL) return;
    QREViewController * vc = tree->GetRootVC();
    if (vc == NULL) return;

    XMLOutPutDialog dialog(this);
    dialog.setModal(true);
    dialog.SetXMLString(vc->GetXMLString(QString("")));
    dialog.exec();
}

QREViewController * MainWindow::GetRootViewController()
{
    GUIObjectsTreeWidget * tree = MainWindow::GetGUIObjectsTreeWidget();
    if (tree)
    {
        return tree->GetRootVC();
    }
    return NULL;
}

void MainWindow::on_actionSaveViewController_triggered()
{
    SaveViewControllerDialog dialog(this);
    dialog.setModal(true);
    dialog.SetViewController(MainWindow::GetRootViewController());
    dialog.SetStoreDataPath(_dataFullPath);
    dialog.exec();
}

void MainWindow::on_actionSelectedObjectDelete_triggered()
{
    GUIObjectsTreeWidget * tree = MainWindow::GetGUIObjectsTreeWidget();
    if (tree)
    {
        tree->DeleteSelectedObject();
    }
}

void MainWindow::on_actionCpphelper_triggered()
{
    QREVCCPPHelper dialog(this);
    dialog.SetViewController(MainWindow::GetRootViewController());
    dialog.exec();
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dialog(this);
    dialog.exec();
}
