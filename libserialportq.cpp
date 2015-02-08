#include "libserialportq.h"

LibSerialPortQ::LibSerialPortQ(QObject *parent) : QObject(parent), myport(NULL), comm_timeout(40)
{

}

LibSerialPortQ::~LibSerialPortQ()
{

}

