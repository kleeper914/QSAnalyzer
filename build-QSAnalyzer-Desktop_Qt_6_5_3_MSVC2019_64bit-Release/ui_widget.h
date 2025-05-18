/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *MainWidget;
    QHBoxLayout *FilePath;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QTextEdit *filePath;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *Open_File;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *signalInfo;
    QLabel *meanLabel;
    QLineEdit *meanLineEdit;
    QLabel *varianceLabel;
    QLineEdit *varianceLineEdit;
    QLabel *rmsLabel;
    QLineEdit *rmsLineEdit;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *FunctionArea;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *Waveform;
    QPushButton *Amplitude_Spectrum;
    QPushButton *Power_Spectrum;
    QPushButton *Transient_Envelope;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *Eye_Diagram;
    QPushButton *Constellation_Diagram;
    QPushButton *pushButton_3;
    QPushButton *PushButton_2;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton_10;
    QPushButton *pushButton_11;
    QPushButton *pushButton_12;
    QPushButton *pushButton_13;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(800, 600);
        verticalLayoutWidget = new QWidget(Widget);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(19, 10, 771, 581));
        MainWidget = new QVBoxLayout(verticalLayoutWidget);
        MainWidget->setObjectName("MainWidget");
        MainWidget->setSizeConstraint(QLayout::SetDefaultConstraint);
        MainWidget->setContentsMargins(0, 0, 0, 0);
        FilePath = new QHBoxLayout();
        FilePath->setObjectName("FilePath");
        FilePath->setContentsMargins(-1, 0, -1, 0);
        label = new QLabel(verticalLayoutWidget);
        label->setObjectName("label");

        FilePath->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        FilePath->addItem(horizontalSpacer);

        filePath = new QTextEdit(verticalLayoutWidget);
        filePath->setObjectName("filePath");
        filePath->setMinimumSize(QSize(0, 40));
        filePath->setMaximumSize(QSize(16777215, 40));

        FilePath->addWidget(filePath);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        FilePath->addItem(horizontalSpacer_2);

        Open_File = new QPushButton(verticalLayoutWidget);
        Open_File->setObjectName("Open_File");
        Open_File->setMinimumSize(QSize(80, 40));
        Open_File->setMaximumSize(QSize(80, 40));
        Open_File->setIconSize(QSize(20, 20));

        FilePath->addWidget(Open_File);

        FilePath->setStretch(0, 1);
        FilePath->setStretch(2, 8);
        FilePath->setStretch(4, 1);

        MainWidget->addLayout(FilePath);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        MainWidget->addItem(verticalSpacer_2);

        signalInfo = new QHBoxLayout();
        signalInfo->setObjectName("signalInfo");
        meanLabel = new QLabel(verticalLayoutWidget);
        meanLabel->setObjectName("meanLabel");

        signalInfo->addWidget(meanLabel);

        meanLineEdit = new QLineEdit(verticalLayoutWidget);
        meanLineEdit->setObjectName("meanLineEdit");
        meanLineEdit->setReadOnly(true);

        signalInfo->addWidget(meanLineEdit);

        varianceLabel = new QLabel(verticalLayoutWidget);
        varianceLabel->setObjectName("varianceLabel");

        signalInfo->addWidget(varianceLabel);

        varianceLineEdit = new QLineEdit(verticalLayoutWidget);
        varianceLineEdit->setObjectName("varianceLineEdit");
        varianceLineEdit->setReadOnly(true);

        signalInfo->addWidget(varianceLineEdit);

        rmsLabel = new QLabel(verticalLayoutWidget);
        rmsLabel->setObjectName("rmsLabel");

        signalInfo->addWidget(rmsLabel);

        rmsLineEdit = new QLineEdit(verticalLayoutWidget);
        rmsLineEdit->setObjectName("rmsLineEdit");
        rmsLineEdit->setReadOnly(true);

        signalInfo->addWidget(rmsLineEdit);

        signalInfo->setStretch(0, 1);
        signalInfo->setStretch(1, 2);
        signalInfo->setStretch(2, 1);
        signalInfo->setStretch(3, 2);
        signalInfo->setStretch(4, 1);
        signalInfo->setStretch(5, 2);

        MainWidget->addLayout(signalInfo);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Fixed);

        MainWidget->addItem(verticalSpacer);

        FunctionArea = new QVBoxLayout();
        FunctionArea->setObjectName("FunctionArea");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        Waveform = new QPushButton(verticalLayoutWidget);
        Waveform->setObjectName("Waveform");
        Waveform->setMinimumSize(QSize(80, 80));
        Waveform->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_3->addWidget(Waveform);

        Amplitude_Spectrum = new QPushButton(verticalLayoutWidget);
        Amplitude_Spectrum->setObjectName("Amplitude_Spectrum");
        Amplitude_Spectrum->setMinimumSize(QSize(80, 80));

        horizontalLayout_3->addWidget(Amplitude_Spectrum);

        Power_Spectrum = new QPushButton(verticalLayoutWidget);
        Power_Spectrum->setObjectName("Power_Spectrum");
        Power_Spectrum->setMinimumSize(QSize(80, 80));

        horizontalLayout_3->addWidget(Power_Spectrum);

        Transient_Envelope = new QPushButton(verticalLayoutWidget);
        Transient_Envelope->setObjectName("Transient_Envelope");
        Transient_Envelope->setMinimumSize(QSize(80, 80));

        horizontalLayout_3->addWidget(Transient_Envelope);

        horizontalLayout_3->setStretch(0, 1);
        horizontalLayout_3->setStretch(1, 1);
        horizontalLayout_3->setStretch(2, 1);
        horizontalLayout_3->setStretch(3, 1);

        FunctionArea->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        Eye_Diagram = new QPushButton(verticalLayoutWidget);
        Eye_Diagram->setObjectName("Eye_Diagram");
        Eye_Diagram->setMinimumSize(QSize(80, 80));

        horizontalLayout_2->addWidget(Eye_Diagram);

        Constellation_Diagram = new QPushButton(verticalLayoutWidget);
        Constellation_Diagram->setObjectName("Constellation_Diagram");
        Constellation_Diagram->setMinimumSize(QSize(80, 80));

        horizontalLayout_2->addWidget(Constellation_Diagram);

        pushButton_3 = new QPushButton(verticalLayoutWidget);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setMinimumSize(QSize(80, 80));

        horizontalLayout_2->addWidget(pushButton_3);

        PushButton_2 = new QPushButton(verticalLayoutWidget);
        PushButton_2->setObjectName("PushButton_2");
        PushButton_2->setMinimumSize(QSize(80, 80));

        horizontalLayout_2->addWidget(PushButton_2);

        horizontalLayout_2->setStretch(0, 1);
        horizontalLayout_2->setStretch(1, 1);
        horizontalLayout_2->setStretch(2, 1);
        horizontalLayout_2->setStretch(3, 1);

        FunctionArea->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        pushButton_10 = new QPushButton(verticalLayoutWidget);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setMinimumSize(QSize(80, 80));

        horizontalLayout_4->addWidget(pushButton_10);

        pushButton_11 = new QPushButton(verticalLayoutWidget);
        pushButton_11->setObjectName("pushButton_11");
        pushButton_11->setMinimumSize(QSize(80, 80));

        horizontalLayout_4->addWidget(pushButton_11);

        pushButton_12 = new QPushButton(verticalLayoutWidget);
        pushButton_12->setObjectName("pushButton_12");
        pushButton_12->setMinimumSize(QSize(80, 80));

        horizontalLayout_4->addWidget(pushButton_12);

        pushButton_13 = new QPushButton(verticalLayoutWidget);
        pushButton_13->setObjectName("pushButton_13");
        pushButton_13->setMinimumSize(QSize(80, 80));

        horizontalLayout_4->addWidget(pushButton_13);

        horizontalLayout_4->setStretch(0, 1);
        horizontalLayout_4->setStretch(1, 1);
        horizontalLayout_4->setStretch(2, 1);
        horizontalLayout_4->setStretch(3, 1);

        FunctionArea->addLayout(horizontalLayout_4);

        FunctionArea->setStretch(0, 1);
        FunctionArea->setStretch(1, 1);
        FunctionArea->setStretch(2, 1);

        MainWidget->addLayout(FunctionArea);

        MainWidget->setStretch(0, 4);
        MainWidget->setStretch(1, 1);
        MainWidget->setStretch(2, 2);
        MainWidget->setStretch(3, 1);
        MainWidget->setStretch(4, 16);

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Widget", nullptr));
        label->setText(QCoreApplication::translate("Widget", "File Path: ", nullptr));
        Open_File->setText(QCoreApplication::translate("Widget", "Open File", nullptr));
        meanLabel->setText(QCoreApplication::translate("Widget", "Mean:", nullptr));
        varianceLabel->setText(QCoreApplication::translate("Widget", "Variance/power:", nullptr));
        rmsLabel->setText(QCoreApplication::translate("Widget", "RMS:", nullptr));
        Waveform->setText(QCoreApplication::translate("Widget", "Waveform", nullptr));
        Amplitude_Spectrum->setText(QCoreApplication::translate("Widget", "Amplitude Spectrum", nullptr));
        Power_Spectrum->setText(QCoreApplication::translate("Widget", "Power Spectrum", nullptr));
        Transient_Envelope->setText(QCoreApplication::translate("Widget", "Transient Envelope", nullptr));
        Eye_Diagram->setText(QCoreApplication::translate("Widget", "Eye Diagram", nullptr));
        Constellation_Diagram->setText(QCoreApplication::translate("Widget", "Constellation Diagram", nullptr));
        pushButton_3->setText(QCoreApplication::translate("Widget", "PushButton", nullptr));
        PushButton_2->setText(QCoreApplication::translate("Widget", "PushButton", nullptr));
        pushButton_10->setText(QCoreApplication::translate("Widget", "PushButton", nullptr));
        pushButton_11->setText(QCoreApplication::translate("Widget", "PushButton", nullptr));
        pushButton_12->setText(QCoreApplication::translate("Widget", "PushButton", nullptr));
        pushButton_13->setText(QCoreApplication::translate("Widget", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
