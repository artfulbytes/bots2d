#include "MicrocontrollerCBinding.h"

MicrocontrollerCBinding::MicrocontrollerCBinding(Microcontroller::VoltageLines &voltageLines, setup_function setupFcn, loop_function loopFcn) :
    Microcontroller(voltageLines),
    m_loopFcn(loopFcn)
{
    for (int i = 0; i < Microcontroller::VoltageLine::Idx::Count; i++) {
        m_voltageLinesCArray[i] = &m_microcontrollerVoltageLineLevels[i];
    }
    setupFcn(m_voltageLinesCArray, Microcontroller::VoltageLine::Idx::Count);
}

void MicrocontrollerCBinding::microcontrollerUpdate()
{
    m_loopFcn();
}

MicrocontrollerCBinding::~MicrocontrollerCBinding()
{
}
