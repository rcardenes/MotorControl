#include "motormodel.h"

#define DOUBLETOQ15FACTOR 32768
#define Q15TODOUBLEFACTOR 0.000030517578125

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
