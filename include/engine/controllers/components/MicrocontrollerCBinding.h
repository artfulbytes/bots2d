#ifndef MICROCONTROLLER_C_BINDING_H_
#define MICROCONTROLLER_C_BINDING_H_

#include "Microcontroller.h"

typedef void (*setup_function)(float **const, int);
typedef void (*loop_function)(void);

/**
 * Base class for providing C-bindings to the controller code.
 * Useful if the code that runs on your target is pure C-code.
 *
 * This must be inherited, and the C functions setupFcn and loopFcn
 * must be provided inside the CPP of the inherited class and be
 * wrapped inside extern "C".
 */
class MicrocontrollerCBinding : public Microcontroller
{
public:
    /** loopFcn must NOT contain any long-winded calls such as endless loop or blocking */
    MicrocontrollerCBinding(Microcontroller::VoltageLines &voltageLines, setup_function setupFcn, loop_function loopFcn);
    virtual ~MicrocontrollerCBinding() = 0;

private:
    void microcontrollerUpdate() override final;
    loop_function m_loopFcn = nullptr;
    float *m_voltageLinesCArray[VoltageLine::Idx::Count];
};

#endif /* MICROCONTROLLER_C_BINDING_H_ */
