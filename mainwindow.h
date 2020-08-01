#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QPlainTextEdit>
#include <QDebug>
#include <QTimer>
#include <QStringList>

#include <QtCharts>

#include <QtCharts/QLineSeries>

#include "usart_message.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void serialRecive();
    void analizeSerialData();
    void clearChart();
    void startAnalizingSlot();
    void stopAnalizingSlot();
signals:
    void newSerialData();
private:
    Ui::MainWindow *ui;
    QPlainTextEdit *label;
    QSerialPort *serial;
    USART_message *message;

    QChart *chart;
    QChartView *chartView;

    QPushButton *startAnalizing;
    QPushButton *stopAnalizing;
    QPushButton *clearButton;
};
#endif // MAINWINDOW_H
