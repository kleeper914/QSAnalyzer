#include "signalplot.h"

/*
@brief: SignalPlot构造函数
@param: parent - 父窗口指针
*/
SignalPlot::SignalPlot(QWidget *parent)
    : QDialog{parent}, m_zoomDirection(Qt::Horizontal)
{
    //初始化绘图区域
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);  //允许最大化
    m_plot = new QCustomPlot(this);
    layout = new QVBoxLayout(this);
    layout->setContentsMargins(10, 10, 10, 10);
    layout->addWidget(m_plot);
    resize(800, 600);       //初始窗口大小

    //基础样式设置
    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);     //允许拖拽缩放
    m_plot->axisRect()->setRangeZoom(m_zoomDirection);
    m_plot->legend->setVisible(true);   //显示图例
    m_plot->legend->setBrush(QBrush(QColor(255, 255, 255, 200)));   //半透明背景

    //增加右键菜单支持
    m_plot->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_plot, &QCustomPlot::customContextMenuRequested, this, &SignalPlot::showContextMenu);
}

/*
@brief: SignalPlot析构函数
*/
SignalPlot::~SignalPlot() {

}

/*
@brief: 初始化绘图设置
@param: xLabel - x轴的标签
@param: yLabel - y轴的标签
*/
void SignalPlot::initPlot(const QString &xLabel, const QString &yLabel){
    m_plot->xAxis->setLabel(xLabel);
    m_plot->yAxis->setLabel(yLabel);
    m_plot->xAxis->grid()->setPen(QPen(QColor(200, 200, 200), 1, Qt::DotLine));
    m_plot->yAxis->grid()->setPen(QPen(QColor(200, 200, 200), 1, Qt::DotLine));
    m_plot->clearGraphs();  //清空旧图形
}

/*
@brief: 清除m_plot中所有图形并重绘
*/
void SignalPlot::clearPlot() {
    m_plot->clearPlottables();
    m_plot->clearGraphs();
    m_plot->replot();
}

/*
@brief: 设置频谱图的频率轴(x轴), 可选线性或对数刻度
@param: isLogScale - 是否启用对数坐标轴
*/
void SignalPlot::setSpectrumAxes(bool isLogScale) {
    if(isLogScale) {
        //使用对数刻度
        QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
        logTicker->setSubTickCount(8);  //增加子刻度密度
        logTicker->setLogBase(10);  //以10为底
        m_plot->xAxis->setTicker(logTicker);
        m_plot->xAxis->setScaleType(QCPAxis::stLogarithmic);
        m_plot->xAxis->grid()->setSubGridVisible(true);

    }
    else {
        //使用线性刻度
        QSharedPointer<QCPAxisTicker> linTicker(new QCPAxisTicker);
        m_plot->xAxis->setTicker(linTicker);
        m_plot->xAxis->setScaleType(QCPAxis::stLinear);
        m_plot->xAxis->grid()->setSubGridVisible(false);
    }

    m_plot->yAxis->setNumberPrecision(3);
}

/*
@brief: 设置功率谱的频率轴(x轴), 可选线性或对数刻度
@param: isLogScale - 是否启用对数坐标轴
*/
void SignalPlot::setPSDAxes(bool isLogScale) {
    if(isLogScale) {
        //使用对数刻度
        QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
        logTicker->setSubTickCount(8);  //增加子刻度密度
        logTicker->setLogBase(10);  //以10为底
        m_plot->xAxis->setTicker(logTicker);
        m_plot->xAxis->setScaleType(QCPAxis::stLogarithmic);
        m_plot->xAxis->grid()->setSubGridVisible(true);
    }
    else {
        //使用线性刻度
        QSharedPointer<QCPAxisTicker> linTicker(new QCPAxisTicker);
        m_plot->xAxis->setTicker(linTicker);
        m_plot->xAxis->setScaleType(QCPAxis::stLinear);
        m_plot->xAxis->grid()->setSubGridVisible(false);
    }

    m_plot->yAxis->setNumberPrecision(3);
}

/*
@brief: 切换频谱坐标刻度辅助函数
*/
void SignalPlot::replotCurrentGraph() {
    switch (currentPlotType) {
    case Amplitude:
        plotAmplitudeSpectrum(xLogScale, yDBUnit);
        break;
    case PSD:
        plotPSD(xLogScale, yDBUnit);
        break;
    default:
        break;
    }
}

/*
@brief: 绘制时域波形图
@param: real - 信号实部(I路)
@param: imag - 信号虚部(Q路)(可选, 不传则只绘制实部)
*/
void SignalPlot::plotTimeDomain(double sampleRate, const vector<double> &real, const vector<double> &imag) {
    //保存状态
    currentPlotType = TimeDomain;
    xLogScale = false;
    yDBUnit = false;

    //生成时间轴
    const int N = static_cast<int>(real.size());
    QVector<double> time(N);
    QVector<double> y1(N);
    for(int i = 0; i < N; i++) {
        time[i] = (double)i / sampleRate;
        y1[i] = real[i];
    }
    initPlot("t(s)", "Amplitude");
    //绘制I路波形
    m_plot->addGraph();
    m_plot->graph(0)->setData(time, y1);
    m_plot->graph(0)->setPen(QPen(Qt::blue));
    m_plot->graph(0)->setName("In-phase");

    if(!imag.empty()) {
        QVector<double> y2(N);
        for(int i = 0; i < N; i++) y2[i] = imag[i];
        m_plot->addGraph();
        m_plot->graph(1)->setData(time, y2);
        m_plot->graph(1)->setPen(QPen(Qt::red));
        m_plot->graph(1)->setName("Quadrature-phase");
        m_plot->legend->setVisible(true);
    }

    m_plot->rescaleAxes();
    m_plot->replot();
}

/*
@brief: 绘制幅频谱
@param: isLogScale - 是否使用对数频率轴
@param: isDB - 是否使用dB
*/
void SignalPlot::plotAmplitudeSpectrum(bool isLogScale, bool isDB) {
    if(spectrumCache.freq.empty() || spectrumCache.magLinear.empty()) {
        QMessageBox::warning(this, "Plot error", "Spectrum data is empty");
        return;
    }
    if(spectrumCache.freq.size() != spectrumCache.magLinear.size()) {
        QMessageBox::warning(this, "Plot error", "The frequency and amplitude data lengths are inconsistent");
        return;
    }
    //保存数据和状态
    currentPlotType = Amplitude;
    xLogScale = isLogScale;
    yDBUnit = isDB;
    const auto& magData = isDB ? spectrumCache.magDB : spectrumCache.magLinear;
    QVector<double> qFreq = QVector<double>(spectrumCache.freq.begin(), spectrumCache.freq.end());
    QVector<double> qMag = QVector<double>(magData.begin(), magData.end());
    const int N = static_cast<int>(magData.size());
    QString title = "Amplitude Spectrum";

    clearPlot();
    initPlot(isLogScale ? "Frequency [Hz] (Log)" : "Frequency [Hz]", isDB ? "Amplitude [dB]" : "Amplitude");
    //添加标题
    //tip: 这里有点bug, 每次切换坐标刻度都会新增一行title
    // QCPTextElement* titleElem = new QCPTextElement(m_plot);
    // titleElem->setText(title);
    // titleElem->setFont(QFont("Arial", 12, QFont::Bold));
    // m_plot->plotLayout()->insertRow(0);
    // m_plot->plotLayout()->addElement(0, 0, titleElem);

    //创建频谱曲线
    m_plot->addGraph();
    QPen spectrumPen(Qt::blue);
    spectrumPen.setWidthF(1.5);     //加粗线条
    if(isDB) {
        spectrumPen.setColor(QColor(30, 120, 200));
        spectrumPen.setStyle(Qt::DashLine);
    }
    m_plot->graph(0)->setPen(spectrumPen);
    m_plot->graph(0)->setData(qFreq, qMag);
    m_plot->graph(0)->setName("Amplitude Spectrum");
    //坐标轴配置
    setSpectrumAxes(isLogScale);
    //范围设置
    m_plot->rescaleAxes();

    // 对数刻度特殊处理
    if(isLogScale) {
        QCPRange xRange = m_plot->xAxis->range();
        if(xRange.lower <= 0) {
            xRange.lower = 1e-6; // 设置最小显示频率
            m_plot->xAxis->setRange(xRange);
        }
    }

    m_plot->replot();
}

/*
@brief: 绘制功率谱密度(PSD)
@param: isLogScale - 是否使用对数频率刻度
@param: isDB - 是否转换为分贝
*/
void SignalPlot::plotPSD(bool isLogScale, bool isDB) {
    if(spectrumCache.freq.empty() || spectrumCache.magLinear.empty()) {
        QMessageBox::warning(this, "Plot error", "Spectrum data is empty");
        return;
    }
    if(spectrumCache.freq.size() != spectrumCache.magLinear.size()) {
        QMessageBox::warning(this, "Plot error", "The frequency and amplitude data lengths are inconsistent");
        return;
    }
    //保存数据及状态
    currentPlotType = PSD;
    xLogScale = isLogScale;
    yDBUnit = isDB;
    const auto& psdData = isDB ? spectrumCache.psdDB : spectrumCache.psdLinear;
    QVector<double> qFreq = QVector<double>(spectrumCache.freq.begin(), spectrumCache.freq.end());
    QVector<double> qPSD = QVector<double>(psdData.begin(), psdData.end());
    const int N = static_cast<int>(spectrumCache.freq.size());
    QString title = "Power Spectral Density";

    clearPlot();
    initPlot(isLogScale ? "Frequency [Hz] (Log)" : "Frequency [Hz]", isDB ? "Power Spectral Density [dB/Hz]" : "Power Spectral Density [W/Hz]");
    //添加标题
    // QCPTextElement* titleElem = new QCPTextElement(m_plot);
    // titleElem->setText(title);
    // titleElem->setFont(QFont("Arial", 12, QFont::Bold));
    // titleElem->setTextColor(QColor(50, 50, 50));
    // m_plot->plotLayout()->insertRow(0);
    // m_plot->plotLayout()->addElement(0, 0, titleElem);

    //创建PSD曲线
    m_plot->addGraph();
    QPen psdPen(QColor(0, 115, 189));
    psdPen.setWidthF(1.8);     //加粗线条
    psdPen.setStyle(isDB ? Qt::SolidLine : Qt::DashDotLine);
    m_plot->graph(0)->setPen(psdPen);
    m_plot->graph(0)->setData(qFreq, qPSD);
    m_plot->graph(0)->setName("Power Spectral Density");
    //坐标轴配置
    setPSDAxes(isLogScale);
    //范围设置
    m_plot->rescaleAxes();

    //对数轴保护设置
    if(isLogScale) {
        QCPRange xRange = m_plot->xAxis->range();
        if(xRange.lower <= 1e-6) xRange.lower = 1e-6;
        m_plot->xAxis->setRange(xRange);
        //Y轴动态调整(避免负无穷)
        QCPRange yRange = m_plot->yAxis->range();
        if(isDB && yRange.lower < -200) yRange.lower = -200;
        m_plot->yAxis->setRange(yRange);
    }

    //执行绘图
    m_plot->replot(QCustomPlot::rpQueuedRefresh);   //队列刷新提升性能
}

/*-------------------Slot函数实现--------------------*/
void SignalPlot::showContextMenu(const QPoint &pos) {
    QMenu menu(m_plot);
    QAction* resetAction = menu.addAction("Reset Zoom");
    menu.addSeparator();    //分隔线
    //缩放模式选择
    QActionGroup* zoomGroup = new QActionGroup(&menu);
    QAction* zoomXAction = zoomGroup->addAction("Scale X axis");
    QAction* zoomYAction = zoomGroup->addAction("Scale Y axis");
    QAction* zoomXYAction = zoomGroup->addAction("Scale X and Y axis");
    //设置单选特性
    zoomXAction->setCheckable(true);
    zoomYAction->setCheckable(true);
    zoomXYAction->setCheckable(true);
    //根据当前状态设置勾选
    zoomXAction->setChecked(m_zoomDirection == Qt::Horizontal);
    zoomYAction->setChecked(m_zoomDirection == Qt::Vertical);
    zoomXYAction->setChecked(m_zoomDirection == (Qt::Horizontal | Qt::Vertical));
    menu.addActions(zoomGroup->actions());

    connect(resetAction, &QAction::triggered, [this](){
        m_plot->rescaleAxes();
        m_plot->replot();
    });

    connect(zoomXAction, &QAction::triggered, [this](){
        m_zoomDirection = Qt::Horizontal;
        m_plot->axisRect()->setRangeZoom(m_zoomDirection);
    });

    connect(zoomYAction, &QAction::triggered, [this](){
        m_zoomDirection = Qt::Vertical;
        m_plot->axisRect()->setRangeZoom(m_zoomDirection);
    });

    connect(zoomXYAction, &QAction::triggered, [this](){
        m_zoomDirection = Qt::Horizontal | Qt::Vertical;
        m_plot->axisRect()->setRangeZoom(m_zoomDirection);
    });

    //频谱类绘图显示切换x轴log/y轴dB
    if(currentPlotType == Amplitude || currentPlotType == PSD) {
        menu.addSeparator();

        //x轴对数切换
        QAction* xLogAction = menu.addAction("Log X Axis");
        xLogAction->setCheckable(true);
        xLogAction->setChecked(xLogScale);
        connect(xLogAction, &QAction::toggled, this, &SignalPlot::toggleXLogScale);

        //y轴dB切换
        QAction* yDBAction = menu.addAction("y dB Scale");
        yDBAction->setCheckable(true);
        yDBAction->setChecked(yDBUnit);
        connect(yDBAction, &QAction::toggled, this, &SignalPlot::toggleYdBUnit);

        menu.addSeparator();
    }

    menu.exec(m_plot->mapToGlobal(pos));
}

void SignalPlot::toggleXLogScale(bool checked) {
    xLogScale = checked;
    replotCurrentGraph();
}

void SignalPlot::toggleYdBUnit(bool checked) {
    yDBUnit = checked;
    replotCurrentGraph();
}
