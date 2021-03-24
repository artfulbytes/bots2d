#include "microcontroller_c_functions.h"
#include "microcontroller_c_setup.h"
#include "components/Microcontroller.h"
#include <stddef.h>

static void *userdata_ptr = NULL;

void sleep_ms(uint32_t sleep_ms)
{
    physics_sleep(sleep_ms, userdata_ptr);
}

float get_voltage(int idx)
{
    return get_voltage_level(idx, userdata_ptr);
}

void set_voltage(int idx, float level)
{
    set_voltage_level(idx, level, userdata_ptr);
}

void set_userdata(void *userdata)
{
    userdata_ptr = userdata;
}

void sleep_ms_ud(uint32_t sleep_ms, void *userdata)
{
    physics_sleep(sleep_ms, userdata);
}

float get_voltage_ud(int idx, void *userdata)
{
    return get_voltage_level(idx, userdata);
}

void set_voltage_ud(int idx, float level, void *userdata)
{
    set_voltage_level(idx, level, userdata);
}
