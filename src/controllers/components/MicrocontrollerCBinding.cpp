#include "components/MicrocontrollerCBinding.h"

MicrocontrollerCBinding::MicrocontrollerCBinding(Microcontroller::VoltageLines &voltageLines, unsigned int updateRateHz, setup_function setupFcn, loop_function loopFcn) :
    Microcontroller(voltageLines, updateRateHz),
    m_loopFcn(loopFcn)
{
    setupFcn(get_voltage_level, set_voltage_level, this);
}

void MicrocontrollerCBinding::microcontrollerUpdate()
{
    m_loopFcn();
}

MicrocontrollerCBinding::~MicrocontrollerCBinding()
{
}
