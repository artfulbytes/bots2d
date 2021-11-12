#include "NsumoController/main_function.h"
#include "NsumoController/nsumo/trace.h"
#include "NsumoController/nsumo/state_machine.h"

void _main()
{
    trace_init();
    state_machine_run();
}
