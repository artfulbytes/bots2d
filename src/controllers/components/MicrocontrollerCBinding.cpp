#include "components/MicrocontrollerCBinding.h"

extern "C" {
#include "microcontroller_c_setup.h"
}

MicrocontrollerCBinding::MicrocontrollerCBinding(Microcontroller::VoltageLines &voltageLines, main_function mainFcn) :
    Microcontroller(voltageLines),
    m_mainFcn(mainFcn)
{
    set_userdata(this);
}

MicrocontrollerCBinding::MicrocontrollerCBinding(Microcontroller::VoltageLines &voltageLines, main_function_userdata mainFcnUserdata) :
    Microcontroller(voltageLines),
    m_mainFcnUserdata(mainFcnUserdata)
{
}

void MicrocontrollerCBinding::main()
{
    if (m_mainFcn) {
        m_mainFcn();
    } else {
        m_mainFcnUserdata(this);
    }
}

MicrocontrollerCBinding::~MicrocontrollerCBinding()
{
}
