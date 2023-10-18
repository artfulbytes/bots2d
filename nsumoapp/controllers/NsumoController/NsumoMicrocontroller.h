#ifndef NSUMO_MICROCONTROLLER_H
#define NSUMO_MICROCONTROLLER_H

#include "components/CMicrocontroller.h"

class NsumoMicrocontroller : public CMicrocontroller
{
public:
    NsumoMicrocontroller(Microcontroller::VoltageLines &voltageLines);
    ~NsumoMicrocontroller() {}
};

#endif // NSUMO_MICROCONTROLLER_H
