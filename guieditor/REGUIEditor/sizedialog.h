#ifndef SIZEDIALOG_H
#define SIZEDIALOG_H

#include <QDialog>
#include <recore.h>

typedef enum _sizeDialogType
{
    SizeDialogPX = 0,
    SizeDialogFL = 1
} SizeDialogType;

namespace Ui
{
    class SizeDialog;
}

class SizeDialog : public QDialog
{
    Q_OBJECT

protected:
virtual void showEvent(QShowEvent * event);

public:
    int GetWidthPX() const { return _widthPX; }
    int GetHeightPX() const { return _heightPX; }
    REFloat32 GetWidthFL() const { return _widthFL; }
    REFloat32 GetHeightFL() const { return _heightFL; }

    void SetWidthPX(int width);
    void SetHeightPX(int height);
    void SetWidthFL(REFloat32 width);
    void SetHeightFL(REFloat32 height);

    void SetType(SizeDialogType newType);
    SizeDialogType GetType() { return _type; }
    bool IsCancelled() { return _isCancelled; }

    explicit SizeDialog(QWidget *parent = 0);
    ~SizeDialog();

private:
    Ui::SizeDialog *ui;
    int _widthPX;
    int _heightPX;
    REFloat32 _widthFL;
    REFloat32 _heightFL;
    SizeDialogType _type;
    bool _isCancelled;

private slots:
    void on_exchangeToolButton_clicked();
    void on_heightSpinBoxFL_valueChanged(double newHeight);
    void on_widthSpinBoxFL_valueChanged(double newWidth);
    void on_heightSpinBox_valueChanged(int newHeight);
    void on_widthSpinBox_valueChanged(int newWidth);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // SIZEDIALOG_H
