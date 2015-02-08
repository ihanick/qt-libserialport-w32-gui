#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QMap>

#include "libserialportq.h"


class QThreadEx : public QThread
{
protected:
    void run() { exec(); }
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_datatoSend_returnPressed();

    void on_sndBtn_clicked();

    void on_openBtn_clicked();

public slots:
    void read_done(LibSerialPortQ* conn);
    void write_done();


signals:
    void write(QByteArray data);
    void read();
    void SerialOpen(QString name);

private:
    Ui::MainWindow *ui;
    QThreadEx serial_thread;

    LibSerialPortQ serial;
    QMap<QString, QString> ports;

    void send();
};

#endif // MAINWINDOW_H
