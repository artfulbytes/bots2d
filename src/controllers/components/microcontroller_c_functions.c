#include "microcontroller_c_functions.h"
#include "microcontroller_c_setup.h"
#include <stddef.h>

void *userdata_ptr = NULL;
get_voltage_function get_voltage_func_ptr = NULL;
set_voltage_function set_voltage_func_ptr = NULL;
sleep_function sleep_func_ptr = NULL;

void sleep_ms(uint32_t sleep_ms)
{
    sleep_func_ptr(sleep_ms, userdata_ptr);
}

float get_voltage(int idx)
{
    return get_voltage_func_ptr(idx, userdata_ptr);
}

void set_voltage(int idx, float level)
{
    set_voltage_func_ptr(idx, level, userdata_ptr);
}

void setup(get_voltage_function get_voltage_fcn, set_voltage_function set_voltage_fcn, sleep_function sleep_fcn, void *userdata)
{
    sleep_func_ptr = sleep_fcn;
    get_voltage_func_ptr= get_voltage_fcn;
    set_voltage_func_ptr = set_voltage_fcn;
    userdata_ptr = userdata;
}
