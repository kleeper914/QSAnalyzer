#ifndef DIGITALMODULATOR_H
#define DIGITALMODULATOR_H

#include <vector>
#include <complex>
#include <string>
#include <stdexcept>
#include <random>
#include <cmath>
#include <fstream>
#include <Utility/utility.h>

using std::vector;
using std::string;
using std::ofstream;

typedef std::complex<double> Complex;

class DigitalModulator
{
public:
    DigitalModulator(unsigned long long numBits, double symbolRate, double sampleRate, double rolloff = 0.5, int filterSpan = 6);
    virtual ~DigitalModulator() = default;

    //主流程: 生成信号(不可重写)
    void generate() {
        bits_ = generateBits();
        symbols_ = modulate(bits_);
        modulateSignal_ = pulseShaping(symbols_);
    }

    //公共方法, 获取数据
    const vector<int>& getBits() const {return bits_;}
    const vector<Complex>& getSymbols() const {return symbols_;}
    const vector<Complex>& getModulateSignal() const {return modulateSignal_;}

    //保存数据到文件
    void saveSymbolsToFile(const string& filename) const;
    void saveWaveformToFile(const string& filename) const;

protected:
    //派生类需要实现的调制方法
    virtual vector<Complex> modulate(const vector<int>& bits) = 0;

    //公共工具方法(派生类可重写)
    vector<int> generateBits(int bitsPerSymbol = 2);
    vector<double> designRrcFilter();
    vector<Complex> pulseShaping(const vector<Complex>& symbols);

protected:
    unsigned long long numBits_;       //总比特数
    double symbolRate_;                //符号率(符号/秒)
    double sampleRate_;                //采样率(Hz)
    int samplesPerSymbol_;             //每符号采样数
    double rolloff_;                   //升余弦滚降因子
    int filterSpan_;                   //滤波器跨度(符号数)

private:
    vector<int> bits_;              //原始比特流
    vector<Complex> symbols_;       //调制符号
    vector<Complex> modulateSignal_;     //成形后信号
};

#endif // DIGITALMODULATOR_H
