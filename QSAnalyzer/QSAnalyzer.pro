QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Analyzer/analyzer.cpp \
    DigitalModulator/digitalmodulator.cpp \
    main.cpp \
    QCustomPlot/qcustomplot.cpp \
    DigitalModulator/qpskmodulator.cpp \
    SignalPlot/signalplot.cpp \
    Utility/utility.cpp \
    Widget/widget.cpp

HEADERS += \
    Analyzer/analyzer.h \
    DigitalModulator/digitalmodulator.h \
    FFTW3/fftw3.h \
    QCustomPlot/qcustomplot.h \
    DigitalModulator/qpskmodulator.h \
    SignalPlot/signalplot.h \
    Utility/utility.h \
    Widget/widget.h

FORMS += \
    Widget/widget.ui

LIBS += D:\code\Qt\QSAnalyzer\QSAnalyzer\FFTW3\libfftw3-3.lib

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
