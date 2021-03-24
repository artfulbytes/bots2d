#ifndef C_MICROCONTROLLER_SUMOBOT_4W_EXAMPLE_H_
#define C_MICROCONTROLLER_SUMOBOT_4W_EXAMPLE_H_

#include "components/CMicrocontroller.h"

/**
 * Example class for demonstrating how to use CMicrcontroller.
 */
class CMicrocontrollerSumobot4WExample : public CMicrocontroller
{
public:
    CMicrocontrollerSumobot4WExample(Microcontroller::VoltageLines &voltageLines);
    ~CMicrocontrollerSumobot4WExample() {}
};

#endif /* C_MICROCONTROLLER_SUMOBOT_4W_EXAMPLE_H_ */
