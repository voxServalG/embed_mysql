#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMenu>
#include <QToolBar>
#include <QStackedWidget>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTableView>
#include <QChartView>
#include <QLineSeries>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QCheckBox>
#include <QTimer>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QtCharts>

#include "embed_sql.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private:
    Ui::MainWindow *ui;
    void createMenu();
    void createNaviBar();
    void createStackWidget();
    void initSerial();
    void initTimer();

    QWidget* createHomePage();
    QWidget* createDataPage();
    QWidget* createAnalysisPage();

    QStackedWidget* stackWidget;
    QAction* homeAction;
    QAction* dataAction;
    QAction* analysisAction;

    bool isDatabaseConnected;

    QCheckBox* tempCheck;
    QCheckBox* humidCheck;
    QCheckBox* lightCheck;

    QLineSeries* seriesTemp;
    QLineSeries* seriesHumid;
    QLineSeries* seriesLight;

    QChartView*  chartView;
    QChart* chart;

    embed_sql* mysql;
    QSqlTableModel* dataModel;

    QTimer* dataQueryTimer;
    QTimer* rxTimer;

    QSerialPort* serial;
    QString portName;
    QString tempArea;

    int bootSec;
public slots:
    void switchPages();
    void setChartSeriesVisibility();
    void updateDataModel();
    void updateChart();
    void updateSerialData();


};
#endif // MAINWINDOW_H
