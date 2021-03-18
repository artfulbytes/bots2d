#include "components/MicrocontrollerCBinding.h"

MicrocontrollerCBinding::MicrocontrollerCBinding(Microcontroller::VoltageLines &voltageLines, setup_function setupFcn, loop_function loopFcn) :
    Microcontroller(voltageLines),
    m_loopFcn(loopFcn)
{
    setupFcn(get_voltage_level, set_voltage_level, physics_sleep, this);
}

void MicrocontrollerCBinding::microcontrollerUpdate()
{
    m_loopFcn();
}

MicrocontrollerCBinding::~MicrocontrollerCBinding()
{
}
