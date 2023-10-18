#include "drivers/vl53l0x.h"
#include "microcontroller_c_bindings.h"
#include "NsumoController/voltage_lines.h"

// Should match max range of the sumobot spec
#define VOLTAGE_TO_RANGE (800u)

vl53l0x_result_e vl53l0x_read_range_multiple(vl53l0x_ranges_t ranges, bool *fresh_values)
{
    ranges[VL53L0X_IDX_LEFT]= get_voltage(VOLTAGE_LEFT_RANGE_SENSOR) * VOLTAGE_TO_RANGE;
    ranges[VL53L0X_IDX_FRONT_LEFT] = get_voltage(VOLTAGE_FRONT_LEFT_RANGE_SENSOR) * VOLTAGE_TO_RANGE;
    ranges[VL53L0X_IDX_FRONT] = get_voltage(VOLTAGE_FRONT_RANGE_SENSOR) * VOLTAGE_TO_RANGE;
    ranges[VL53L0X_IDX_FRONT_RIGHT] = get_voltage(VOLTAGE_FRONT_RIGHT_RANGE_SENSOR) * VOLTAGE_TO_RANGE;
    ranges[VL53L0X_IDX_RIGHT] = get_voltage(VOLTAGE_RIGHT_RANGE_SENSOR) * VOLTAGE_TO_RANGE;
    *fresh_values = true;
    return VL53L0X_RESULT_OK;
}

vl53l0x_result_e vl53l0x_init(void)
{
    return VL53L0X_RESULT_OK;
}
