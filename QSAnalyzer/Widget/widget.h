#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QScreen>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <Analyzer/analyzer.h>
#include <SignalPlot/signalplot.h>
#include <Utility/utility.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_Open_File_clicked();

    void on_Waveform_clicked();

    void on_Amplitude_Spectrum_clicked();

    void on_Power_Spectrum_clicked();

private:
    Ui::Widget *ui;
    Analyzer* analyzer_;

private:
    void initWindow(void);
};
#endif // WIDGET_H
