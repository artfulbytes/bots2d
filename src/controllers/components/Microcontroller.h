#ifndef MICROCONTROLLER_H_
#define MICROCONTROLLER_H_

#include "ControllerComponent.h"
#include <array>
#include <atomic>
#include <thread>
#include <mutex>

/**
 * Base class for mimicking a real microcontroller.
 *
 * The microcontroller's update loop is offloaded to a separate thread to prevent the simulator's
 * update rate from depending on the microcontroller's update rate.
 *
 * This class must be inherited, and inherited classes must implement microcontrollerUpdate.
 */
class Microcontroller : public ControllerComponent
{
public:
    /**
     * A voltage line simulates a real voltage line with a pointer
     * to a float value, which represents the voltage level. This pointer
     * enables sharing the value between a microcontroller and an "electrical"
     * object such as a motor or sensor.
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
     * because it's useful to override it for debugging.
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

private:
    /** This is the main controller loop; it runs in a separate thread */
    virtual void microcontrollerUpdate() = 0;

    void microcontrollerLoop();
    std::thread m_thread;
    std::atomic<bool> m_running = true;

    /** The voltage lines which the simulator objects (e.g. sumobot, wheel motor) writes to and read from */
    VoltageLines m_simulatorVoltageLines;

    /**
     * To give the simulator loop and microcontroller loop freedom to access the voltage lines whenever
     * they want while avoiding race conditions, each loop access its own array of voltage lines. The
     * values are copied (safely with a mutex) between them every simulator iteration.
     */
    std::mutex m_voltageLinesMutex;
    void transferVoltageLevelsSimulatorToMicrocontroller();
    float m_microcontrollerVoltageLineLevels[VoltageLine::Idx::Count] = {0};
};


#endif /* MICROCONTROLLER_H_ */
