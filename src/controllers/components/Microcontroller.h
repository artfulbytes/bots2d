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
    void start();

    void onFixedUpdate(float stepTime) override final;

    /**
     * Microcontroller should typically not handle key events, but don't make this method final,
     * because it's useful to override it for debugging.
     */
    virtual void onKeyEvent(const Event::Key &keyEvent);

protected:
    /** The voltage line levels the microcontroller loop writes to and read from */
    float m_microcontrollerVoltageLineLevels[VoltageLine::Idx::Count];

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
     * shared array is used to safely (with a mutex) synchronize them.
     */
    std::mutex m_voltageLinesMutex;
    void transferVoltageLevelsSimulatorToMicrocontroller();
    void transferVoltageLevelsMicrocontrollerToSimulator();
    float m_sharedVoltageLineLevels[VoltageLine::Idx::Count];
};


#endif /* MICROCONTROLLER_H_ */
