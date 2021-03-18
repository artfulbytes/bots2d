#include "Sumobot4WheelExample/MicrocontrollerSumobot4WheelExample.h"

extern "C" {
#include "Sumobot4WheelExample/c_bindings.h"
}

MicrocontrollerSumobot4WheelExample::MicrocontrollerSumobot4WheelExample(Microcontroller::VoltageLines &voltageLines) :
    MicrocontrollerCBinding(voltageLines, setup, loop)
{
}
