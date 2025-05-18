#ifndef QPSKMODULATOR_H
#define QPSKMODULATOR_H

#include "digitalmodulator.h"

class QPSKModulator : public DigitalModulator
{
public:
    QPSKModulator(int numBits = 1000, double symbolRate = 1000, double sampleRate = 4000,
                    double rolloff = 0.5, int filterSpan = 6);
protected:
    vector<Complex> modulate(const vector<int> &bits) override;
};

#endif // QPSKMODULATOR_H
