
#ifndef RENDERDIALOG_H
#define RENDERDIALOG_H

#include <QDialog>
#include "glrenderer.h"

namespace Ui
{
class RenderDialog;
}

class RenderDialog : public QDialog
{
    Q_OBJECT
    
protected:
    REFloat32 _scale;
    void UpdateGeometry();
public:
    GLRenderer * GetRenderer();
    void OnRenderDeviceScreenSizeChanged();
    void SetScaleValue(REFloat32 v);

    explicit RenderDialog(QWidget *parent = 0);
    ~RenderDialog();
    
private:
    Ui::RenderDialog *ui;

private slots:
    void UpdateRenderer();
};

#endif // RENDERDIALOG_H

