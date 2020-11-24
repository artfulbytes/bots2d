#include "sumobot4wheelexample/MicrocontrollerSumobot4WheelExample.h"

extern "C" {
#include "sumobot4wheelexample/c_bindings.h"
}

MicrocontrollerSumobot4WheelExample::MicrocontrollerSumobot4WheelExample(Microcontroller::VoltageLineArray &voltageLines) :
    MicrocontrollerCBinding(voltageLines, setup, loop)
{
}
