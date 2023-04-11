#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mysql = new embed_sql;
    if(mysql->init_sql())
        qDebug("Connect successfully");
    else
        qDebug("Coneect failed");


    createMenu();
    createNaviBar();
    createStackWidget();
    //createDataPage();
    connect(homeAction, &QAction::triggered, this, &MainWindow::switchPages);
    connect(dataAction, &QAction::triggered, this, &MainWindow::switchPages);
    connect(analysisAction, &QAction::triggered, this, &MainWindow::switchPages);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMenu()
{
    //create top menus
    QMenu* fileMenu = menuBar()->addMenu("File");
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

    QLabel* label1 = new QLabel("1");
    QLabel* label2 = new QLabel("2");
    QLabel* label3 = new QLabel("3");

    stackWidget->addWidget(homePage);
    stackWidget->addWidget(dataPage);
    stackWidget->addWidget(analysisPage);

    setCentralWidget(stackWidget);

}
QWidget* MainWindow::createHomePage()
{
    QWidget* homePage = new QWidget;
    QHBoxLayout* layout = new QHBoxLayout;

    QLabel* welcomeLabel = new QLabel("WELCOME!!!");
    layout->addWidget(welcomeLabel);

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

    QChartView* chartView = new QChartView(this);
    QChart* chart = new QChart();
    chart->setTitle("ANALYSIS");
    chartView->setChart(chart);
    this->setCentralWidget(chartView);

    QLineSeries* seriesTemp = new QLineSeries();
    QLineSeries* seriesHumid = new QLineSeries();
    seriesTemp->setName("Temp");
    seriesHumid->setName("Humidity");
    chart->addSeries(seriesTemp);
    chart->addSeries(seriesHumid);

    //sql query
    QSqlQuery query(mysql->getDb());
    QString sql = "select * from field_data";
    query.exec(sql);
    bool isQueryEmpty = true;
    qint32 timeFirst, timeEnd;
    while(query.next())
    {
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
    axisX_time->setTickCount(20);

    chart->addAxis(axisX_time, Qt::AlignBottom);
    QValueAxis *axisY=new QValueAxis;                //左边y轴
    axisY->setRange(0,100);
    axisY->setTickCount(11);
    chart->addAxis(axisY, Qt::AlignLeft);
    QValueAxis *axisY1=new QValueAxis;                //右边y轴
    axisY1->setRange(0,100);
    axisY1->setTickCount(11);
    chart->addAxis(axisY1, Qt::AlignRight);
    seriesTemp->attachAxis(axisX_time);                 //折线与坐标轴捆绑
    seriesTemp->attachAxis(axisY);
    seriesHumid->attachAxis(axisX_time);
    seriesHumid->attachAxis(axisY1);


    layout->addWidget(chartView);
    analysisPage->setLayout(layout);
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
