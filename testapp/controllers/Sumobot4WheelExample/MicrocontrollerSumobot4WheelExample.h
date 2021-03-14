#ifndef MICROCONTROLLER_SUMOBOT_4_WHEEL_EXAMPLE_H_
#define MICROCONTROLLER_SUMOBOT_4_WHEEL_EXAMPLE_H_

#include "components/MicrocontrollerCBinding.h"

/**
 * Example class for demonstrating how to use MicrocontrollerCBinding.
 */
class MicrocontrollerSumobot4WheelExample : public MicrocontrollerCBinding
{
public:
    MicrocontrollerSumobot4WheelExample(Microcontroller::VoltageLines &voltageLines, unsigned int updateRateHz);
    ~MicrocontrollerSumobot4WheelExample() {}
};

#endif /* MICROCONTROLLER_SUMOBOT_4_WHEEL_EXAMPLE_H_ */
