#include "motormodel.h"

#define DOUBLETOQ15FACTOR 32768
#define Q15TODOUBLEFACTOR 0.000030517578125

MotorModel::MotorModel(SerialComm *serial, QObject *parent)
    : QObject(parent)
{
    connect(
                this, &MotorModel::sendCmd,
                serial, &SerialComm::writeCommand
                );
}

uint16_t
MotorModel::doubleToQ15(double value)
{
    return static_cast<uint16_t>(value * DOUBLETOQ15FACTOR);
}

double
MotorModel::q15ToDouble(uint16_t value)
{
    return double(value) * Q15TODOUBLEFACTOR;
}

void
MotorModel::setLsw(LswStates mode)
{
    emit sendCmd(SliderSet, SetLsw, mode, false);
}

void
MotorModel::setSpeedTarget(double value)
{
    emit sendCmd(SliderSet, SetTargetSpeed, doubleToQ15(value), false);
}

void
MotorModel::setMotorEngage(bool engage)
{
    emit sendCmd(SliderSet, SetMotorEngage, engage ? 1: 0, false);
}

void
MotorModel::getVariables()
{
    emit sendCmd(VariableGet, BusVolt, 0, true);
    emit sendCmd(VariableGet, ParkQ, 0, true);
    emit sendCmd(VariableGet, ParkD, 0, true);
}
