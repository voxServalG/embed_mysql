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

    dataModel = new QSqlTableModel(dataPage, mysql->db);
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
