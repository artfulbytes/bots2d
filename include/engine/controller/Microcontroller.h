#ifndef MICROCONTROLLER_H_
#define MICROCONTROLLER_H_

#include "ControllerComponent.h"

class Microcontroller : public ControllerComponent
{
public:
    enum class LineType { Input, Output };
    enum class VoltageLine {
        A0 = 0, A1, A2, A3, A4, A5, A6, A7,
        B0,     B1, B2, B3, B4, B5, B6, B7,
        Count
    };
    struct VoltageLineConfig
    {
        LineType type;
        float *level;
    };

    virtual void onKeyEvent(const Event::Key &keyEvent);
    virtual void onFixedUpdate(double stepTime) = 0;
    void setVoltageLine(VoltageLine line, const VoltageLineConfig &config);

protected:
    VoltageLineConfig voltageLines[static_cast<int>(VoltageLine::Count)];

};

#endif /* MICROCONTROLLER_H_ */
