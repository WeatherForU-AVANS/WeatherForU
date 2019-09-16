#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QAbstractBarSeries>
#include <QtCharts/QPercentBarSeries>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QAreaSeries>
#include <QtCharts/QLegend>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtCore/QRandomGenerator>
#include <QtCharts/QBarCategoryAxis>
#include <QtWidgets/QApplication>
#include <QtCharts/QValueAxis>

#include <QtSql>
#include <QtSql/QSqlDatabase>
#include <QMessageBox>

#include <QtNetwork>
#include <QNetworkAccessManager>

//#include <qtcharts/qbarcategoryaxis>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);




    //Tuturial: https://www.youtube.com/watch?v=_u7TcjIDNzw
    /*QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "my_sql_db");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("");
    //db.setPort(3306);
    db.setDatabaseName("weatherforu");
    if (db.open())
    {
        QMessageBox::information(this, "Connection", "Connection Succesfull");
    }
    else
    {
        QMessageBox::information(this, "Failed", db.lastError().text());
        //QMessageBox::information(this, "Failed", "Connection failed succesfully ");
    }*/

    QNetworkAccessManager manager;
    QNetworkReply *response = manager.get(QNetworkRequest(QUrl("http://localhost/weatherforu/")));
    QEventLoop event;
    connect(response,SIGNAL(finished()),&event,SLOT(quit()));
    event.exec();
    QString html = response->readAll(); // Source should be stored here
    //QMessageBox::information(this, "nice", html);

    QRegExp rx("(\\ |\\,|\\.|\\:|\\t)"); //RegEx for ' ' or ',' or '.' or ':' or '\t'
    QStringList query = html.split(rx);

    int array[99];
    for(int i = 0; i < query.length()-1; i++)
    {
        int value = query[i].toInt();
        array[i] = value;
        //QMessageBox::information(this, "nice", QString::number(value));
    }

    QChartView *chartViewTemperature = createLineChart("Temperature", array, query.length()-1, 0, query.length()-2, 0, 100 );
    QChartView *chartViewHumidity = createLineChart("Humidity", new int[5]{1,2,3,4,5}, 5, 0, 10, 0, 10 );
    QChartView *chartViewLight = createLineChart("Light", new int[5]{1,2,3,4,5}, 5, 0, 10, 0, 10 );

    ui->tabWidget->addTab(chartViewTemperature, "Temperature");
    ui->tabWidget->addTab(chartViewHumidity, "Humidity");
    ui->tabWidget->addTab(chartViewLight, "Light");







}

MainWindow::~MainWindow()
{
    delete ui;
}

DataTable MainWindow::generateRandomData(int listCount, int valueMax, int valueCount) const
{
    DataTable dataTable;

   /* // generate random data
    //for (int i(0); i < listCount; i++) {
        DataList dataList;
        qreal yValue(0);
        for (int j(0); j < valueCount; j++) {
            yValue = yValue + QRandomGenerator::global()->bounded(valueMax / (qreal) valueCount);
            QPointF value((j + QRandomGenerator::global()->generateDouble()) * ((qreal) m_valueMax / (qreal) valueCount),
                          yValue);
            QString label = "Slice " + QString::number(i) + ":" + QString::number(j);
            dataList << Data(value, label);
        }
        dataTable << dataList;
    //}*/

    return dataTable;
}

QChartView *MainWindow::createLineChart(QString name,int data[],int size, int minX, int maxX, int minY, int maxY)
{
    QChart *chart = new QChart();
    chart->setTitle(name);


    QLineSeries *lineseries = new QLineSeries();
       lineseries->setName(name);
       for(int i = 0; i < size; i++)
       {
           lineseries->append(QPoint(i, data[i]));
       }
       /*lineseries->append(QPoint(0, 1));
       lineseries->append(QPoint(1, 2.5));
       lineseries->append(QPoint(2, 5));
       lineseries->append(QPoint(3, 7.5));
       lineseries->append(QPoint(4, 8));
       lineseries->append(QPoint(5, 3));*/
          chart->addSeries(lineseries);



    chart->createDefaultAxes();
      chart->axes(Qt::Horizontal).first()->setRange(minX, maxX);
      chart->axes(Qt::Vertical).first()->setRange(minY, maxY);

      // Add space to label to add space between labels and axis
        QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
        Q_ASSERT(axisY);
        axisY->setLabelFormat("%.1f  ");
        QChartView *chartView;

        chartView = new QChartView(chart);

        return chartView;
}


