#ifndef MOTORMODEL_H
#define MOTORMODEL_H

#include <qglobal.h>

namespace MotorModel
{
    uint16_t doubleToQ15(double value);
    double q15ToDouble(uint16_t value);
}

#endif // MOTORMODEL_H
