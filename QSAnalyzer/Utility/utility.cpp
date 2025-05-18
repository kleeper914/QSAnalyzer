#include "utility.h"

QString complexToQString(const std::complex<double>& complexNum) {
    std::ostringstream oss;
    oss << complexNum.real() << (complexNum.imag() >= 0 ? "+" : "") << complexNum.imag() << "i";
    return QString::fromStdString(oss.str());
}

bool containsNumber(const std::string &s) {
    std::istringstream iss(s);
    double dummy;
    return (iss >> dummy) && (iss.eof());
}

bool isComplexLine(const std::string &s) {
    std::istringstream iss(s);
    double re, im;
    return (iss >> re >> im) && (iss.eof());
}

void extractRealAndImg(const std::vector<std::complex<double>>& signal, std::vector<double>& real, std::vector<double>& imag) {
    for(int i = 0; i < signal.size(); i++) {
        real[i] = signal[i].real();
        imag[i] = signal[i].imag();
    }
}

std::vector<double> linToDB(const std::vector<double> &data, bool isPower) {
    std::vector<double> dBData;
    dBData.reserve(data.size());
    const double ref = isPower ? 1.0 : sqrt(1.0);
    const double epsilon = 1e-20;

    for(auto val : data) {
        double dB = isPower ?
            10 * log10(val / ref + epsilon) :
            20 * log10(val / ref + epsilon);
        dBData.push_back(dB);
    }
    return dBData;
}

void shiftVector(std::vector<double> &vec) {
    const size_t N = vec.size();
    if(N < 2) return;

    const size_t half = N / 2;
    std::rotate(vec.begin(), vec.begin() + half, vec.end());
}
