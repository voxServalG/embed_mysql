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


    createMenu();
    createNaviBar();
    createStackWidget();
    //createDataPage();

    this->updateTimer = new QTimer(this);
    this->updateTimer->setInterval(3000);
    this->updateTimer->start();

    //设置串口
    serial = new QSerialPort;
    foreach(const QSerialPortInfo& info, QSerialPortInfo::availablePorts())
    {
        serial->setPort(info);
        if(serial->open(QIODevice::ReadWrite))      // 以读写方式打开串口
                {
                    qDebug() << "串口打开成功";                        // 关闭
                } else
                {
                    qDebug() << "串口打开失败，请重试";
                }
    }
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setParity(QSerialPort::NoParity);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);


    //设置串口

    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateDataModel);
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateChart);
    //
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateSerialData);
    //
    connect(homeAction, &QAction::triggered, this, &MainWindow::switchPages);
    connect(dataAction, &QAction::triggered, this, &MainWindow::switchPages);
    connect(analysisAction, &QAction::triggered, this, &MainWindow::switchPages);
    connect(tempCheck, &QCheckBox::stateChanged, this, &MainWindow::setChartSeriesVisibility);
    connect(humidCheck, &QCheckBox::stateChanged, this, &MainWindow::setChartSeriesVisibility);
}

MainWindow::~MainWindow()
{
    delete ui;
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

    naviBar->setMovable(false);
}

void MainWindow::createStackWidget()
{
    stackWidget = new QStackedWidget;
    QWidget* homePage = createHomePage();
    QWidget* dataPage = createDataPage();
    QWidget* analysisPage = createAnalysisPage();

    stackWidget->addWidget(homePage);
    stackWidget->addWidget(dataPage);
    stackWidget->addWidget(analysisPage);



    setCentralWidget(stackWidget);

}
QWidget* MainWindow::createHomePage()
{
    QWidget* homePage = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout;

    QLabel* welcomeLabel = new QLabel("欢迎使用巡检系统");
    QLabel* stateLabel = new QLabel(isDatabaseConnected ? "数据库已连接" : "数据库未连接");
    layout->addWidget(welcomeLabel);
    layout->addWidget(stateLabel);

    homePage->setLayout(layout);
    return homePage;
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

    QLabel* timeLabel = new QLabel("TIME");
    QLineEdit* timeEdit = new QLineEdit;
    QLabel* tempLabel = new QLabel("TEMP");
    QLineEdit* tempEdit = new QLineEdit;

    QHBoxLayout* sLayout_1 = new QHBoxLayout;
    sLayout_1->addWidget(timeLabel);
    sLayout_1->addWidget(timeEdit);
    sLayout_1->addStretch();

    QHBoxLayout* sLayout_2 = new QHBoxLayout;
    sLayout_2->addWidget(tempLabel);
    sLayout_2->addWidget(tempEdit);
    sLayout_2->addStretch();

    QVBoxLayout* sLayout = new QVBoxLayout;
    sLayout->addLayout(sLayout_1);
    sLayout->addLayout(sLayout_2);
    QFrame* sFrame = new QFrame;
    sFrame->setFrameStyle(QFrame::StyledPanel);
    sFrame->setLayout(sLayout);


    //to update dataModel every interval. to do setModel for tableView every interval
    dataModel = new QSqlTableModel(dataPage, mysql->getDb());
    dataModel->setTable("field_data");
    dataModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    dataModel->setHeaderData(0, Qt::Horizontal, "TIME");
    dataModel->setHeaderData(1, Qt::Horizontal, "TEMP");
    dataModel->setHeaderData(2, Qt::Horizontal, "HUMID");
    dataModel->setHeaderData(3, Qt::Horizontal, "LIGHT");
    dataModel->select();

    QTableView* tableView = new QTableView(this);
    tableView->setModel(dataModel);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(titleLayout);
    layout->addWidget(sFrame);
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
    tempCheck = new QCheckBox("Temp");
    humidCheck = new QCheckBox("Humidity");
    tempCheck->setCheckState(Qt::Checked);
    humidCheck->setCheckState(Qt::Checked);
    layout_title->addWidget(titleLabel);
    layout_title->addWidget(tempCheck);
    layout_title->addWidget(humidCheck);

//setting chartview into 'layout'
    chartView = new QChartView(this);
    chart = new QChart();
    chart->setTitle("ANALYSIS");
    chartView->setChart(chart);
    this->setCentralWidget(chartView);

    layout->addWidget(chartView);

//Adding QHB into QVB
    QVBoxLayout* layout_final = new QVBoxLayout;
    layout_final->addLayout(layout_title);
    layout_final->addLayout(layout);
    analysisPage->setLayout(layout_final);
    return analysisPage;
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


}

void MainWindow::updateDataModel()
{
    this->dataModel->select();
}

void MainWindow::updateChart()
{
    QChart* newChart = new QChart();
    seriesTemp = new QLineSeries();
    seriesHumid = new QLineSeries();
    seriesTemp->setName("Temp");
    seriesHumid->setName("Humidity");


    //add series to chart
    newChart->addSeries(seriesTemp);
    newChart->addSeries(seriesHumid);
    //seriesHumid->setVisible(false);

    //sql query
    QSqlQuery query(mysql->getDb());
    QString sql = "select * from field_data";
    query.exec(sql);
    bool isQueryEmpty = true;
    int numOfValue = 0;
    qint32 timeFirst, timeEnd;
    while(query.next())
    {
        numOfValue++;
        qint32 time = query.value(0).toInt();
        qreal temp = query.value(1).toDouble();
        qreal humidity = query.value(2).toDouble();
        if(isQueryEmpty)
        {
            timeFirst = time;
            isQueryEmpty = false;
        }
        timeEnd = time;
        seriesTemp->append(time, temp);
        seriesHumid->append(time, humidity);
    }

    //set axis
    QValueAxis* axisX_time = new QValueAxis;
    axisX_time->setRange(timeFirst, timeEnd);
    axisX_time->setTickCount(numOfValue);

    newChart->addAxis(axisX_time, Qt::AlignBottom);
    QValueAxis *axisY=new QValueAxis;                //左边y轴
    axisY->setRange(0,100);
    axisY->setTickCount(11);
    newChart->addAxis(axisY, Qt::AlignLeft);
    QValueAxis *axisY1=new QValueAxis;              //右边y轴
    axisY1->setRange(0,100);
    axisY1->setTickCount(11);
    newChart->addAxis(axisY1, Qt::AlignRight);
    seriesTemp->attachAxis(axisX_time);                 //折线与坐标轴捆绑
    seriesTemp->attachAxis(axisY);

    seriesHumid->attachAxis(axisX_time);
    seriesHumid->attachAxis(axisY1);

    chartView->setChart(newChart);
}

void MainWindow::updateSerialData()
{
    QByteArray rx_data;
    rx_data = serial->readAll();
    qDebug() << rx_data.data();
}
