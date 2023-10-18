#include "drivers/tb6612fng.h"
#include "microcontroller_c_bindings.h"
#include "NsumoController/voltage_lines.h"
#include <assert.h>

#define VOLTAGE_PER_BATTERY_FULL_CHARGE (4.2f)
#define VOLTAGE_BATTERIES_FULL_CHARGE (2 * VOLTAGE_PER_BATTERY_FULL_CHARGE)
#define MAX_VOLTAGE_ALLOWED (6.0f)
#define LOSS_FACTOR (0.938f)
#define SCALE_FACTOR (MAX_VOLTAGE_ALLOWED / VOLTAGE_BATTERIES_FULL_CHARGE)
#define MAX_VOLTAGE (SCALE_FACTOR * LOSS_FACTOR * VOLTAGE_BATTERIES_FULL_CHARGE)

static uint8_t left_duty_cycle = 0;
static uint8_t right_duty_cycle = 0;
static tb6612fng_mode_e left_mode;
static tb6612fng_mode_e right_mode;

static float duty_cycle_to_voltage(uint8_t duty_cycle)
{
    return ((float)duty_cycle / 100.0f) * MAX_VOLTAGE;
}

void tb6612fng_set_mode(tb6612fng_e tb, tb6612fng_mode_e mode)
{
    switch(mode)
    {
    case TB6612FNG_MODE_STOP:
        switch (tb) {
        case TB6612FNG_LEFT:
            set_voltage(VOLTAGE_FRONT_LEFT_MOTOR, 0);
            set_voltage(VOLTAGE_BACK_LEFT_MOTOR, 0);
            break;
        case TB6612FNG_RIGHT:
            set_voltage(VOLTAGE_FRONT_RIGHT_MOTOR, 0);
            set_voltage(VOLTAGE_BACK_RIGHT_MOTOR, 0);
            break;
        }
        break;
    case TB6612FNG_MODE_FORWARD:
        switch (tb) {
        case TB6612FNG_LEFT:
            set_voltage(VOLTAGE_FRONT_LEFT_MOTOR, duty_cycle_to_voltage(left_duty_cycle));
            set_voltage(VOLTAGE_BACK_LEFT_MOTOR, duty_cycle_to_voltage(left_duty_cycle));
            break;
        case TB6612FNG_RIGHT:
            set_voltage(VOLTAGE_FRONT_RIGHT_MOTOR, duty_cycle_to_voltage(right_duty_cycle));
            set_voltage(VOLTAGE_BACK_RIGHT_MOTOR, duty_cycle_to_voltage(right_duty_cycle));
            break;
        }
        break;
    case TB6612FNG_MODE_REVERSE:
        switch (tb) {
        case TB6612FNG_LEFT:
            set_voltage(VOLTAGE_FRONT_LEFT_MOTOR, -duty_cycle_to_voltage(left_duty_cycle));
            set_voltage(VOLTAGE_BACK_LEFT_MOTOR, -duty_cycle_to_voltage(left_duty_cycle));
            break;
        case TB6612FNG_RIGHT:
            set_voltage(VOLTAGE_FRONT_RIGHT_MOTOR, -duty_cycle_to_voltage(right_duty_cycle));
            set_voltage(VOLTAGE_BACK_RIGHT_MOTOR, -duty_cycle_to_voltage(right_duty_cycle));
            break;
        }
        break;
    }
    switch (tb) {
    case TB6612FNG_LEFT:
        left_mode = mode;
        break;
    case TB6612FNG_RIGHT:
        right_mode = mode;
        break;
    }
}

void tb6612fng_set_pwm(tb6612fng_e tb, uint8_t duty_cycle)
{
    assert(duty_cycle <= 100);
    switch (tb)
    {
    case TB6612FNG_LEFT:
        left_duty_cycle = duty_cycle;
        tb6612fng_set_mode(tb, left_mode);
        break;
    case TB6612FNG_RIGHT:
        right_duty_cycle = duty_cycle;
        tb6612fng_set_mode(tb, right_mode);
        break;
    }
}

void tb6612fng_init()
{
}
