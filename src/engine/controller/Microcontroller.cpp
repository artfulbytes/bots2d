#include "Microcontroller.h"

#include <cassert>
#include <chrono>
#include <thread>

namespace {
    const unsigned int microcontrollerLoopSleep_ms = 10;
}

Microcontroller::Microcontroller(Microcontroller::VoltageLines &voltageLines) :
    m_simulatorVoltageLines(voltageLines)
{
}

Microcontroller::~Microcontroller()
{
    m_running = false;
    m_thread.join();
}

void Microcontroller::start()
{
    /* We need a separate function for starting the microcontroller because
     * we can't create/start inside the constructor when the object is not yet
     * fully created. */
    m_thread = std::thread(&Microcontroller::microcontrollerLoop, this);
}

void Microcontroller::transferVoltageLevelsSimulatorToMicrocontroller()
{
    m_voltageLinesMutex.lock();
    for (int i = 0; i < Microcontroller::VoltageLine::Idx::Count; i++) {
        if (m_simulatorVoltageLines[i].level == nullptr) {
            /* Skip unused lines */
            continue;
        }
        if (m_simulatorVoltageLines[i].type == Microcontroller::VoltageLine::Type::Output) {
            *m_simulatorVoltageLines[i].level = m_sharedVoltageLineLevels[i];
        } else {
            m_sharedVoltageLineLevels[i] = *m_simulatorVoltageLines[i].level;
        }
    }
    m_voltageLinesMutex.unlock();
}

void Microcontroller::transferVoltageLevelsMicrocontrollerToSimulator()
{
    m_voltageLinesMutex.lock();
    for (int i = 0; i < Microcontroller::VoltageLine::Idx::Count; i++) {
        if (m_simulatorVoltageLines[i].type == Microcontroller::VoltageLine::Type::Output) {
            m_sharedVoltageLineLevels[i] = m_microcontrollerVoltageLineLevels[i];
        } else {
            m_microcontrollerVoltageLineLevels[i] = m_sharedVoltageLineLevels[i];
        }
    }
    m_voltageLinesMutex.unlock();
}

/* Called by the simulator update loop, keep it short to avoid affecting
 * the frame rate. */
void Microcontroller::onFixedUpdate(double stepTime)
{
    transferVoltageLevelsSimulatorToMicrocontroller();
}

void Microcontroller::microcontrollerLoop()
{
    while (m_running) {
        transferVoltageLevelsMicrocontrollerToSimulator();
        microcontrollerUpdate();
        std::this_thread::sleep_for(std::chrono::milliseconds(microcontrollerLoopSleep_ms));
    }
}

void Microcontroller::onKeyEvent(const Event::Key &keyEvent)
{
    /* Microcontroller should typically not handle key events,
     * but don't make this method final, because it's
     * useful to override it when testing */
}

