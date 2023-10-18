#include "NsumoController/NsumoMicrocontroller.h"

extern "C" {
#include "NsumoController/main_function.h"
}

NsumoMicrocontroller::NsumoMicrocontroller(Microcontroller::VoltageLines &voltageLines) :
    CMicrocontroller(voltageLines, _main)
{
}
