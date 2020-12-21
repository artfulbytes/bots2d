#ifndef MICROCONTROLLER_SUMOBOT_4_WHEEL_EXAMPLE_H_
#define MICROCONTROLLER_SUMOBOT_4_WHEEL_EXAMPLE_H_

#include "components/MicrocontrollerCBinding.h"

class MicrocontrollerSumobot4WheelExample : public MicrocontrollerCBinding
{
public:
    MicrocontrollerSumobot4WheelExample(Microcontroller::VoltageLines &voltageLines);
    ~MicrocontrollerSumobot4WheelExample() {}
};

#endif /* MICROCONTROLLER_SUMOBOT_4_WHEEL_EXAMPLE_H_ */
