#include "common/assert_handler.h"
#include "common/defines.h"
#include "common/trace.h"

void assert_handler(uint16_t program_counter)
{
    UNUSED(program_counter);
    TRACE("Assert");
    while(1);
}
