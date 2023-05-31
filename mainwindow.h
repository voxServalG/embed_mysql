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
#include <QComboBox>

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
    QWidget* createHistoryPage();
    void createCURFrame();

    QChart* depictChart(QString sqlForChart, int timeRange);

    QStackedWidget* stackWidget;
    QAction* homeAction;
    QAction* dataAction;
    QAction* analysisAction;
    QAction* historyAction;

    bool isDatabaseConnected;
    bool isCOMConnected;

    QCheckBox* tempCheck;
    QCheckBox* humidCheck;
    QCheckBox* lightCheck;

    QLineSeries* seriesTemp;
    QLineSeries* seriesHumid;
    QLineSeries* seriesLight;

    QChartView*  chartView;
    QChart* chart;
    QChartView* historyChartView;

    embed_sql* mysql;
    QSqlTableModel* dataModel;

    QTimer* dataQueryTimer;
    QTimer* rxTimer;
    QTimer* CURUpdateTimer;
    QTimer* rxClearTimer;

    QSerialPort* serial;
    QString portName;
    QString tempArea;
    QString newTableName;

    qint32 time, light;
    qreal temp, humidity;

    QLabel* timeValueLabel;
    QLabel* tempValueLabel;
    QLabel* humidValueLabel;
    QLabel* lightValueLabel;
    QLabel* coorValueLabel;
    QFrame* CURFrame;

    QTableView* tableView;
    int bootSec;
    int coorT;
    int coorX;
    int coorY;
    bool p1Passed;
    bool p2Passed;
    bool p3Passed;
    bool p4Passed;
    bool p5Passed;

    QComboBox* dateBox;
    QComboBox* timeBox;

    QMenu* fileMenu;
public slots:
    void switchPages();
    void setChartSeriesVisibility();
    void updateDataModel();
    void updateChart();
    void updateSerialData();
    void updateCURFrame();
    void clearRx();
    void tableScrollToBottom();
    void updateTimeOnDateChanged();
    void updateHistoryChartOnTimeChanged();
    void refineAxisOnZoomHappened(const QRectF &plotArea);
    void startUpdateCoordinate();
    void updateCoordinate();

};
#endif // MAINWINDOW_H
