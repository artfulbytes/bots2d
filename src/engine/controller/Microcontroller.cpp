#include "Microcontroller.h"

#include <cassert>

Microcontroller::Microcontroller(Microcontroller::VoltageLineArray &voltageLines) :
    m_voltageLines(voltageLines)
{
}

void Microcontroller::onKeyEvent(const Event::Key &keyEvent)
{
    /* Microcontroller should typically not handle key events,
     * but don't make this method final, because it's
     * useful to override it when testing */
}

