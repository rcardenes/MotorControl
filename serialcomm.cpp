#include "serialcomm.h"

/*
 * Communications protocol:
 *
 *  - Each packet will be composed of 4 bytes:
 *
 *    1) Command Byte
 *    2) Size Byte
 *    3) Data LSB
 *    4) Data MSB
 *
 *  - The target system will echo the bytes back. Read the input
 *    to confirm delivery.
 *
 *  -
 */
SerialComm::SerialComm(QObject *parent) : QObject(parent),
    serialPort(this)
{
}

void
SerialComm::setupSerialPort(QString name)
{
    if (serialPort.isOpen()) {
        serialPort.close();
        emit this->disconnected();
    }

    serialPort.setPortName(name);
    serialPort.setBaudRate(9600);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);

    emit setup(name);
}

static QString translatePacket(QString message, char packet[4])
{
    return message
            .arg(qulonglong(packet[0]), 0, 16, QChar('0'))
            .arg(qulonglong(packet[1]), 0, 16, QChar('0'))
            .arg(qulonglong(packet[2]), 0, 16, QChar('0'))
            .arg(qulonglong(packet[3]), 0, 16, QChar('0'));
}


static QString translatePacket(QString message, char packet[4], int pos)
{
    return message
            .arg(qulonglong(packet[0]), 0, 16, QChar('0'))
            .arg(qulonglong(packet[1]), 0, 16, QChar('0'))
            .arg(qulonglong(packet[2]), 0, 16, QChar('0'))
            .arg(qulonglong(packet[3]), 0, 16, QChar('0'))
            .arg(pos);
}

void
SerialComm::sendCmd(SerialComm::Command cmd, uint8_t size, uint16_t data)
{
    if (!serialPort.isOpen()) {
        emit this->errorMessage("Serial port not setup");
        return;
    }

    char packet[4];

    packet[0] = static_cast<char>(cmd);
    packet[1] = static_cast<char>(size);
    packet[2] = static_cast<char>(data & 0xff);
    packet[3] = static_cast<char>(data >> 8);

    // Discard all buffered data, including input, if any
    serialPort.clear();

    for (int i = 0; i < 4; i++) {
        if (serialPort.write(&packet[i], 1) != 1) {
            emit this->errorMessage(QString("Couldn't write byte ... #%1").arg(i));
            return;
        }
        if (!serialPort.waitForReadyRead(500)) {
            QString text("Timed out without answer for message: %1 %2 %3 %4");
            emit this->errorMessage(translatePacket(text, packet));
            return;
        }

        char readChar;
        serialPort.read(&readChar, 1);
        if (packet[i] != readChar) {
            QString text("Received wrong ACK at byte #%5 for message: %1 %2 %3 %4");
            emit this->errorMessage(translatePacket(text, packet, i));
            return;
        }
    }

    emit this->cmdAck(translatePacket(QString("ACK for: %1 %2 %3 %4"), packet));
}

void
SerialComm::connectToDevice()
{
    qInfo("Connecting...");
    if (!serialPort.open(QIODevice::ReadWrite)) {
        emit this->errorMessage(QString("Could not connect to device: ") + serialPort.errorString());
    }
    else {
        qInfo("Connected!");
        emit this->connected();
    }
}

void
SerialComm::disconnectFromDevice()
{
    serialPort.close();
    emit this->disconnected();
}
