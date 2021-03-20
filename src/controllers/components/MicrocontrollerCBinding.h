#ifndef MICROCONTROLLER_C_BINDING_H_
#define MICROCONTROLLER_C_BINDING_H_

#include "Microcontroller.h"

typedef void (*main_function)(void);
/**
 * Base class for providing C-bindings to the controller code.
 * Useful if the code that runs on your target is pure C-code.
 *
 * This must be inherited, and the inheriting class needs to provide a
 * C-function main (wrapped inside extern "C", which should act as the
 * main function of the C-code.
 */
class MicrocontrollerCBinding : public Microcontroller
{
public:
    MicrocontrollerCBinding(Microcontroller::VoltageLines &voltageLines, main_function mainFcn);
    virtual ~MicrocontrollerCBinding() = 0;

private:
    void main() override final;
    main_function m_mainFcn = nullptr;
};

#endif /* MICROCONTROLLER_C_BINDING_H_ */
