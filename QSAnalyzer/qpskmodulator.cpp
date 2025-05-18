#include "qpskmodulator.h"

QPSKModulator::QPSKModulator(int numBits, double symbolRate, double sampleRate,
              double rolloff, int filterSpan)
    : DigitalModulator(numBits, symbolRate, sampleRate, rolloff, filterSpan) {}

/*
@brief: QPSK调制实现(Gray编码)
@param: bits - 输入的二进制比特流
@return: 复数符号序列
@throws invalid_argument 如果比特数不是偶数
*/
vector<Complex> QPSKModulator::modulate(const vector<int> &bits) {
    if(bits.size() % 2 != 0) {
        throw std::invalid_argument("QPSK需要偶数比特数!");
    }
    vector<Complex> symbols;
    for(size_t i = 0; i < bits.size(); i += 2) {
        int b0 = bits[i];
        int b1 = bits[i + 1];
        //Gray编码映射
        if(b0 == 0 && b1 == 0) {
            symbols.push_back(Complex(1.0, 1.0) / sqrt(2.0));
        }
        else if(b0 == 0 && b1 == 1) {
            symbols.push_back(Complex(-1.0, 1.0) / sqrt(2.0));
        }
        else if(b0 == 1 && b1 == 1) {
            symbols.push_back(Complex(-1.0, -1.0) / sqrt(2.0));
        }
        else {
            symbols.push_back(Complex(1.0, -1.0) / sqrt(2.0));
        }
    }
    return symbols;
}
