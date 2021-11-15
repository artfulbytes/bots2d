#include "components/Microcontroller.h"

#include <cassert>
#include <chrono>
#include <thread>
#include <iostream>

Microcontroller::Microcontroller(Microcontroller::VoltageLines &voltageLines) :
    m_simulatorVoltageLines(voltageLines)
{
}

Microcontroller::~Microcontroller()
{
    m_running = false;
    m_sleepSteps = 0;
    /* Make sure we signal in case the thread is blocking */
    m_conditionWake.notify_one();
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

void Microcontroller::transferVoltageLevels()
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
void Microcontroller::onFixedUpdate(float stepTime)
{
    m_physicsStarted = true;

    /* Check if controller code has requested to sleep for X physics steps,
     * and if it has, count the steps and wake it up afterwards */
    std::unique_lock<std::mutex> uniqueLock(m_mutexSteps);
    m_currentStepTime = stepTime;
    if (m_sleepSteps > 0) {
        m_sleepSteps--;
        if (m_sleepSteps == 0) {
            m_conditionWake.notify_one();
        }
    }
    m_stepsTaken++;
    uniqueLock.unlock();


    transferVoltageLevels();
    if (!m_thread.joinable() && m_microcontrollerStarted) {
        start();
    }
}

void Microcontroller::microcontrollerThreadFn()
{
    /* To make the main function behave more like a real main function, we allow
     * it to contain an endless loop. But a thread with an endless loop can't be
     * terminated, and will normally cause the program to hang when we join the thread.
     * As a workaround (similar to the one used by Boost), check the m_running
     * variable in the C callback functions and throw an exception if it's false. Note,
     * for this to work, the main function must call the C callback functions
     * regularly. */
    try {
        main();
    } catch (int e) {
        (void)e;
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
    if (!m_running) {
        throw 0;
    }
    return level;
}

float get_voltage_cb(int idx, void *userdata)
{
    assert(userdata);
    Microcontroller *microcontroller = static_cast<Microcontroller*>(userdata);
    return microcontroller->getVoltageLevel(idx);
}

void Microcontroller::setVoltageLevel(int idx, float level)
{
    m_voltageLinesMutex.lock();
    assert(idx >= 0);
    assert(idx <= VoltageLine::Idx::Count);
    m_microcontrollerVoltageLineLevels[idx] = level;
    m_voltageLinesMutex.unlock();
    if (!m_running) {
        throw 0;
    }
}

void set_voltage_cb(int idx, float level, void *userdata)
{
    assert(userdata);
    Microcontroller *microcontroller = static_cast<Microcontroller*>(userdata);
    microcontroller->setVoltageLevel(idx, level);
}

void Microcontroller::msSleep(int sleep_ms)
{
    std::unique_lock<std::mutex> uniqueLock(m_mutexSteps);
    m_sleepSteps = static_cast<unsigned int>(sleep_ms / (m_currentStepTime * 1000));
    m_conditionWake.wait(uniqueLock, [this] { return m_sleepSteps == 0; });
    if (!m_running) {
        throw 0;
    }
}

void ms_sleep_cb(uint32_t sleep_ms, void *userdata)
{
    assert(userdata);
    Microcontroller *microcontroller = static_cast<Microcontroller*>(userdata);
    microcontroller->msSleep(sleep_ms);
}

uint32_t time_ms_cb(void *userdata)
{
    assert(userdata);
    Microcontroller *microcontroller = static_cast<Microcontroller*>(userdata);
    return microcontroller->timeMs();
}

uint32_t Microcontroller::timeMs()
{
    if (!m_running) {
        throw 0;
    }
    std::unique_lock<std::mutex> uniqueLock(m_mutexSteps);
    return static_cast<uint32_t>(1000 * m_stepsTaken * m_currentStepTime);
}
