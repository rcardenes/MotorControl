#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <QObject>
#include <QSerialPort>

class SerialComm : public QObject
{
    Q_OBJECT

public:
    explicit SerialComm(QObject *parent = nullptr);


signals:
    void cmdAck(QString);
    void errorMessage(QString);
    void setup(QString);
    void disconnected();
    void connected();
    void log(QString);

public slots:
    void setupSerialPort(QString name);
    void writeCommand(uint16_t cmd, uint16_t size, uint16_t data, bool expectResponse);
    void connectToDevice();
    void disconnectFromDevice();

private:
    QSerialPort serialPort;
};

#endif // SERIALCOMM_H
