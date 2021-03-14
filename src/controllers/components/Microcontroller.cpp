#include "components/Microcontroller.h"

#include <cassert>
#include <chrono>
#include <thread>
#include <iostream>

namespace {
    const unsigned int startupTime_ms = 500;
}

Microcontroller::Microcontroller(Microcontroller::VoltageLines &voltageLines, unsigned int updateRateHz) :
    m_simulatorVoltageLines(voltageLines),
    m_loopSleepTime_ms(1000 / updateRateHz)
{
}

Microcontroller::~Microcontroller()
{
    m_running = false;
    if (m_thread.joinable()) {
        m_thread.join();
    } else {
        std::cout << "Microcontroller not started?" << std::endl;
    }
}

void Microcontroller::start()
{
    m_microcontrollerStarted = true;
    /* We need a separate function for starting the microcontroller because
     * we can't create/start inside the constructor when the object is not yet
     * fully created. Also, make sure we don't start it before the physics
     * has started. */
    if (m_physicsStarted) {
        m_thread = std::thread(&Microcontroller::microcontrollerThreadFn, this);
    }
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
            *m_simulatorVoltageLines[i].level = m_microcontrollerVoltageLineLevels[i];
        } else {
            m_microcontrollerVoltageLineLevels[i] = *m_simulatorVoltageLines[i].level;
        }
    }
    m_voltageLinesMutex.unlock();
}

/* Called by the simulator update loop, keep it short to avoid affecting
 * the frame rate. */
void Microcontroller::onFixedUpdate()
{
    m_physicsStarted = true;
    transferVoltageLevelsSimulatorToMicrocontroller();
    if (!m_thread.joinable() && m_microcontrollerStarted) {
        start();
    }
}

void Microcontroller::microcontrollerThreadFn()
{
    /* Give it some start up time to prevent buggy physics behaviour */
    std::this_thread::sleep_for(std::chrono::milliseconds(startupTime_ms));
    while (m_running) {
        microcontrollerUpdate();
        std::this_thread::sleep_for(std::chrono::milliseconds(m_loopSleepTime_ms));
    }
}

void Microcontroller::onKeyEvent(const Event::Key &keyEvent)
{
    (void)keyEvent;
}

float Microcontroller::getVoltageLevel(int idx)
{
    m_voltageLinesMutex.lock();
    assert(idx >= 0);
    assert(idx <= VoltageLine::Idx::Count);
    float level = m_microcontrollerVoltageLineLevels[idx];
    m_voltageLinesMutex.unlock();
    return level;
}

float Microcontroller::get_voltage_level(int idx, void *userdata)
{
    Microcontroller *microcontroller = static_cast<Microcontroller*>(userdata);
    assert(userdata != nullptr);
    return microcontroller->getVoltageLevel(idx);
}

void Microcontroller::setVoltageLevel(int idx, float level)
{
    m_voltageLinesMutex.lock();
    assert(idx >= 0);
    assert(idx <= VoltageLine::Idx::Count);
    m_microcontrollerVoltageLineLevels[idx] = level;
    m_voltageLinesMutex.unlock();
}

void Microcontroller::set_voltage_level(int idx, float level, void *userdata)
{
    Microcontroller *microcontroller = static_cast<Microcontroller*>(userdata);
    assert(userdata != nullptr);
    microcontroller->setVoltageLevel(idx, level);
}
