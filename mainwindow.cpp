#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mysql = new embed_sql;
    if(mysql->init_sql())
        isDatabaseConnected = true;
    else
        isDatabaseConnected = false;



    initTimer();

    initSerial();
    this->bootSec = 0;
    //
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDateTimeStr = currentDateTime.toString("yyyyMMdd_hhmmss");
    newTableName = "field_data_" + currentDateTimeStr;
    QString createTableQuerySql = "CREATE TABLE `" + newTableName + "` (`time` INT(10) NOT NULL, `temp` VARCHAR(50) NOT NULL COLLATE 'utf8_general_ci', `humidity` VARCHAR(50) NOT NULL COLLATE 'utf8_general_ci', `light` VARCHAR(50) NOT NULL COLLATE 'utf8_general_ci', PRIMARY KEY (`time`) USING BTREE ) COLLATE='utf8_general_ci';";
    QSqlQuery createTable(mysql->getDb());
    createTable.exec(createTableQuerySql);
    //
    createCURFrame();
    createMenu();
    createNaviBar();
    createStackWidget();
    //createDataPage();



    connect(dataQueryTimer, &QTimer::timeout, this, &MainWindow::updateDataModel);
    connect(dataQueryTimer, &QTimer::timeout, this, &MainWindow::updateChart);
    connect(dataQueryTimer, &QTimer::timeout, this, &MainWindow::tableScrollToBottom);
    //
    connect(rxTimer, &QTimer::timeout, this, &MainWindow::updateSerialData);
    connect(rxTimer, &QTimer::timeout, this, &MainWindow::updateCURFrame);

    connect(rxClearTimer, &QTimer::timeout, this, &MainWindow::clearRx);
    //
    connect(homeAction, &QAction::triggered, this, &MainWindow::switchPages);
    connect(dataAction, &QAction::triggered, this, &MainWindow::switchPages);
    connect(analysisAction, &QAction::triggered, this, &MainWindow::switchPages);
    connect(historyAction, &QAction::triggered, this, &MainWindow::switchPages);
    connect(tempCheck, &QCheckBox::stateChanged, this, &MainWindow::setChartSeriesVisibility);
    connect(humidCheck, &QCheckBox::stateChanged, this, &MainWindow::setChartSeriesVisibility);
    connect(lightCheck, &QCheckBox::stateChanged, this, &MainWindow::setChartSeriesVisibility);
    connect(dateBox, &QComboBox::currentTextChanged, this, &MainWindow::updateTimeOnDateChanged);
    connect(timeBox, &QComboBox::currentTextChanged, this, &MainWindow::updateHistoryChartOnTimeChanged);
    connect(historyChartView->chart(), &QtCharts::QChart::plotAreaChanged, this, &MainWindow::refineAxisOnZoomHappened);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearRx()
{
    serial->clear();
}


void MainWindow::initTimer()
{
    this->dataQueryTimer = new QTimer(this);
    this->dataQueryTimer->setInterval(1000);
    this->dataQueryTimer->start();

    this->rxTimer = new QTimer(this);
    this->rxTimer->setInterval(1000);
    this->rxTimer->start();

    this->rxClearTimer = new QTimer(this);
    this->rxClearTimer->setInterval(6500);
    this->rxClearTimer->start();
}
void MainWindow::initSerial()
{
    //设置串口
    serial = new QSerialPort;
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setParity(QSerialPort::NoParity);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
//    foreach(const QSerialPortInfo& info, QSerialPortInfo::availablePorts())
//    {
//        serial->setPort(info);
//        if(serial->open(QIODevice::ReadOnly))      // 以读写方式打开串口
//                {
//                    qDebug() << "串口打开成功";                        // 关闭
//                } else
//                {
//                    qDebug() << "串口打开失败，请重试";
//                }
//    }
    serial->setPortName("COM5");
    isCOMConnected = false;
    if(serial->open(QIODevice::ReadOnly))      // 以读方式打开串口
                    {
                        isCOMConnected = true;
                    } else
                    {
                        isCOMConnected = false;
                    }


    //设置串口
}
void MainWindow::createMenu()
{
    //create top menus
    QMenu* fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("test");
    QMenu* editMenu = menuBar()->addMenu("Edit");
    QMenu* checkMenu = menuBar()->addMenu("Check");
    QMenu* helpMenu = menuBar()->addMenu("Help");
}

void MainWindow::createNaviBar()
{
    //create left navigation bar
    QToolBar* naviBar = new QToolBar("Navigation", this);
    addToolBar(Qt::LeftToolBarArea, naviBar);

    homeAction = new QAction("HOME" ,this);
    naviBar->addAction(homeAction);

    dataAction = new QAction("DATA" ,this);
    naviBar->addAction(dataAction);

    analysisAction = new QAction("ANALYSIS" ,this);
    naviBar->addAction(analysisAction);

    historyAction = new QAction("HISTORY", this);
    naviBar->addAction(historyAction);

    naviBar->setMovable(false);
}

void MainWindow::createStackWidget()
{
    stackWidget = new QStackedWidget;
    QWidget* homePage = createHomePage();
    QWidget* dataPage = createDataPage();
    QWidget* analysisPage = createAnalysisPage();
    QWidget* historyPage = createHistoryPage();

    stackWidget->addWidget(homePage);
    stackWidget->addWidget(dataPage);
    stackWidget->addWidget(analysisPage);
    stackWidget->addWidget(historyPage);



    setCentralWidget(stackWidget);

}
QWidget* MainWindow::createHomePage()
{
    QWidget* homePage = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout;
    QVBoxLayout* stateLayout = new QVBoxLayout;

    QLabel* welcomeLabel = new QLabel("欢迎使用巡检系统");
    QLabel* dbStateLabel = new QLabel(isDatabaseConnected ? "数据库已连接" : "数据库未连接");
    QLabel* COMStateLabel = new QLabel(isCOMConnected ? "COM connected" : "COM not connected");

    stateLayout->addWidget(dbStateLabel);
    stateLayout->addWidget(COMStateLabel);

    layout->addWidget(welcomeLabel);
    layout->addLayout(stateLayout);

    homePage->setLayout(layout);
    return homePage;
}

void MainWindow::updateCURFrame()
{
    this->timeValueLabel->setText(QString::number(time));
    this->tempValueLabel->setText(QString::number(temp));
    this->humidValueLabel->setText(QString::number(humidity));
    this->lightValueLabel->setText(QString::number(light));
}

void MainWindow::createCURFrame()
{
    this->CURFrame = new QFrame;

    QLabel* timeLabel = new QLabel("TIME");
    timeValueLabel = new QLabel(QString::number(time));
    QLabel* tempLabel = new QLabel("TEMP");
    tempValueLabel = new QLabel(QString::number(temp));
    QLabel* humidLabel = new QLabel("HUMIDITY");
    humidValueLabel = new QLabel(QString::number(humidity));
    QLabel* lightLabel = new QLabel("LIGHT");
    lightValueLabel = new QLabel(QString::number(light));


    QHBoxLayout* sLayout_1 = new QHBoxLayout;
    sLayout_1->addWidget(timeLabel);
    sLayout_1->addWidget(timeValueLabel);
    sLayout_1->addStretch();

    QHBoxLayout* sLayout_2 = new QHBoxLayout;
    sLayout_2->addWidget(tempLabel);
    sLayout_2->addWidget(tempValueLabel);
    sLayout_2->addStretch();

    QHBoxLayout* sLayout_3 = new QHBoxLayout;
    sLayout_3->addWidget(humidLabel);
    sLayout_3->addWidget(humidValueLabel);
    sLayout_3->addStretch();

    QHBoxLayout* sLayout_4 = new QHBoxLayout;
    sLayout_4->addWidget(lightLabel);
    sLayout_4->addWidget(lightValueLabel);
    sLayout_4->addStretch();


    QHBoxLayout* sLayout = new QHBoxLayout;
    sLayout->addLayout(sLayout_1);
    sLayout->addLayout(sLayout_2);
    sLayout->addLayout(sLayout_3);
    sLayout->addLayout(sLayout_4);
    CURFrame->setFrameStyle(QFrame::StyledPanel);
    CURFrame->setLayout(sLayout);

}
QWidget* MainWindow::createDataPage()
{
    QWidget* dataPage = new QWidget;

    QLabel* titleLabel = new QLabel("Field Data");
    QPushButton* addButton = new QPushButton("Add");
    QHBoxLayout* titleLayout = new QHBoxLayout;
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(addButton);

//    QLabel* timeLabel = new QLabel("TIME");
//    timeValueLabel = new QLabel(QString::number(time));
//    QLabel* tempLabel = new QLabel("TEMP");
//    tempValueLabel = new QLabel(QString::number(temp));


//    QHBoxLayout* sLayout_1 = new QHBoxLayout;
//    sLayout_1->addWidget(timeLabel);
//    sLayout_1->addWidget(timeValueLabel);
//    sLayout_1->addStretch();

//    QHBoxLayout* sLayout_2 = new QHBoxLayout;
//    sLayout_2->addWidget(tempLabel);
//    sLayout_2->addWidget(tempValueLabel);
//    sLayout_2->addStretch();

//    QVBoxLayout* sLayout = new QVBoxLayout;
//    sLayout->addLayout(sLayout_1);
//    sLayout->addLayout(sLayout_2);
//    QFrame* sFrame = new QFrame;
//    sFrame->setFrameStyle(QFrame::StyledPanel);
//    sFrame->setLayout(sLayout);


    //to update dataModel every interval. to do setModel for tableView every interval
    dataModel = new QSqlTableModel(dataPage, mysql->getDb());
    dataModel->setTable(newTableName);
    dataModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    dataModel->setHeaderData(0, Qt::Horizontal, "TIME");
    dataModel->setHeaderData(1, Qt::Horizontal, "TEMP");
    dataModel->setHeaderData(2, Qt::Horizontal, "HUMID");
    dataModel->setHeaderData(3, Qt::Horizontal, "LIGHT");
    dataModel->select();

    this->tableView = new QTableView(this);
    tableView->setModel(dataModel);
    tableView->setAutoFillBackground(true);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(titleLayout);
    layout->addWidget(CURFrame);
    layout->addWidget(tableView);

    dataPage->setLayout(layout);
    return dataPage;
}

QWidget* MainWindow::createAnalysisPage()
{
    QWidget* analysisPage = new QWidget;

    QHBoxLayout* layout = new QHBoxLayout;
    QHBoxLayout* layout_title = new QHBoxLayout;
//setting chartview into 'layout_title'
    QLabel* titleLabel = new QLabel("Analysis");
    tempCheck = new QCheckBox("TEMP");
    humidCheck = new QCheckBox("HUMIDITY");
    lightCheck = new QCheckBox("LIGHT");
    tempCheck->setCheckState(Qt::Checked);
    humidCheck->setCheckState(Qt::Checked);
    lightCheck->setCheckState(Qt::Checked);
    layout_title->addWidget(titleLabel);
    layout_title->addWidget(tempCheck);
    layout_title->addWidget(humidCheck);
    layout_title->addWidget(lightCheck);

//setting chartview into 'layout'
    chartView = new QChartView(this);
    chart = new QChart();
    chart->setTitle("ANALYSIS");
    chartView->setChart(chart);
    chartView->setAutoFillBackground(true);
    this->setCentralWidget(chartView);

    layout->addWidget(chartView);

//Adding QHB into QVB
    QVBoxLayout* layout_final = new QVBoxLayout;
    layout_final->addLayout(layout_title);
    layout_final->addWidget(CURFrame);
    layout_final->addLayout(layout);
    analysisPage->setLayout(layout_final);
    return analysisPage;
}

QWidget* MainWindow::createHistoryPage()
{
    QWidget* historyPage = new QWidget;

    QHBoxLayout* searchLayout = new QHBoxLayout;
    QLabel* dateTitleLabel = new QLabel("日期");
    dateBox = new QComboBox;
    QLabel* timeTitleLabel = new QLabel("时间");
    timeBox = new QComboBox;

    searchLayout->addWidget(dateTitleLabel);
    searchLayout->addWidget(dateBox);
    searchLayout->addStretch();
    searchLayout->addWidget(timeTitleLabel);
    searchLayout->addWidget(timeBox);
    searchLayout->addStretch();

    QStringList dateList = mysql->getAllDates();
    for(QString date : dateList)
    {
        dateBox->addItem(date);
    }

    QVBoxLayout* finalLayout = new QVBoxLayout;
    historyChartView = new QChartView;
    finalLayout->addLayout(searchLayout);
    finalLayout->addWidget(historyChartView);

    updateTimeOnDateChanged();
    updateHistoryChartOnTimeChanged();
    historyPage->setLayout(finalLayout);
    historyChartView->setRubberBand(QtCharts::QChartView::HorizontalRubberBand);

    return historyPage;
}
void MainWindow::switchPages()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if(action->text() == "HOME")
        stackWidget->setCurrentIndex(0);
    else if(action->text() == "DATA")
        stackWidget->setCurrentIndex(1);
    else if(action->text() == "ANALYSIS")
        stackWidget->setCurrentIndex(2);
    else if(action->text() == "HISTORY")
        stackWidget->setCurrentIndex(3);

}

void MainWindow::setChartSeriesVisibility()
{
    if(tempCheck->isChecked())
    {
        seriesTemp->setVisible(true);
    }
    else
    {
        seriesTemp->setVisible(false);
    }

    if(humidCheck->isChecked())
    {
        seriesHumid->setVisible(true);
    }
    else
    {
        seriesHumid->setVisible(false);
    }

    if(lightCheck->isChecked())
    {
        seriesLight->setVisible(true);
    }
    else
    {
        seriesLight->setVisible(false);
    }


}

void MainWindow::updateDataModel()
{
    this->dataModel->select();
}

void MainWindow::tableScrollToBottom()
{
    this->tableView->scrollToBottom();
}
void MainWindow::updateChart()
{
//    QChart* newChart = new QChart();
//    seriesTemp = new QLineSeries();
//    seriesHumid = new QLineSeries();
//    seriesLight = new QLineSeries();
//    seriesTemp->setName("TEMP");
//    seriesHumid->setName("HUMIDITY");
//    seriesLight->setName("LIGHT");


//    //add series to chart
//    newChart->addSeries(seriesTemp);
//    newChart->addSeries(seriesHumid);
//    newChart->addSeries(seriesLight);
//    //seriesHumid->setVisible(false);

//    //sql query
//    QSqlQuery query(mysql->getDb());
//    QString sql = "select * from " + newTableName;
//    query.exec(sql);
//    bool isQueryEmpty = true;
//    int numOfValue = 0;
//    qint32 timeFirst, timeEnd;
//    while(query.next())
//    {
//        numOfValue++;
//        time = query.value(0).toInt();
//        temp = query.value(1).toDouble();
//        humidity = query.value(2).toDouble();
//        light = query.value(3).toInt();
//        //qDebug() << light;
//        if(isQueryEmpty)
//        {
//            timeFirst = time;
//            isQueryEmpty = false;
//        }
//        timeEnd = time;
//        seriesTemp->append(time, temp);
//        seriesHumid->append(time, humidity);
//        seriesLight->append(time, light);

//    }

//    //set axis
//    QValueAxis* axisX_time = new QValueAxis;
//    timeFirst = timeEnd>=15 ? timeEnd-15 : 0;
//    axisX_time->setRange(timeFirst, timeEnd);
//    axisX_time->setTickCount(timeEnd - timeFirst + 1);

//    newChart->addAxis(axisX_time, Qt::AlignBottom);
//    QValueAxis *axisY=new QValueAxis;                //左边y轴
//    axisY->setRange(0,100);
//    axisY->setTickCount(11);
//    newChart->addAxis(axisY, Qt::AlignLeft);
//    QValueAxis *axisY1=new QValueAxis;              //右边y轴
//    axisY1->setRange(0,800);
//    axisY1->setTickCount(11);
//    newChart->addAxis(axisY1, Qt::AlignRight);

//    seriesTemp->attachAxis(axisX_time);                 //折线与坐标轴捆绑
//    seriesTemp->attachAxis(axisY);

//    seriesHumid->attachAxis(axisX_time);
//    seriesHumid->attachAxis(axisY);

//    seriesLight->attachAxis(axisX_time);
//    seriesLight->attachAxis(axisY1);
    QString sql = "select * from " + newTableName;
    QChart* newChart = depictChart(sql, 15);
    chartView->setChart(newChart);
    //chartView->setRubberBand(QChartView::HorizontalRubberBand);
}

QChart* MainWindow::depictChart(QString sql, int timeRange)
{
    QChart* newChart = new QChart();
    seriesTemp = new QLineSeries();
    seriesHumid = new QLineSeries();
    seriesLight = new QLineSeries();
    seriesTemp->setName("TEMP");
    seriesHumid->setName("HUMIDITY");
    seriesLight->setName("LIGHT");


    //add series to chart
    newChart->addSeries(seriesTemp);
    newChart->addSeries(seriesHumid);
    newChart->addSeries(seriesLight);
    //seriesHumid->setVisible(false);

    //sql query
    QSqlQuery query(mysql->getDb());
    query.exec(sql);
    bool isQueryEmpty = true;
    int numOfValue = 0;
    qint32 timeFirst, timeEnd;
    while(query.next())
    {
        numOfValue++;
        time = query.value(0).toInt();
        temp = query.value(1).toDouble();
        humidity = query.value(2).toDouble();
        light = query.value(3).toInt();
        //qDebug() << light;
        if(isQueryEmpty)
        {
            timeFirst = time;
            isQueryEmpty = false;
        }
        timeEnd = time;
        seriesTemp->append(time, temp);
        seriesHumid->append(time, humidity);
        seriesLight->append(time, light);

    }

    //set axis
    QValueAxis* axisX_time = new QValueAxis;
    timeFirst = timeEnd>=timeRange ? timeEnd-timeRange : 0;
    axisX_time->setRange(timeFirst, timeEnd);
    axisX_time->setTickCount(timeEnd - timeFirst + 1);

    newChart->addAxis(axisX_time, Qt::AlignBottom);
    QValueAxis *axisY=new QValueAxis;                //左边y轴
    axisY->setRange(0,100);
    axisY->setTickCount(11);
    newChart->addAxis(axisY, Qt::AlignLeft);
    QValueAxis *axisY1=new QValueAxis;              //右边y轴
    axisY1->setRange(0,800);
    axisY1->setTickCount(11);
    newChart->addAxis(axisY1, Qt::AlignRight);

    seriesTemp->attachAxis(axisX_time);                 //折线与坐标轴捆绑
    seriesTemp->attachAxis(axisY);

    seriesHumid->attachAxis(axisX_time);
    seriesHumid->attachAxis(axisY);

    seriesLight->attachAxis(axisX_time);
    seriesLight->attachAxis(axisY1);

    return newChart;
}

void MainWindow::updateSerialData()
{

    bootSec++;
    QByteArray rx_data = "na";

    rx_data = serial->readLine();
    qDebug() << rx_data;

    int tempIndex = rx_data.indexOf("T");
    int humidIndex = rx_data.indexOf("H");
    int lightIndex = rx_data.indexOf("L");
    int rxLength = rx_data.length();

    if(tempIndex != -1 && humidIndex != -1 && lightIndex != -1)
    {
    QString humidity = rx_data.mid(humidIndex+1, tempIndex-1);
    QString temp = rx_data.mid(tempIndex+1, lightIndex-tempIndex-1);
    QString light = rx_data.mid(lightIndex+1, rxLength-lightIndex-2);
    QString bootSecStr = QString::number(bootSec);
    //qDebug() << "data: "<< rx_data.data() << ", T="<<temp<<", H="<<humid<<", L="<<light<<", bootSec="<<bootSec;

    QString addQuerySql = "INSERT INTO " + newTableName + " (time, temp, humidity, light) VALUES (" + bootSecStr + "," + temp + "," + humidity + "," + light + ");";
    qDebug() << addQuerySql;

    QSqlQuery* addQuery = new QSqlQuery(mysql->getDb());
    addQuery->exec(addQuerySql);


    }



}




void MainWindow::updateTimeOnDateChanged()
{
    QString selectedDate = dateBox->currentText();
    QStringList timeListOnSelectedDate = mysql->getAllTimesProvidedDate(selectedDate);

    this->timeBox->clear();
    for(QString time : timeListOnSelectedDate)
    {
        timeBox->addItem(time);
    }
}

void MainWindow::updateHistoryChartOnTimeChanged()
{
    QString currentDate = dateBox->currentText();
    QString currentTime = timeBox->currentText();
    QString tableName = "field_data_" + currentDate + "_" + currentTime;
    QString sql = "SELECT * FROM " + tableName;
    QChart* newChart = depictChart(sql, 1800);
    historyChartView->setChart(newChart);
}

void MainWindow::refineAxisOnZoomHappened(const QRectF &plotArea)
{
    // 获取图表的 X 轴和 Y 轴
            QtCharts::QValueAxis *axisX = qobject_cast<QtCharts::QValueAxis *>(historyChartView->chart()->axisX());
            QtCharts::QValueAxis *axisY = qobject_cast<QtCharts::QValueAxis *>(historyChartView->chart()->axisY());

            // 根据缩放后的绘图区域调整坐标轴范围
            axisX->setRange(plotArea.left(), plotArea.right());
            axisY->setRange(plotArea.bottom(), plotArea.top());

            // 根据缩放后的绘图区域调整坐标轴刻度
            axisX->applyNiceNumbers();
            axisY->applyNiceNumbers();
}
