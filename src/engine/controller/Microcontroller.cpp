#include "Microcontroller.h"

#include <cassert>

void Microcontroller::onKeyEvent(const Event::Key &keyEvent)
{
    /* Microcontroller should typically not handle key events,
     * but don't make this method final, because it's
     * useful to override it when testing */
}

void Microcontroller::setVoltageLine(VoltageLine line, const VoltageLineConfig &config)
{
    assert(line != VoltageLine::Count);
    voltageLines[static_cast<int>(line)] = config;
}
