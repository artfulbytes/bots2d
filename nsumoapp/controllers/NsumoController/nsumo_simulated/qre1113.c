#include "drivers/qre1113.h"
#include "NsumoController/voltage_lines.h"
#include "microcontroller_c_bindings.h"

#define LINE_SENSOR_VOLTAGE_THRESHOLD (700u)
#define LINE_NOT_DETECTED LINE_SENSOR_VOLTAGE_THRESHOLD
#define LINE_DETECTED (0u)

void qre1113_get_voltages(struct qre1113_voltages *voltages)
{
    voltages->front_left = get_voltage(VOLTAGE_FRONT_LEFT_LINE_DETECTOR) ? LINE_DETECTED : LINE_NOT_DETECTED;
    voltages->front_right = get_voltage(VOLTAGE_FRONT_RIGHT_LINE_DETECTOR) ? LINE_DETECTED : LINE_NOT_DETECTED;
    voltages->back_left = get_voltage(VOLTAGE_BACK_LEFT_LINE_DETECTOR) ? LINE_DETECTED : LINE_NOT_DETECTED;
    voltages->back_right = get_voltage(VOLTAGE_BACK_RIGHT_LINE_DETECTOR) ? LINE_DETECTED : LINE_NOT_DETECTED;
}

void qre1113_init(void)
{
}
