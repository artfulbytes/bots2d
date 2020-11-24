#ifndef MICROCONTROLLER_H_
#define MICROCONTROLLER_H_

#include "ControllerComponent.h"
#include <array>

class Microcontroller : public ControllerComponent
{
public:
    enum VoltageLine {
        VOLTAGE_LINE_A0 = 0,
        VOLTAGE_LINE_A1,
        VOLTAGE_LINE_A2,
        VOLTAGE_LINE_A3,
        VOLTAGE_LINE_A4,
        VOLTAGE_LINE_A5,
        VOLTAGE_LINE_A6,
        VOLTAGE_LINE_A7,
        VOLTAGE_LINE_B0,
        VOLTAGE_LINE_B1,
        VOLTAGE_LINE_B2,
        VOLTAGE_LINE_B3,
        VOLTAGE_LINE_B4,
        VOLTAGE_LINE_B5,
        VOLTAGE_LINE_B6,
        VOLTAGE_LINE_B7,
        VOLTAGE_LINE_COUNT
    };
    typedef std::array<float *, Microcontroller::VOLTAGE_LINE_COUNT> VoltageLineArray;

    /* Must keep track of voltage line index to ensure they match with controller code */
    Microcontroller(VoltageLineArray &voltageLines);
    virtual void onKeyEvent(const Event::Key &keyEvent);
    virtual void onFixedUpdate(double stepTime) = 0;

protected:
    VoltageLineArray m_voltageLines;

};


#endif /* MICROCONTROLLER_H_ */
