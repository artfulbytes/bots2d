#include "components/CMicrocontroller.h"

extern "C" {
#include "microcontroller_c_setup.h"
}

CMicrocontroller::CMicrocontroller(Microcontroller::VoltageLines &voltageLines, main_function mainFcn) :
    Microcontroller(voltageLines),
    m_mainFcn(mainFcn)
{
    set_userdata(this);
}

CMicrocontroller::CMicrocontroller(Microcontroller::VoltageLines &voltageLines, main_function_userdata mainFcnUserdata) :
    Microcontroller(voltageLines),
    m_mainFcnUserdata(mainFcnUserdata)
{
}

void CMicrocontroller::main()
{
    if (m_mainFcn) {
        m_mainFcn();
    } else {
        m_mainFcnUserdata(this);
    }
}

CMicrocontroller::~CMicrocontroller()
{
}
