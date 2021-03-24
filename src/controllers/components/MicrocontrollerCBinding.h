#ifndef MICROCONTROLLER_C_BINDING_H_
#define MICROCONTROLLER_C_BINDING_H_

#include "Microcontroller.h"

typedef void (*main_function)(void);
typedef void (*main_function_userdata)(void *);
/**
 * Base class for providing C-bindings to the controller code.
 * Useful if the code that runs on your target is pure C-code.
 *
 * This must be inherited, and the inheriting class needs to provide a
 * C-function main (wrapped inside extern "C", which should act as the
 * main function of the C-code.
 *
 * NOTE: If you want to simulate several C microcontrollers at the same
 * time, you can't define your "state" variables outside your functions, because
 * they will overwrite each other. You must define them inside your main function.
 */
class MicrocontrollerCBinding : public Microcontroller
{
public:
    /**
     * To pass a main function without userdata argument. ONLY one such microcontroller
     * can run at a time!
     */
    MicrocontrollerCBinding(Microcontroller::VoltageLines &voltageLines, main_function mainFcn);
    /**
     * To pass a main function with userdata argument. Use this if you simulate multiple
     * C microcontrollers at a time.
     */
    MicrocontrollerCBinding(Microcontroller::VoltageLines &voltageLines, main_function_userdata mainFcn);
    virtual ~MicrocontrollerCBinding() = 0;

private:
    void main() override final;
    main_function m_mainFcn = nullptr;
    main_function_userdata m_mainFcnUserdata = nullptr;
};

#endif /* MICROCONTROLLER_C_BINDING_H_ */
