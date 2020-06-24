#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    arduino = new QSerialPort;
    port_name = "";
    ada_arduino = false;
    serialbuffer = "";

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
       if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
           if(serialPortInfo.vendorIdentifier() == vendor_id){
               if(serialPortInfo.productIdentifier() == produk_id){
                   port_name = serialPortInfo.portName();
                   ada_arduino = true;
               }
           }
       }
    }
    if(ada_arduino){
        arduino->setPortName(port_name);
        arduino->open(QSerialPort::ReadOnly);
        arduino->setBaudRate(QSerialPort::Baud9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setFlowControl(QSerialPort::NoFlowControl);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        QObject::connect(arduino, SIGNAL(readyRead()), this, SLOT(BacaSerial()));
    }else{
        QMessageBox::warning(this, "Error", "Arduino tidak terkoneksi");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    if(arduino->isOpen()){
        arduino->close();
    }
}

//fungsi membaca serialdata
void MainWindow::BacaSerial(){
    QStringList splitdata = serialbuffer.split(", ");
    if(splitdata.length() < 3){
        dataserial = arduino->readAll();
        serialbuffer += QString::fromStdString(dataserial.toStdString());
    }else{
        MainWindow::UpdateData(splitdata[1]);
        serialbuffer = "";
    }
}

void MainWindow::UpdateData(const QString data){
    ui->lbcount->setText(data + " cm");
}

