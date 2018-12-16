#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <QObject>
#include <QSerialPort>

class SerialComm : public QObject
{
    Q_OBJECT
public:
    explicit SerialComm(QObject *parent = nullptr);

    enum Command {
        LifePulseTsk,
        TextSet,
        ButtonSet,
        SliderSet,
        VariableGet,
        ArrayGet,
        DataGet
    };

    enum SliderType {
        SetLsw = 0,
        SetTargetSpeed = 2,
        SetMotorEngage = 4
    };

signals:
    void cmdAck(QString);
    void errorMessage(QString);
    void setup(QString);
    void disconnected();
    void connected();
    void log(QString);

public slots:
    void setupSerialPort(QString name);
    void sendCmd(SerialComm::Command cmd, uint8_t size, uint16_t data);
    void connectToDevice();
    void disconnectFromDevice();

private:
    QSerialPort serialPort;
};

#endif // SERIALCOMM_H
