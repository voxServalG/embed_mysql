#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createMenu();
    createNaviBar();
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

    QAction* homeAction = new QAction("HOME" ,this);
    naviBar->addAction(homeAction);

    QAction* dataAction = new QAction("DATA" ,this);
    naviBar->addAction(dataAction);

    QAction* analysisAction = new QAction("ANALYSIS" ,this);
    naviBar->addAction(analysisAction);

    naviBar->setMovable(false);
}
