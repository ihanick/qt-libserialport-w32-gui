#ifndef LIBSERIALPORTQ_H
#define LIBSERIALPORTQ_H

#include <QObject>
#include <QByteArray>
#include <QMutex>
#include <QMutexLocker>

#include <QDebug>

#include <QThread>
#include <QMap>

#include "libserialport.h"

class LibSerialPortQ;

class LibSerialPortQ : public QObject
{
    Q_OBJECT

    QByteArray rdata;
    QMutex rdata_mtx;
    struct sp_port *myport;
    int comm_timeout;
public:
    explicit LibSerialPortQ(QObject *parent = 0);
    ~LibSerialPortQ();

    QByteArray read_data() {
        QByteArray ret;
        {
            QMutexLocker locker(&rdata_mtx);
            ret.swap(rdata);
        }
        return ret;
    }

    QMap<QString, QString> enum_all_ports() {
        QMap<QString, QString> portinfo;
        enum sp_return ret;
        struct sp_port **port, **port_list, *myport = NULL;

        qDebug() << "Enum start";

        if ( (ret = sp_list_ports(&port_list) ) != SP_OK) {
            int code = sp_last_error_code();
            char * msg = sp_last_error_message();
            QString DataAsString = QString::fromUtf16((const ushort*)msg);
            qDebug() << DataAsString;
            sp_free_error_message(msg);
            return portinfo;
        }

        for(port = port_list; port != NULL && *port != NULL; ++port) {
            char *name, *desc, *tran_str;
            enum sp_transport tran;

            if ( (name = sp_get_port_name(*port)) == NULL)
                name = "";

            if ((desc = sp_get_port_description(*port) ) == NULL)
                desc = "";

            tran = sp_get_port_transport(*port);
            if (tran == SP_TRANSPORT_USB && myport == NULL)
                sp_copy_port(*port, &myport);
            switch (tran) {
            case SP_TRANSPORT_NATIVE:
                tran_str = "native";
                break;
            case SP_TRANSPORT_USB:
                tran_str = "usb";
                break;
            case SP_TRANSPORT_BLUETOOTH:
                tran_str = "bt";
                break;
            default:
                tran_str = "unk";
            }

            qDebug() << name << " " << desc << tran_str;
            portinfo[name] = QString("%1: %2 (%3)").arg(name).arg(desc).arg(tran_str);
        }
        qDebug() << "Enum end";
        return portinfo;
    }

signals:
    void connection_error(const QString msg);
    void read_error(const QString msg);
    void write_error(const QString msg);
    void read_done(LibSerialPortQ* conn);
    void write_done();
public slots:
    void SerialOpen(QString name) {
        qDebug() << "Opening port " << name;
        enum sp_return ret = SP_OK;
        if ((ret = sp_get_port_by_name(name.toLocal8Bit().constData(), &myport) ) < 0) {
            qDebug() << "Can't find port: '" << name << "'";
            return;
        }
        if(myport == NULL) {
            qDebug() << "Can't find port (NULL):  '" << name << "'";
            return;
        }
        if ( (ret = sp_open(myport, (enum sp_mode) (SP_MODE_READ|SP_MODE_WRITE) )) != SP_OK) {
            qDebug() << "Can't open port:  '" << name << "'" << name.size();
            return;
        }
        qDebug() << "Connected to:" << name;
    }
    void read() {
        qDebug() << "Read called";
        // make real read here
        //QObject().thread()->usleep(1000*1000*2);
        char response_data[80];
        enum sp_return ret = SP_OK;
        if (myport) {
            do {
                if ( (ret = sp_blocking_read(myport, response_data, 80, comm_timeout)) < 0 ) {
                    // handler error here
                    break;
                }
                {
                    QMutexLocker locker(&rdata_mtx);
                    rdata.append(response_data, (int)ret);
                }
            } while (ret > 0);
        }

        emit read_done(this);
    }

    void write(QByteArray data) {
        qDebug() << "Writing....";
        qDebug() << QString(data);

        //QObject().thread()->usleep(1000*1000*2);
        if (myport) {
            enum sp_return ret = SP_OK;
            if ( (ret = sp_blocking_write(myport, data.data(), data.length(), comm_timeout)) < 0 ) {
                // handle error
            }
        }

        qDebug() << "Write finished.";
        emit write_done();
    }
};

#endif // LIBSERIALPORTQ_H
