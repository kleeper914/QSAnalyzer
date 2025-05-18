#ifndef GENERATOR_H
#define GENERATOR_H

#include <vector>
#include <complex>
#include <random>

using namespace std;

enum class ModulationType {
    QPSK
};

class Generator
{
public:
    Generator();

private:
    double sampleRate_;
    std::mt19937 rng;
    vector<complex<double>> signal_;
};

#endif // GENERATOR_H
