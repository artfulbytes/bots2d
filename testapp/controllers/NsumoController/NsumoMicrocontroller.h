#ifndef NSUMO_MICROCONTROLLER_H_
#define NSUMO_MICROCONTROLLER_H_

#include "components/CMicrocontroller.h"

class NsumoMicrocontroller : public CMicrocontroller
{
public:
    NsumoMicrocontroller(Microcontroller::VoltageLines &voltageLines);
    ~NsumoMicrocontroller() {}
};

#endif /* NSUMO_MICROCONTROLLER_H_ */
