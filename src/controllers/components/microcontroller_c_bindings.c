#include "microcontroller_c_bindings.h"
#include "microcontroller_c_setup.h"
#include "components/Microcontroller.h"
#include <stddef.h>

/** Can only be used by a single C microcontroller at a time */
static void *userdata_ptr = NULL;

void set_userdata(void *userdata)
{
    userdata_ptr = userdata;
}

float get_voltage(int idx)
{
    return get_voltage_cb(idx, userdata_ptr);
}

void set_voltage(int idx, float level)
{
    set_voltage_cb(idx, level, userdata_ptr);
}

void sleep_ms(uint32_t ms)
{
    ms_sleep_cb(ms, userdata_ptr);
}

float get_voltage_ud(int idx, void *userdata)
{
    return get_voltage_cb(idx, userdata);
}

void set_voltage_ud(int idx, float level, void *userdata)
{
    set_voltage_cb(idx, level, userdata);
}

void sleep_ms_ud(uint32_t ms, void *userdata)
{
    ms_sleep_cb(ms, userdata);
}

uint32_t ms_elapsed(void *userdata)
{
    return ms_elapsed_cb(userdata);
}
