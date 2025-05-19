#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget), analyzer_(nullptr)
{
    ui->setupUi(this);
    initWindow();
}

Widget::~Widget()
{
    delete ui;
    delete analyzer_;
}

/*
@brief: 窗口的初始化操作
*/
void Widget::initWindow(void) {
    //程序命名
    this->setWindowTitle("QSAnalyzer");

    //设置ui窗口居中

}

/*
@brief: OpenFile点击按键事件, 用户选择信号文件, 指定采样率和fft点数, 读取信号并自动计算均值、方差(功率)、RMS
*/
void Widget::on_Open_File_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, "select signal file", "", "Signal Files (*.txt *.csv *.wav);;All Files (*.*)"
    );
    if(!filePath.isEmpty()) {
        ui->filePath->setText(filePath);
        cout << filePath.toStdString() << endl;

        //弹出对话框, 用户输入采样率和fft点数
        QDialog* dialog = new QDialog(this);
        dialog->setWindowTitle("Enter Sample Rate and FFT Points");

        QFormLayout* formlayout = new QFormLayout;
        QLabel* labelSampleRate = new QLabel("Sample Rate(Hz):");
        QLineEdit* lineEditSampleRate = new QLineEdit;
        QLabel* labelFFTSize = new QLabel("FFT Size:");
        QLineEdit* lineEditFFTSize = new QLineEdit;
        lineEditFFTSize->setText("1024");
        formlayout->addRow(labelSampleRate, lineEditSampleRate);
        formlayout->addRow(labelFFTSize, lineEditFFTSize);

        QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(buttonBox, &QDialogButtonBox::accepted, dialog, &QDialog::accept);
        connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

        QVBoxLayout* mainLayout = new QVBoxLayout;
        mainLayout->addLayout(formlayout);
        mainLayout->addWidget(buttonBox);
        dialog->setLayout(mainLayout);

        if(dialog->exec() == QDialog::Accepted) {
            bool okSampleRate, okFFTSize;
            int sampleRate = lineEditSampleRate->text().toInt(&okSampleRate);
            int fftSize = lineEditFFTSize->text().toInt(&okFFTSize);
            if(!okSampleRate) {
                QMessageBox::warning(this, "Invalid Input", "Please enter a valid integer for Sample Rate.");
                return;
            }
            //用户未填写fftSize, 默认1024
            if(!okFFTSize) {
                fftSize = 1024;
            }
            cout << "SampleRate: " << sampleRate << " Hz   fftSize: " << fftSize << endl;

            //释放旧analyzer
            delete analyzer_;
            analyzer_ = nullptr;

            analyzer_ = new Analyzer(filePath.toStdString(), sampleRate, fftSize);
            //计算并显示均值、方差(功率)、RMS信息
            bool isComplex = analyzer_->getIsComplex();
            if(isComplex) {
                //cout << "Complex signal" << endl;
                Complex mean;
                double power = 0.0, rms = 0.0;
                analyzer_->timeDomainAnalysis(mean, power, rms);
                ui->meanLineEdit->setText(complexToQString(mean));
                ui->varianceLineEdit->setText(QString::number(power));
                ui->rmsLineEdit->setText(QString::number(rms));
            }
            else {
                cout << "Real signal" << endl;
                double mean = 0.0, variance = 0.0, rms = 0.0;
                analyzer_->timeDomainAnalysis(mean, variance, rms);
                ui->meanLineEdit->setText(QString::number(mean));
                ui->varianceLineEdit->setText(QString::number(variance));
                ui->rmsLineEdit->setText(QString::number(rms));
            }
        }
    }
}

/*
@brief: Waveform按键点击事件, 绘制信号时域波形图
@throw QMessageBox 如果未加载信号文件则弹出警告
*/
void Widget::on_Waveform_clicked()
{
    if(!analyzer_) {
        QMessageBox::warning(this, "Warning", "Please open and load the signal file first");
        return;
    }

    try {
        if(analyzer_->getIsComplex()) {
            const vector<Complex>& signal = analyzer_->getComplexSignal();
            int N = signal.size();
            vector<double> real(N, 0.0);
            vector<double> imag(N, 0.0);
            extractRealAndImg(signal, real, imag);
            double sampleRate = analyzer_->getSampleRate();

            SignalPlot* plotter = new SignalPlot(this);
            plotter->setAttribute(Qt::WA_DeleteOnClose);
            plotter->plotTimeDomain(sampleRate, real, imag);
            plotter->exec();
        }
        else {
            const vector<double>& signal = analyzer_->getSignal();
            int N = signal.size();
        }
    }
    catch(const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void Widget::on_Amplitude_Spectrum_clicked()
{
    if(!analyzer_) {
        QMessageBox::warning(this, "Warning", "Please open and load the signal file first");
        return;
    }

    try {
        SignalPlot* plotter = new SignalPlot(this);
        analyzer_->frequencyDomainAnalysis(
            plotter->spectrumCache.magLinear,
            plotter->spectrumCache.magDB,
            plotter->spectrumCache.psdLinear,
            plotter->spectrumCache.psdDB
        );
        plotter->spectrumCache.freq = analyzer_->getFrequencyAxis();
        plotter->spectrumCache.shiftFreq = analyzer_->getShiftFrequencyAxis();
        plotter->plotAmplitudeSpectrum(false, false);
        plotter->exec();
    }
    catch(const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void Widget::on_Power_Spectrum_clicked()
{
    if(!analyzer_) {
        QMessageBox::warning(this, "Warning", "Please open and load the signal file first");
        return;
    }

    try {
        SignalPlot* plotter = new SignalPlot(this);
        analyzer_->frequencyDomainAnalysis(
            plotter->spectrumCache.magLinear,
            plotter->spectrumCache.magDB,
            plotter->spectrumCache.psdLinear,
            plotter->spectrumCache.psdDB
            );
        plotter->spectrumCache.freq = analyzer_->getFrequencyAxis();
        plotter->spectrumCache.shiftFreq = analyzer_->getShiftFrequencyAxis();
        plotter->plotPSD(false, false);
        plotter->exec();
    }
    catch(const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}


void Widget::on_Constellation_Diagram_clicked()
{
    if(!analyzer_) {
        QMessageBox::warning(this, "Warning", "Please open and load the signal file first");
        return;
    }
    if(!analyzer_->getIsComplex()) {
        QMessageBox::warning(this, "Warning", "Only Complex Signal can plot constellation diagram");
        return;
    }

    try {
        const vector<Complex>& signal = analyzer_->getComplexSignal();
        const int N = static_cast<int>(signal.size());
        vector<pair<double, double>> iqPoints(N);
        vector<double> phase(N);
        vector<double> magnitude(N);
        analyzer_->constellationAnalysis(iqPoints, phase, magnitude);
        SignalPlot* plotter = new SignalPlot(this);
        plotter->plotConstellationDiagram(iqPoints);
        plotter->exec();
    }
    catch(const std::exception& e) {
        QMessageBox::critical(this, "Error", e.what());
    }
}

