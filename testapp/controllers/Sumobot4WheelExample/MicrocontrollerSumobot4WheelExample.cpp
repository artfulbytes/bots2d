#include "Sumobot4WheelExample/MicrocontrollerSumobot4WheelExample.h"

extern "C" {
#include "Sumobot4WheelExample/main_function.h"
}

MicrocontrollerSumobot4WheelExample::MicrocontrollerSumobot4WheelExample(Microcontroller::VoltageLines &voltageLines) :
    MicrocontrollerCBinding(voltageLines, _main)
{
}
