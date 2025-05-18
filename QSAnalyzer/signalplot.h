#ifndef SIGNALPLOT_H
#define SIGNALPLOT_H

#include <QWidget>
#include <QVBoxLayout>
#include <QVector>
#include "qcustomplot.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QString>
#include <vector>
#include <complex>
#include <cmath>

using std::vector;
using std::string;

typedef std::complex<double> Complex;

class SignalPlot : public QDialog
{
    Q_OBJECT
public:
    enum PlotType {
        TimeDomain,
        Amplitude,
        PSD
    };

    struct SpectrumData {
        vector<double> freq;
        vector<double> magLinear;
        vector<double> magDB;
        vector<double> psdLinear;
        vector<double> psdDB;
    }spectrumCache;

    explicit SignalPlot(QWidget *parent = nullptr);
    ~SignalPlot();

    void plotTimeDomain(double sampleRate, const vector<double>& real, const vector<double>& imag = {});
    void plotAmplitudeSpectrum(bool isLogScale = false, bool isDB = false);
    void plotPSD(bool isLogScale = false, bool isDB = false);

private:
    void initPlot(const QString& xLabel, const QString& yLabel);
    void clearPlot();
    void setSpectrumAxes(bool isLogScale);
    void setPSDAxes(bool isLogScale);
    void replotCurrentGraph();

private slots:
    void showContextMenu(const QPoint& pos);
    void toggleXLogScale(bool checked);
    void toggleYdBUnit(bool checked);

private:
    QCustomPlot* m_plot;
    QVBoxLayout* layout;
    Qt::Orientations m_zoomDirection;
    PlotType currentPlotType;
    bool xLogScale;
    bool yDBUnit;
};

#endif // SIGNALPLOT_H
