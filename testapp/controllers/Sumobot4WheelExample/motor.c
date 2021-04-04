#include "Sumobot4WheelExample/mini-sumobot/drivers/motor.h"
#include "microcontroller_c_bindings.h"
#include "Sumobot4WheelExample/voltage_lines.h"
#include <assert.h>

#define MAX_VOLTAGE_MOTOR (6.0f);

void motor_init()
{
}

void motor_set_duty_cycle(motors_t motors, int16_t duty_cycle)
{
    assert(-100 <= duty_cycle && duty_cycle <= 100);
    float motor_voltage = ((float)duty_cycle / 100.0f) * MAX_VOLTAGE_MOTOR;
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

void motor_stop_safely()
{
}
