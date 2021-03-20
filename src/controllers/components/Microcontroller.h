#ifndef MICROCONTROLLER_H_
#define MICROCONTROLLER_H_

#include "ControllerComponent.h"
#include <array>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

/**
 * Base class for mimicking a real microcontroller.
 *
 * The execution of the microcontroller is offloaded to a separate thread to prevent
 * the simulator's update rate from depending on the microcontroller's update rate
 * (or vice versa).
 *
 * This class must be inherited and the inheritor must implement main.
 */
class Microcontroller : public ControllerComponent
{
public:
    /**
     * A voltage line simulates a real voltage line with a pointer
     * to a float value, which represents the voltage level. This pointer
     * enables sharing the value between a microcontroller and an "electrical"
     * object such as a motor or sensor. In other words, it's how the microcontroller
     * control output (e.g. motor) and read input (sensor).
     */
    struct VoltageLine {
        enum Idx {
            A0 = 0, A1, A2, A3, A4, A5, A6, A7,
            B0, B1, B2, B3, B4, B5, B6, B7,
            Count
        };
        /* Direction from the microcontroller's point of view (e.g. motor is output, a sensor is input) */
        enum class Type { Input, Output };
        Type type;
        float *level = nullptr;
    };
    typedef std::array<VoltageLine, VoltageLine::Idx::Count> VoltageLines;

    /**
     * \param voltageLines List of voltage lines that may be connected to "electrical" objects.
     * Users must manually keep track of which voltage lines are connected to what objects.
     */
    Microcontroller(VoltageLines &voltageLines);
    ~Microcontroller();

    /**
     * Must be called to start the loop function.
     */
    void start();

    void onFixedUpdate(float stepTime) override final;

    /**
     * Microcontroller should typically not handle key events, but don't make this method final,
     * because it can be useful to override it when debugging.
     */
    virtual void onKeyEvent(const Event::Key &keyEvent);

    /* C-callback function to set voltage level from controller code.
     * Accesses the member function getVoltageLevel by casting userdata. */
    static float get_voltage_level(int idx, void *userdata);
    float getVoltageLevel(int idx);

    /* C-callback function to set voltage level from controller code.
     * Accesses the member function setVoltageLevel by casting userdata. */
    static void set_voltage_level(int idx, float level, void *userdata);
    void setVoltageLevel(int idx, float level);

    /* C-callback function that calculates and sleeps for the number of physics steps contained
     * in sleep_ms milliseconds.
     *
     * NOTE: The precision is determined by how small the physics step time is.
     *       A smaller step time means better precision.
     * NOTE: The controller code SHOULD use this function and NOT the OS sleep function.
     */
    void physicsSleep(int sleep_ms);
    static void physics_sleep(int sleep_ms, void *userdata);

private:
    /** This is the controller main function; it runs in a separate thread */
    virtual void main() = 0;

    /** Thread function that runs the microcontroller loop */
    void microcontrollerThreadFn();

    /** The voltage lines which the simulator objects (e.g. sumobot, wheel motor) writes to and read from */
    VoltageLines m_simulatorVoltageLines;
    std::thread m_thread;
    std::atomic<bool> m_running = true;
    bool m_physicsStarted = false;
    bool m_microcontrollerStarted = false;

    /**
     * To give the simulator loop and microcontroller loop freedom to access the voltage lines whenever
     * they want while avoiding race conditions, each loop access its own array of voltage lines. The
     * values are copied (safely with a mutex) between them every simulator iteration.
     */
    std::mutex m_voltageLinesMutex;
    void transferVoltageLevelsSimulatorToMicrocontroller();
    float m_microcontrollerVoltageLineLevels[VoltageLine::Idx::Count] = {0};

    /**
     * To make the sleep behaviour of the controller consistent with the physics, we should
     * sleep for a certain number of physics steps. This results in a much more reliable sleep behaviour
     * compared to using the OS sleep function, because the OS sleep function has no guarantee for how
     * long it takes for a sleeping thread to wake up. This also enables us to consistently slow down and
     * speed up the simulation.
     */
    std::condition_variable m_conditionWake;
    unsigned int m_sleepSteps = 0;
    std::mutex m_mutexSleepSteps;
    float m_currentStepTime = 0.0f;
};


#endif /* MICROCONTROLLER_H_ */
