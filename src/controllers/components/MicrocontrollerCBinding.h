#ifndef MICROCONTROLLER_C_BINDING_H_
#define MICROCONTROLLER_C_BINDING_H_

#include "Microcontroller.h"

typedef void (*loop_function)(void);
typedef float (*get_voltage_function)(int, void *);
typedef void (*set_voltage_function)(int, float, void *);
typedef void (*setup_function)(get_voltage_function, set_voltage_function, void *);

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
    MicrocontrollerCBinding(Microcontroller::VoltageLines &voltageLines, unsigned int updateRateHz, setup_function setupFcn, loop_function loopFcn);
    virtual ~MicrocontrollerCBinding() = 0;

private:
    void microcontrollerUpdate() override final;
    loop_function m_loopFcn = nullptr;
};

#endif /* MICROCONTROLLER_C_BINDING_H_ */
