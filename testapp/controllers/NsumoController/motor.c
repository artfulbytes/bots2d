#include "NsumoController/nsumo/drivers/motor.h"
#include "microcontroller_c_bindings.h"
#include "NsumoController/voltage_lines.h"
#include <assert.h>

#define VOLTAGE_PER_BATTERY_FULL_CHARGE (4.2f)
#define VOLTAGE_BATTERIES_FULL_CHARGE (2 * VOLTAGE_PER_BATTERY_FULL_CHARGE)
#define MAX_VOLTAGE_ALLOWED (6.0f)
#define LOSS_FACTOR (0.938f)
#define SCALE_FACTOR (MAX_VOLTAGE_ALLOWED / VOLTAGE_BATTERIES_FULL_CHARGE)
#define MAX_VOLTAGE (SCALE_FACTOR * LOSS_FACTOR * VOLTAGE_BATTERIES_FULL_CHARGE)

void motor_init() { }

void motor_set_duty_cycle(motors_t motors, int16_t duty_cycle)
{
    assert(-100 <= duty_cycle && duty_cycle <= 100);
    float motor_voltage = ((float)duty_cycle / 100.0f) * MAX_VOLTAGE;
    switch (motors) {
    case MOTORS_LEFT:
        set_voltage(VOLTAGE_FRONT_LEFT_MOTOR, motor_voltage);
        set_voltage(VOLTAGE_BACK_LEFT_MOTOR, motor_voltage);
        break;
    case MOTORS_RIGHT:
        set_voltage(VOLTAGE_FRONT_RIGHT_MOTOR, motor_voltage);
        set_voltage(VOLTAGE_BACK_RIGHT_MOTOR, motor_voltage);
        break;
    }
}

void motor_stop_safely() { }
