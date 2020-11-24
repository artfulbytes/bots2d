#ifndef MICROCONTROLLER_C_BINDING_H_
#define MICROCONTROLLER_C_BINDING_H_

#include "Microcontroller.h"

typedef void (*setup_function)(float **const, int);
typedef void (*loop_function)(void);

/* Must be inherited and C functions setupFcn and loopFcn
 * must be provided inside the CPP of the inherited class
 * wrapped inside extern "C" */
class MicrocontrollerCBinding : public Microcontroller
{
public:
    MicrocontrollerCBinding(Microcontroller::VoltageLineArray &voltageLines, setup_function setupFcn, loop_function loopFcn);
    void onFixedUpdate(double stepTime) override final;
    /* Make class abstract */
    virtual ~MicrocontrollerCBinding() = 0;

private:
    loop_function m_loopFcn = nullptr;
    float *m_voltageLinesCArray[Microcontroller::VOLTAGE_LINE_COUNT];
};

#endif /* MICROCONTROLLER_C_BINDING_H_ */
