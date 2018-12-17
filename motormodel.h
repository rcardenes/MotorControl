#ifndef MOTORMODEL_H
#define MOTORMODEL_H

#include "serialcomm.h"
#include <QObject>

class MotorModel : public QObject
{
    Q_OBJECT

public:
    explicit MotorModel(SerialComm *, QObject *parent = nullptr);

    static uint16_t doubleToQ15(double value);
    static double q15ToDouble(uint16_t value);

    enum LswStates {
        Locked = 0,
        RampGenerator = 1,
        SMO = 2
    };

    void setLsw(LswStates mode);
    void setSpeedTarget(double target);
    void setMotorEngage(bool engage);
    void getVariables();

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

    enum AvailableVariable
    {
        BusVolt = 0,
        ParkQ   = 1,
        ParkD   = 3
    };


signals:
    void sendCmd(uint16_t, uint16_t, uint16_t, bool);
};

#endif // MOTORMODEL_H
