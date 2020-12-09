#ifndef MICROCONTROLLER_C_BINDING_H_
#define MICROCONTROLLER_C_BINDING_H_

#include "Microcontroller.h"

typedef void (*setup_function)(float **const, int);
typedef void (*loop_function)(void);

/* Must be inherited and C functions setupFcn and loopFcn
 * must be provided inside the CPP of the inherited class
 * wrapped inside extern "C". */
class MicrocontrollerCBinding : public Microcontroller
{
public:
    /* loopFcn must NOT contain any long-winded calls such as endless loop or blocking */
    MicrocontrollerCBinding(Microcontroller::VoltageLines &voltageLines, setup_function setupFcn, loop_function loopFcn);
    /* Makes the class abstract */
    virtual ~MicrocontrollerCBinding() = 0;

private:
    void microcontrollerUpdate() override final;
    loop_function m_loopFcn = nullptr;
    float *m_voltageLinesCArray[VoltageLine::Idx::Count];
};

#endif /* MICROCONTROLLER_C_BINDING_H_ */
