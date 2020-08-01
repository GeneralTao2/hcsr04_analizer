#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    label = new QPlainTextEdit(this);
    ui->setupUi(this);
    label->document()->setPlainText("Hello");
    label->show();
    label->setGeometry(10, 10,780, 60);
    QFont font = label->font();
    font.setPointSize(10);
    label->setFont(font);

    message = new USART_message(50);

    serial = new QSerialPort(this);
    serial->setPortName("COM4");
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->open(QIODevice::ReadWrite);
    serial->write("Wello!");

    chart = new QChart();
    chart->legend()->hide();
    chart->setTitle("HC-SR04");

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->setParent(this);
    chartView->setGeometry(10, 70, 780, 460);

    clearButton = new QPushButton(this);
    clearButton->setGeometry(10, 540, 80, 40);
    clearButton->setText("CLEAR");

    startAnalizing = new QPushButton(this);
    startAnalizing->setGeometry(100, 540, 80, 40);
    startAnalizing->setText("START");

    stopAnalizing = new QPushButton(this);
    stopAnalizing->setGeometry(190, 540, 80, 40);
    stopAnalizing->setText("STOP");

    connect(serial, SIGNAL(readyRead()), this, SLOT(serialRecive()));

    connect(this, SIGNAL(newSerialData()), this, SLOT(analizeSerialData()));

    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearChart()));

    connect(startAnalizing, SIGNAL(clicked()), this, SLOT(startAnalizingSlot()));

    connect(stopAnalizing, SIGNAL(clicked()), this, SLOT(stopAnalizingSlot()));

    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QSerialPort port;
        port.setPort(info);
        if(port.open(QIODevice::ReadWrite)) {
            qDebug() << "Name: " + info.portName() + " " + info.description() + " " + info.manufacturer();
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    serial->close();
    delete  serial;
    delete label;
    delete message;
    delete chart;
    delete chartView;
    delete startAnalizing;
    delete stopAnalizing;
    delete clearButton;
}

void MainWindow::clearChart() {
    chart->removeAllSeries();
}

void MainWindow::startAnalizingSlot() {
    connect(serial, SIGNAL(readyRead()), this, SLOT(serialRecive()));
}

void MainWindow::stopAnalizingSlot() {
    disconnect(serial, SIGNAL(readyRead()), this, SLOT(serialRecive()));
}

void MainWindow::analizeSerialData() {
    QVector<int> arrayData;
    QStringList list = message->globalMap["array"].split(QLatin1Char(' '), Qt::SkipEmptyParts);

    QLineSeries *series = new QLineSeries();
    for(int i=0; i<list.size(); i++) {
        series->append(i, list[i].toInt());
    }
    chart->addSeries(series);
    chart->createDefaultAxes();
    chartView->setChart(chart);
}

void MainWindow::serialRecive() {
    QByteArray ba;
    ba = serial->readAll();
    message->words[message->words.size()-1] = QString::fromLocal8Bit(QByteArray::fromHex(ba.toHex()));
    auto map = message->parseValueToMap();
    if(message->globalMap.empty()) {
        message->globalMap = map;
    } else if(message->globalMap["id"] != map["id"]) {
        message->globalMap = map;
        emit newSerialData();
    }
    label->document()->setPlainText("array: " + map["array"]);
    for(int i=0; i<message->words.size()-1; i++) {
        message->words[i] = message->words[i+1];
    }
}
