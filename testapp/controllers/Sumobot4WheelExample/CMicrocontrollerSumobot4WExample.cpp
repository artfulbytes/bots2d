#include "Sumobot4WheelExample/CMicrocontrollerSumobot4WExample.h"

extern "C" {
#include "Sumobot4WheelExample/main_function.h"
}

CMicrocontrollerSumobot4WExample::CMicrocontrollerSumobot4WExample(Microcontroller::VoltageLines &voltageLines) :
    CMicrocontroller(voltageLines, _main)
{
}
