#include "components/MicrocontrollerCBinding.h"

extern "C" {
#include "microcontroller_c_functions.h"
#include "microcontroller_c_setup.h"
}


MicrocontrollerCBinding::MicrocontrollerCBinding(Microcontroller::VoltageLines &voltageLines, main_function mainFcn) :
    Microcontroller(voltageLines),
    m_mainFcn(mainFcn)
{
    setup(get_voltage_level, set_voltage_level, physics_sleep, this);
}

void MicrocontrollerCBinding::main()
{
    m_mainFcn();
}

MicrocontrollerCBinding::~MicrocontrollerCBinding()
{
}
