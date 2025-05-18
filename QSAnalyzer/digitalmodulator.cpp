#include "digitalmodulator.h"

DigitalModulator::DigitalModulator(unsigned long long numBits, double symbolRate, double sampleRate, double rolloff, int filterSpan)
    : numBits_(numBits), symbolRate_(symbolRate), sampleRate_(sampleRate),
    rolloff_(rolloff), filterSpan_(filterSpan)
{
    //计算每符号采样数, 并确保为整型
    samplesPerSymbol_ = static_cast<int>(sampleRate_ / symbolRate_);
    if(abs(samplesPerSymbol_ * symbolRate_ - sampleRate_) > 1e-6) {
        throw std::invalid_argument("Sample Rate must be an integer multiple of the symbol rate!");
    }
}

/*
@brief: 生成随机比特流
@param: bitsPerSymbol - 每符号比特数
@return: 二进制比特流
@throw invalid_argument 如果比特数不能被bitsPerSymbol整除
*/
vector<int> DigitalModulator::generateBits(int bitsPerSymbol) {
    if(numBits_ % bitsPerSymbol != 0) {
        throw std::invalid_argument("比特数必须能被每符号比特数整除!");
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    vector<int> bits(numBits_);
    for(int i = 0; i < numBits_; i++) {
        bits[i] = dis(gen);
    }
    return bits;
}

/*
 * 参考 https://en.wikipedia.org/wiki/Root-raised-cosine_filter
@brief: 设计升余弦滤波器脉冲响应系数
@return 滤波器系数(归一化后)
*/
vector<double> DigitalModulator::designRrcFilter() {
    int length = filterSpan_ * samplesPerSymbol_;
    vector<double> filter(length);

    double T = 1.0;     //符号间隔归一化
    double beta = rolloff_;
    int mid = length / 2;

    for(int n = 0; n < length; n++) {
        double t = (n - mid) / static_cast<double>(samplesPerSymbol_);
        double pi_t = PI * t;

        if(abs(t) < 1e-8) {
            // t == 0
            filter[n] = (1.0 + beta * (4.0 / PI - 1.0));
        }
        else if(abs(abs(t) - T / (4.0 * beta)) < 1e-8) {
            // t = ±T/(4β)
            double a = (1.0 + 2.0 / PI) * sin(PI / (4.0 * beta));
            double b = (1.0 - 2.0 / PI) * cos(PI / (4.0 * beta));
            filter[n] = (beta / sqrt(2.0)) * (a + b);
        }
        else {
            double numerator = sin((pi_t / T) * (1.0 - beta)) + 4.0 * beta * t / T * cos((pi_t / T) * (1.0 + beta));
            double denominator = (pi_t / T) * (1.0 - pow(4.0 * beta * t / T, 2.0));
            filter[n] = numerator / denominator;
        }
    }
    //归一化
    double sum = 0.0;
    for(double val : filter) {
        sum += val * val;
    }
    double norm = sqrt(sum);
    for(double& val : filter) {
        val /= norm;
    }
    return filter;
}

/*
@brief: 脉冲成形(上采样 + 滤波)
@param: symbols - 输入的复数符号序列
@return: 成形后的复数波形
*/
vector<Complex> DigitalModulator::pulseShaping(const vector<Complex>& symbols) {
    //上采样, 在每个符号之间插入samplesPerSymbol-1个0
    vector<Complex> upsampled(symbols.size() * samplesPerSymbol_, Complex(0, 0));
    for(size_t i = 0; i < symbols.size(); i++) {
        upsampled[i * samplesPerSymbol_] = symbols[i];
    }
    vector<double> filter = designRrcFilter();
    vector<Complex> output(upsampled.size() + filter.size() - 1, Complex(0, 0));
    //在线性时不变系统中进行离散时间信号卷积
    for(size_t i = 0; i < upsampled.size(); i++) {
        for(size_t j = 0; j < filter.size(); j++) {
            output[i + j] += upsampled[i] * filter[j];
        }
    }
    return output;
}

/*
@brief: 保存符号数据到csv
*/
void DigitalModulator::saveSymbolsToFile(const string &filename) const {
    ofstream file(filename);
    file << "real,imag\n";
    for(const Complex& val : symbols_) {
        file << val.real() << "," << val.imag() << "\n";
    }
    file.close();
}

/*
@brief: 保存调制信号数据到csv
*/
void DigitalModulator::saveWaveformToFile(const string &filename) const {
    ofstream file(filename);
    file << "real,imag\n";
    for(const Complex& val : modulateSignal_) {
        file << val.real() << "," << val.imag() << "\n";
    }
    file.close();
}
