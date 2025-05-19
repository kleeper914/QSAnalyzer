#include "analyzer.h"

/*
@brief: 从内存数据构造Analyzer
@param: signal - 输入信号(实数)
@param: sampleRate - 采样率(Hz)
@param: fftSize - FFT长度(默认1024, 需为2的幂)
*/
Analyzer::Analyzer(const vector<double> &signal, double sampleRate, int fftSize)
    : signal_(signal), sampleRate_(sampleRate), fftSize_(fftSize), isComplex_(false)
{
    if(fftSize <= 0 || (fftSize & (fftSize - 1))) {
        throw std::invalid_argument("The FFT length must be a power of 2");
    }
    initFFTW();
}

/*
@brief: 从内存数据构造Analyzer
@param: signal - 输入信号(复数)
@param: sampleRate - 采样率(Hz)
@param: fftSize - FFT长度(默认1024, 需为2的幂)
*/
Analyzer::Analyzer(const vector<Complex> &signal, double sampleRate, int fftSize)
    : complexSignal_(signal), sampleRate_(sampleRate), fftSize_(fftSize), isComplex_(true)
{
    if(fftSize <= 0 || (fftSize & (fftSize - 1))) {
        throw std::invalid_argument("The FFT length must be a power of 2");
    }
    initFFTW();
}

/*
@brief: 从文件构造Analyzer
@param: filename - 输入文件名
@param: sampleRate - 采样率(Hz)
@param: fftSize - FFT长度(默认1024, 需为2的幂)
*/
Analyzer::Analyzer(const string &filename, double sampleRate, int fftSize)
    : sampleRate_(sampleRate), fftSize_(fftSize)
{
    if(fftSize < 0 || (fftSize & (fftSize - 1))) {
        throw std::invalid_argument("The FFT length must be a power of 2");
    }
    isComplex_ = detectComplexSignal(filename); //检查是复数还是实数信号
    if(isComplex_) {
        complexSignal_ = loadComplexSignalFromFile(filename);
    }
    else {
        signal_ = loadSignalFromFile(filename);
    }
    initFFTW();
}

/*
@brief: 析构函数, 释放FFTW资源
*/
Analyzer::~Analyzer() {
    fftw_destroy_plan(fftPlan_);
    fftw_free(fftInput_);
    fftw_free(fftOutput_);
}

/*
@brief: 返回实数信号
@return: 实数信号的常量引用
@throw runtime_error: 信号为空, 数据未加载
*/
const vector<double>& Analyzer::getSignal() const {
    if(signal_.empty()) {
        throw std::runtime_error("Signal data is not loaded, please call the load method first");
    }
    return signal_;
}

/*
@brief: 返回复数信号
@return: 复数信号的常量引用
@throw runtime_error: 信号为空, 数据未加载
*/
const vector<Complex>& Analyzer::getComplexSignal() const {
    if(complexSignal_.empty()) {
        throw std::runtime_error("Signal data is not loaded, please call the load method first");
    }
    return complexSignal_;
}

/*
@brief: 检测信号文件是实数还是复数
@param: filename - 文件路径
@throws runtime_error 连续两次没有检测到数字
*/
bool Analyzer::detectComplexSignal(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件: " + filename);
    }
    string line;
    int validLines = 0;
    bool isComplex = false;
    const int maxAttempts = 2;  //最大尝试行数

    auto isNumber = [](const string& s) {
        istringstream iss(s);
        double val;
        iss >> val;
        return !iss.fail() && iss.eof();
    };
    while(getline(file, line) && validLines < maxAttempts) {
        //跳过空行和注释
        line.erase(0, line.find_first_not_of(" \t"));   //跳过行开头的空格和制表符
        if(line.empty() || line[0] == '#') continue;
        //替换可能的分隔符(逗号/空格/tab)为空格
        replace_if(line.begin(), line.end(),
            [](char c) { return c == ',' || c == '\t'; }, ' ');
        istringstream iss(line);
        string part1, part2;

        if(iss >> part1) {
            if(iss >> part2) {
                //检测两部分是否均为数字
                if(isNumber(part1) && isNumber(part2)) {
                    isComplex = true;
                    validLines++;
                }
            }
            else {
                //只有第一部分是数字
                if(isNumber(part1)) {
                    //若之前已经检测到复数行
                    if(isComplex) {
                        throw std::runtime_error("Inconsistent file format: mixed real/complex data");
                    }
                    validLines++;
                }
            }
        }
    }
    if(validLines == 0) {
        throw std::runtime_error("The file has no valid data lines: " + filename);
    }

    return isComplex;
}

/*
@brief: 生成频率轴数据
*/
void Analyzer::generateFrequencyAxis() {
    const int N = fftSize_;
    const double df = sampleRate_ / N;  //频率分辨率

    if(isComplex_) {
        //复数信号: 0 ~ sampleRate_
        frequencyAxis_.resize(N);
        shiftFrequencyAxis_.resize(N);
        for(int i = 0; i < N; i++) {
            frequencyAxis_[i] = i * df;
            shiftFrequencyAxis_[i] = (i - N/2) * df;
        }
    }
    else {
        //实数信号: 0 ~ sampleRate_/2(共N/2+1个点)
        frequencyAxis_.resize(N / 2 + 1);
        for(int i = 0; i < N / 2 + 1; i++) {
            frequencyAxis_[i] = i * df;
        }
    }
}

/*
@brief: 应用汉明窗(原地修改输入数据)(窗函数参考https://en.wikipedia.org/wiki/Window_function)
@param: data - 输入实数信号
*/
void Analyzer::applyHammingWindow(vector<double> &data) const {
    const int N = data.size();
    for(int i = 0; i < N; i++) {
        data[i] *= 0.54 - 0.46 * cos(2 * PI * i / (N - 1));
    }
}

/*
@brief: 应用汉明窗(原地修改输入数据)
@param: data - 输入复数信号
@param: size - 复数信号点数
*/
void Analyzer::applyHammingWindow(fftw_complex *data, int size) const {
    for(int i = 0; i < size; i++) {
        const double w = 0.54 - 0.46 * cos(2 * PI * i / (size - 1));
        data[i][0] *= w;
        data[i][1] *= w;
    }
}

/*
@brief: 计算窗函数能量, 用于功率谱归一化
@return: Hamming窗的能量值
@note: 对FFT输出做功率归一化时使用(防止窗引入幅值失真)
*/
double Analyzer::calculateWindowEnergy() const {
    const int N = fftSize_;
    double energy = 0.0;
    for(int i = 0; i < N; i++) {
        const double w = 0.54 - 0.46 * cos(2 * PI * i / (N - 1));
        energy += w * w;
    }
    return energy;
}

/*
@brief: 初始化fftw3资源
*/
void Analyzer::initFFTW() {
    //实数信号
    if(!isComplex_) {
        fftInput_ = (double*)fftw_malloc(sizeof(double) * fftSize_);
        fftOutput_ = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * (fftSize_ / 2 + 1));
        fftPlan_ = fftw_plan_dft_r2c_1d(fftSize_, fftInput_, fftOutput_, FFTW_ESTIMATE);
    }
    else {
        fftInput_ = nullptr;
        fftOutput_ = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * fftSize_);
        fftPlan_ = fftw_plan_dft_1d(fftSize_, fftOutput_, fftOutput_, FFTW_FORWARD, FFTW_ESTIMATE);
    }
    //生成频率轴
    generateFrequencyAxis();
}

/*
@brief: 从文件加载实数信号数据(每行一个实数)
@param: filename - 输入信号名
@return: 信号数据(vector<double>)
@throws runtime_error 文件无法打开或格式错误
*/
vector<double> Analyzer::loadSignalFromFile(const string &filename) {
    ifstream file(filename);
    if(!file.is_open()) {
        throw std::runtime_error("Can't open file: " + filename);
    }
    vector<double> signal;
    string line;
    int lineNum = 0;
    bool hasHeader = false;

    while (getline(file, line)) {
        lineNum++;

        //移出行首行尾空白字符
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t")+1);
        //跳过空行和注释
        if(line.empty() || line[0] == '#') continue;
        //检查是否是表头(首行且不含数字)
        if((lineNum == 1) && !containsNumber(line)) {
            hasHeader = true;
            //cout << "Detect table header and skip the first row." << endl;
            continue;
        }

        istringstream iss(line);
        double value;
        if(iss >> value) {
            //检查是否还有额外数据
            string remaining;
            if(iss >> remaining) {
                throw std::runtime_error("line " + std::to_string(lineNum) + " contains multiple values: " + line);
            }
            signal.push_back(value);
        }
        else {
            throw std::runtime_error("Wrong file format(line " + std::to_string(lineNum) + " ): " + line);
        }
    }

    if(signal.empty()) {
        throw std::runtime_error("Invalid file data: " + filename);
    }

    return signal;
}

/*
@brief: 从文件加载复数信号数据(每行一个复数, 第一列为实部, 第二列为虚部)
@param: filename - 读取文件路径
@return: 信号数据(vector<Complex>)
@throws runtime_error 文件无法打开或格式错误
*/
vector<Complex> Analyzer::loadComplexSignalFromFile(const string &filename) {
    ifstream file(filename);
    if(!file.is_open()) {
        throw std::runtime_error("Can't open file: " + filename);
    }
    vector<Complex> signal;
    string line;
    int lineNum = 0;
    bool hasHeader = false;
    const string delimeters = " ,\t";   //支持空格/逗号/tab分隔

    while(getline(file, line)) {
        lineNum++;

        //移出行首行尾空白字符
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);
        //跳过空行和注释
        if(line.empty() || line[0] == '#') continue;
        //检查是否为表头(首行且不含复数)
        if(lineNum == 1 && !isComplexLine(line)) {
            hasHeader = true;
            cout << "Detect table header and skip the first row." << endl;
            continue;
        }
        //替换分隔符统一为空格
        for(char& c : line) {
            if(delimeters.find(c) != string::npos) {
                c = ' ';
            }
        }

        istringstream iss(line);
        double re, im;
        if(iss >> re >> im) {
            //检查是否有多余数据
            string remaining;
            if(iss >> remaining) {
                throw std::runtime_error("line " + std::to_string(lineNum) + " contains multiple values: " + line);
            }
            signal.emplace_back(re, im);
        }
        else {
            throw std::runtime_error("Wrong file format(line " + std::to_string(lineNum) + " ): " + line);
        }
    }
    if(signal.empty()) {
        throw std::runtime_error("Invalid file data: " + filename);
    }

    return signal;
}

/*
@brief: 时域分析(均值、方差、RMS)
@param[out]: mean - 信号均值
@param[out]: variance - 信号方差
@param[out]: rms - 信号有效值(RMS)
*/
void Analyzer::timeDomainAnalysis(double &mean, double &variance, double &rms) const {
    if(isComplex_) return;
    mean = accumulate(signal_.begin(), signal_.end(), 0.0) / signal_.size();
    variance = 0.0;
    for(double v : signal_) {
        variance += (v - mean) * (v - mean);
    }
    variance /= signal_.size();
    rms = sqrt(accumulate(signal_.begin(), signal_.end(), 0.0,
                          [](double sum, double val) { return sum + val * val; }) / signal_.size());
}

/*
@brief: 时域分析(均值、功率、RMS)
@param[out]: mean - 信号均值
@param[out]: power - 信号功率
@param[out]: rms - 信号有效值(RMS)
*/
void Analyzer::timeDomainAnalysis(Complex& mean, double &power, double &rms) const {
    if(!isComplex_) return;
    Complex sum(0, 0);
    double energy = 0.0;
    size_t size = complexSignal_.size();
    for(const auto& v : complexSignal_) {
        sum += v;
        energy += norm(v);
    }
    mean = sum / static_cast<double>(size);
    power = energy / static_cast<double>(size);
    rms = sqrt(power);
}

/*
@brief: 频域分析(幅度谱和功率谱密度)
@param: mag - 幅度谱
@param: psd - 功率谱密度
@param: refLevel - 参考电平(用于dB标准化)
@note: 支持实信号(仅正频率)和复信号(全频率)两种情况
*/
void Analyzer::frequencyDomainAnalysis(vector<double> &mag, vector<double>& magDB, vector<double> &psd, vector<double>& psdDB, double refLevel) const {
    const int N = fftSize_;
    const double scaleFactor = 1.0 / (calculateWindowEnergy() * sampleRate_);

    auto computeLinear = [&](auto& spectrum, auto& PSD) {
        mag.resize(spectrum.size());
        psd.resize(PSD.size());
        std::copy(spectrum.begin(), spectrum.end(), mag.begin());
        std::copy(PSD.begin(), PSD.end(), psd.begin());
    };

    //复数信号处理
    if(isComplex_) {
        vector<double> spectrum(N), PSD(N);
        for(int i = 0; i < N; i++) {
            if(i < complexSignal_.size()) {
                fftOutput_[i][0] = complexSignal_[i].real();
                fftOutput_[i][1] = complexSignal_[i].imag();
            }
            else {
                fftOutput_[i][0] = 0.0;
                fftOutput_[i][1] = 0.0;
            }
        }
        //加窗
        applyHammingWindow(fftOutput_, N);
        //执行FFT
        fftw_execute(fftPlan_);
        //计算全频谱
        spectrum.resize(N);
        PSD.resize(N);
        for(int i = 0; i < N; i++) {
            const double re = fftOutput_[i][0];
            const double im = fftOutput_[i][1];
            spectrum[i] = sqrt(re*re + im*im) / N;
            PSD[i] = (re*re + im*im) * scaleFactor;
        }
        computeLinear(spectrum, PSD);
    }
    //实数信号处理
    else {
        vector<double> spectrum(N/2+1), PSD(N/2+1);
        vector<double> windowed(N, 0.0);
        const int copyLen = std::min(N, static_cast<int>(signal_.size()));
        //加窗
        for(int i = 0; i < copyLen; i++) {
            windowed[i] = signal_[i];
        }
        applyHammingWindow(windowed);
        //执行FFT
        memcpy(fftInput_, windowed.data(), sizeof(double)*N);
        fftw_execute(fftPlan_);
        //计算频谱(仅正频率)
        const int numBins = N/2 + 1;
        spectrum.resize(numBins);
        PSD.resize(numBins);
        for(int i = 0; i < numBins; i++) {
            const double re = fftOutput_[i][0];
            const double im = fftOutput_[i][1];
            spectrum[i] = sqrt(re*re + im*im) / N;
            PSD[i] = (re*re + im*im) * scaleFactor;
        }
        computeLinear(spectrum, PSD);
    }

    //计算dB数据
    const double refSquare = refLevel * refLevel;
    magDB.resize(mag.size());
    psdDB.resize(psd.size());

    auto safeLog10 = [](double val, double ref, bool isPower) {
        const double epsilon = 1e-20;
        double ratio = val / ref + epsilon;
        return isPower ? 10.0 * log10(ratio) : 20.0 * log10(ratio);
    };

    for(size_t i = 0; i < mag.size(); i++) {
        magDB[i] = std::max(safeLog10(mag[i], refLevel, false), MIN_DB_VALUE);
    }
    for(size_t i = 0; i < psd.size(); i++) {
        psdDB[i] = std::max(safeLog10(psd[i], refSquare, true), MIN_DB_VALUE);
    }
}

/*
@brief: 星座图分析(仅对复数信号有效)
@param[out]: iqPoints - 返回所有符号点的IQ数据
@param[out]: phase - 每个符号点的相位(弧度)
@param[out]: magnitude - 每个符号点的幅度
@param symbolLength - 符号长度(默认为1, 表示无符号分隔)
@throws logic_error 如果信号不是复数类型
*/
void Analyzer::constellationAnalysis(vector<pair<double, double> > &iqPoints, vector<double> &phase, vector<double> &magnitude, int symbolLength) const {
    if(!isComplex_) {
        throw std::logic_error("Constellation analysis requires complex signal");
    }

    const auto& signal = complexSignal_;
    const int N = static_cast<int>(signal.size());
    //清空输出容器
    iqPoints.clear();
    phase.clear();
    magnitude.clear();

    //计算符号IQ值
    for(int i = 0; i < N; i += symbolLength) {
        const Complex& point = signal[i];
        const double I = point.real();
        const double Q = point.imag();
        iqPoints.emplace_back(I, Q);
        phase.push_back(std::arg(point));
        magnitude.push_back(std::abs(point));
    }
}
