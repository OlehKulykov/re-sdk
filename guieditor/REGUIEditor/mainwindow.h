
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "renderdialog.h"
#include "QtTreePropertyBrowser"
#include "guiobjectstreeitem.h"
#include "qreguiobjectshelper.h"
#include "openviewcontrollerdialog.h"
#include "qstringpath.h"
#include "sizedialog.h"

namespace Ui
{
class MainWindow;
}

class RendererScaleSliderAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    QStringPath _dataFullPath;
    static MainWindow * _mainWindow;
    void ReloadSelectedObjectProperties();

public:
    static MainWindow * GetMainWindow();
    static QtTreePropertyBrowser * GetPropertyBrowser();
    static GUIObjectsTreeWidget * GetGUIObjectsTreeWidget();
    static GUIObjectsTreeItem * GetSelectedTreeItem();
    static QREObjectCommon * GetSelectedGUIObject();
    static QREViewController * GetRootViewController();
    static QREGUIObjectType GetSelectedGUIObjectType();
    static void OnSelectedNewTreeObject();
    static void PropertyCustomContextMenuRequested(QtProperty * property, const QPoint & pos);
    static void OpenViewController(const QString & name, const QString & fullXMLPath);
    static void OnRenderDeviceScreenSizeChanged();
    static RenderDialog * GetRenderDialog();
    static QSize GetRootViewControllerSizePX();
    static QString WidgetStyleSheetFontWithSize(int size);
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_actionOpen_data_folder_triggered();
    void on_actionViewTextureSetNew_triggered();
    void on_actionViewTextureOpenImage_triggered();
    void on_actionDefaultTextureSetNew_triggered();
    void on_actionDefaultTextureOpenImage_triggered();
    void on_actionBtnPressedTextureSetNew_triggered();
    void on_actionBtnPressedTextureOpenImage_triggered();
    void on_actionBtnDisabledTextureSetNew_triggered();
    void on_actionBtnDisabledTextureOpenImage_triggered();
    void on_actionLabelTextureSetNew_triggered();
    void on_actionLabelTextureOpenImage_triggered();
    void on_actionLabelFontSetNew_triggered();
    void on_actionLabelFontOpenFont_triggered();
    void on_actionTextFieldTextureSetNew_triggered();
    void on_actionTextFieldTextureOpenImage_triggered();
    void on_actionTextFieldFontSetNew_triggered();
    void on_actionTextFieldFontOpenFont_triggered();
    void on_actionPartViewOpenParticleFile_triggered();
    void on_actionOpenViewController_triggered();
    void OnScaleRendererValueChanged(int v);
    void on_actionNewObjectView_triggered();
    void on_actionNewObjectButton_triggered();
    void on_actionNewObjectLabel_triggered();
    void on_actionNewObjectTextField_triggered();
    void on_actionNewObjectParticleView_triggered();
    void on_actionRendererShowHide_triggered();
    void on_actionRootVCRenderSize_triggered();
    void on_actionRootVCScreenSize_triggered();
    void on_actionRootVCXMLOutput_triggered();
    void on_actionSaveViewController_triggered();
    void on_actionSelectedObjectDelete_triggered();
    void on_actionCpphelper_triggered();
    void on_actionAbout_triggered();

private:
    RenderDialog * _renderDialog;
    RendererScaleSliderAction * _scaleRendererAction;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

