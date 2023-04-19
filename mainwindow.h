#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMenu>
#include <QToolBar>
#include <QStackedWidget>
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

    QWidget* createHomePage();
    QWidget* createDataPage();
    QWidget* createAnalysisPage();

    QStackedWidget* stackWidget;
    QAction* homeAction;
    QAction* dataAction;
    QAction* analysisAction;

    QCheckBox* tempCheck;
    QCheckBox* humidCheck;

    QLineSeries* seriesTemp;
    QLineSeries* seriesHumid;

    QChartView*  chartView;
    QChart* chart;

    embed_sql* mysql;
    QSqlTableModel* dataModel;

    QTimer* updateTimer;
public slots:
    void switchPages();
    void setChartSeriesVisibility();
    void updateDataModel();
    void updateChart();


};
#endif // MAINWINDOW_H
