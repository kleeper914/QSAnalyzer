QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    analyzer.cpp \
    digitalmodulator.cpp \
    main.cpp \
    qcustomplot.cpp \
    qpskmodulator.cpp \
    signalplot.cpp \
    utility.cpp \
    widget.cpp

HEADERS += \
    analyzer.h \
    digitalmodulator.h \
    fftw3.h \
    qcustomplot.h \
    qpskmodulator.h \
    signalplot.h \
    utility.h \
    widget.h

FORMS += \
    widget.ui

LIBS += D:\code\Qt\QSAnalyzer\QSAnalyzer\libfftw3-3.lib

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
