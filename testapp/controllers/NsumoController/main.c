#include "NsumoController/main_function.h"
#include "NsumoController/nsumo/trace.h"
#include "NsumoController/nsumo/state_machine/state_machine.h"
#include "NsumoController/nsumo/test.h"

void _main()
{
    trace_init();
    //test_enemy_detection_print();
    state_machine_run();
    //test_drive_duty_cycles();
    //test_drive_and_line_detect();
    //test_rotate_and_enemy_detect();
}
