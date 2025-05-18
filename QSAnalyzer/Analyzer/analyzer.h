#ifndef ANALYZER_H
#define ANALYZER_H

#include <vector>
#include <string>
#include <complex>
#include <FFTW3/fftw3.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <Utility/utility.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;
using std::istringstream;

typedef std::complex<double> Complex;

constexpr double MIN_DB_VALUE = -300.0;     //最小dB值(避免负无穷)

class Analyzer
{
public:
    Analyzer(const vector<double>& signal, double sampleRate, int fftSize = 1024);
    Analyzer(const vector<Complex>& signal, double sampleRate, int fftSize = 1024);
    Analyzer(const string& filename, double sampleRate, int fftSize = 1024);
    ~Analyzer();
    //get方法
    bool getIsComplex(void) const { return isComplex_; }
    double getSampleRate(void) const { return sampleRate_; }
    const vector<double>& getSignal(void) const;
    const vector<Complex>& getComplexSignal(void) const;
    const vector<double>& getFrequencyAxis(void) const { return frequencyAxis_; }
    const vector<double>& getShiftFrequencyAxis(void) const { return shiftFrequencyAxis_; }
    //基础分析
    void timeDomainAnalysis(double& mean, double& variance, double& rms) const;
    void timeDomainAnalysis(Complex& mean, double& power, double& rms) const;
    void frequencyDomainAnalysis(vector<double>& mag, vector<double>& magDB, vector<double>& psd, vector<double>& psdDB, double refLevel = 1.0) const;
    //高级分析
    vector<vector<double>> computeSpectrogram(int windowSize = 256, int overlap = 128) const;
    //数据读取
    static vector<double> loadSignalFromFile(const string& filename);
    static vector<Complex> loadComplexSignalFromFile(const string& filename);
    //数据保存
    static void saveSpectrogram(const string& filename, const vector<vector<double>>& spectrogram);
    static void saveSpectrum(const string& filename, const vector<double>& freq, const vector<double>& spectrum);

private:
    void initFFTW();
    bool detectComplexSignal(const string& filename);
    void generateFrequencyAxis();
    void applyHammingWindow(vector<double>& data) const;
    void applyHammingWindow(fftw_complex* data, int size) const;
    double calculateWindowEnergy() const;

private:
    vector<double> signal_;     //实数信号
    vector<Complex> complexSignal_;     //复数信号
    double sampleRate_;          //采样率(Hz)
    int fftSize_;                //FFT长度
    bool isComplex_;
    vector<double> frequencyAxis_;  //频率轴
    vector<double> shiftFrequencyAxis_;     //fftshift后的频率轴
    //fftw3相关资源
    fftw_plan fftPlan_;
    double* fftInput_;           //FFTW输入
    fftw_complex* fftOutput_;    //FFTW输出
};

#endif // ANALYZER_H
