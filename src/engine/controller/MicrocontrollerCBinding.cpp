#include "MicrocontrollerCBinding.h"
#include "stdio.h"

MicrocontrollerCBinding::MicrocontrollerCBinding(Microcontroller::VoltageLineArray &voltageLines, setup_function setupFcn, loop_function loopFcn) :
    Microcontroller(voltageLines),
    m_loopFcn(loopFcn)
{
    for (int i = 0; i < Microcontroller::VOLTAGE_LINE_COUNT; i++) {
        m_voltageLinesCArray[i] = m_voltageLines[i];
    }
    setupFcn(m_voltageLinesCArray, Microcontroller::VOLTAGE_LINE_COUNT);
}

void MicrocontrollerCBinding::onFixedUpdate(double stepTime)
{
    /* TODO: Make this a separate thread and put mutex during update in engine */
    m_loopFcn();
}

MicrocontrollerCBinding::~MicrocontrollerCBinding()
{
}
