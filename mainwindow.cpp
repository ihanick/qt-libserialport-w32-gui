#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial.moveToThread(&serial_thread);

    connect(this, SIGNAL(read()), &serial, SLOT(read()));
    connect(this, SIGNAL(write(QByteArray)), &serial, SLOT(write(QByteArray)));
    connect(this, SIGNAL(SerialOpen(QString)), &serial, SLOT(SerialOpen(QString)));
    connect(&serial, SIGNAL(read_done(LibSerialPortQ*)), this, SLOT(read_done(LibSerialPortQ*)));
    connect(&serial, SIGNAL(write_done()), this, SLOT(write_done()));

    serial_thread.start();


    ports = serial.enum_all_ports();
    foreach (QString val, ports.keys()) {
        ui->portSelector->addItem(ports[val], val);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_datatoSend_returnPressed()
{
    send();
}

void MainWindow::on_sndBtn_clicked()
{
    send();
}

void MainWindow::send() {
    emit write(ui->datatoSend->text().toLocal8Bit() );
    ui->datatoSend->clear();
}

void MainWindow::read_done(LibSerialPortQ *conn) {
    ui->serialOut->appendPlainText(QString(conn->read_data()));
}

void MainWindow::write_done() {
    emit read();
}

void MainWindow::on_openBtn_clicked()
{
    int idx = ui->portSelector->currentIndex();
    if (idx < 0) {
        ports.clear();
        ports = serial.enum_all_ports();
        foreach (QString val, ports.keys()) {
            ui->portSelector->addItem(ports[val], val);
        }
        return;
    }

    QString portname = ui->portSelector->itemData(idx).toString();

    emit SerialOpen(portname);
}
