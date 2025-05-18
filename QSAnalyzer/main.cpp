#include "widget.h"
#include <fftw3.h>
#include <stdlib.h>
#include <iostream>
#include <QApplication>
#include <cassert>
#include "qpskmodulator.h"

#define N 5

void testQpsk() {
    try {
        //参数设置: 10000bit, 符号率1kHz, 采样率 4kHz
        QPSKModulator qpsk(10000, 1000, 4000);
        qpsk.generate();
        //验证比特数是否为偶数
        const auto& bits = qpsk.getBits();
        assert(bits.size() == 10000 && "比特数应为10000");

        //验证符号数 = 比特数 / 2
        const auto& symbols = qpsk.getSymbols();
        assert(symbols.size() == 5000 && "QPSK符号数应为5000");

        //验证波形采样点数 = 符号数 * 每符号采样数 + 滤波器延迟
        const auto& waveform = qpsk.getModulateSignal();
        //符号数 * samplesPerSymbol + 滤波器跨度 * samplesPerSymbol - 1
        int expectedWaveformLength = 5000 * 4 + 6 * 4 - 1;

        //保存数据
        qpsk.saveSymbolsToFile("qpsk_symbols.csv");
        qpsk.saveWaveformToFile("qpsk_waveform.csv");

        cout << "test passed" << endl;
    }
    catch (const std::exception& e) {
        std::cerr << "test failed: " << e.what() << endl;
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    //设置窗口居中
    QScreen* scr = a.primaryScreen();
    int scr_w = scr->size().width();
    int scr_h = scr->size().height();
    w.move((scr_w - w.width()) / 2, (scr_h - w.height()) / 2);

    //testQpsk();

    w.show();
    return a.exec();
}
