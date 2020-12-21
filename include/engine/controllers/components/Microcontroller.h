#ifndef MICROCONTROLLER_H_
#define MICROCONTROLLER_H_

#include "ControllerComponent.h"
#include <array>
#include <atomic>
#include <thread>
#include <mutex>

class Microcontroller : public ControllerComponent
{
public:
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

    /* Must keep track of voltage line index to ensure they match with controller code */
    Microcontroller(VoltageLines &voltageLines);
    ~Microcontroller();
    void start();

    void onFixedUpdate(double stepTime) override final;

    virtual void onKeyEvent(const Event::Key &keyEvent);

protected:
    /* The voltage line levels the microcontroller loop writes to and read from */
    float m_microcontrollerVoltageLineLevels[VoltageLine::Idx::Count];

private:
    virtual void microcontrollerUpdate() = 0;

    /* To prevent the simulator's update rate from depending on the microcontroller's update rate, offload the
     * microcontroller's update loop to a separate thread. */
    void microcontrollerLoop();
    std::thread m_thread;
    std::atomic<bool> m_running = true;

    /* The voltage lines the simulator objects (e.g. sumobot, wheel motor) writes to and read from */
    VoltageLines m_simulatorVoltageLines;

    /* To give the simulator loop and microcontroller loop freedom to access the voltage lines whenever
     * they want while avoiding race conditions, each loop access its own array of voltage lines. The
     * shared array is used to safely (with a mutex) synchronize them. */
    std::mutex m_voltageLinesMutex;
    void transferVoltageLevelsSimulatorToMicrocontroller();
    void transferVoltageLevelsMicrocontrollerToSimulator();
    float m_sharedVoltageLineLevels[VoltageLine::Idx::Count];
};


#endif /* MICROCONTROLLER_H_ */
