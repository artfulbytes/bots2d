#include "Sumobot4WheelExample/main_function.h"
#include "microcontroller_c_functions.h"
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

/* Must match with how lines are assigned in the C++ microcontroller class */
typedef enum voltage_idx {
    VOLTAGE_FRONT_LEFT_MOTOR = 0,
    VOLTAGE_BACK_LEFT_MOTOR = 1,
    VOLTAGE_FRONT_RIGHT_MOTOR = 2,
    VOLTAGE_BACK_RIGHT_MOTOR = 3,
    VOLTAGE_FRONT_LEFT_LINE_DETECTOR = 4,
    VOLTAGE_BACK_LEFT_LINE_DETECTOR = 5,
    VOLTAGE_FRONT_RIGHT_LINE_DETECTOR = 6,
    VOLTAGE_BACK_RIGHT_LINE_DETECTOR = 7,
    VOLTAGE_LEFT_RANGE_SENSOR = 8,
    VOLTAGE_FRONT_LEFT_RANGE_SENSOR = 9,
    VOLTAGE_FRONT_RANGE_SENSOR = 10,
    VOLTAGE_FRONT_RIGHT_RANGE_SENSOR = 11,
    VOLTAGE_RIGHT_RANGE_SENSOR = 12,
} voltage_idx_t;

typedef enum state {
    SEARCH,
    ATTACK,
    RETREAT
    // Evasive
} state_t;

typedef enum motor {
    MOTOR_FRONT_LEFT,
    MOTOR_BACK_LEFT,
    MOTOR_FRONT_RIGHT,
    MOTOR_BACK_RIGHT
} motor_t;

typedef enum line_detection {
    LINE_DETECTION_NONE,
    LINE_DETECTION_FRONT_LEFT,
    LINE_DETECTION_FRONT_RIGHT,
    LINE_DETECTION_BACK_LEFT,
    LINE_DETECTION_BACK_RIGHT,
    LINE_DETECTION_FRONT,
    LINE_DETECTION_BACK,
    LINE_DETECTION_LEFT,
    LINE_DETECTION_RIGHT,
} line_detection_t;


#define VOLTAGE_LINE_DETECTED (0.0f)
#define MAX_VOLTAGE_RANGE_SENSOR (1.0f)
#define MAX_VOLTAGE_MOTOR (6.0f);


line_detection_t get_line_detection()
{
    bool frontLeft = get_voltage(VOLTAGE_FRONT_LEFT_LINE_DETECTOR) > VOLTAGE_LINE_DETECTED;
    bool frontRight = get_voltage(VOLTAGE_FRONT_RIGHT_LINE_DETECTOR) > VOLTAGE_LINE_DETECTED;
    bool backLeft = get_voltage(VOLTAGE_BACK_LEFT_LINE_DETECTOR) > VOLTAGE_LINE_DETECTED;
    bool backRight = get_voltage(VOLTAGE_BACK_LEFT_LINE_DETECTOR) > VOLTAGE_LINE_DETECTED;

    if (frontLeft) {
        if (frontRight) {
            return LINE_DETECTION_FRONT;
        } else if (backLeft) {
            return LINE_DETECTION_LEFT;
        } else {
            return LINE_DETECTION_FRONT_LEFT;
        }
    } else if (frontRight) {
        if (backRight) {
            return LINE_DETECTION_RIGHT;
        } else {
            return LINE_DETECTION_FRONT_RIGHT;
        }
    } else if (backLeft) {
        if (backRight) {
            return LINE_DETECTION_BACK;
        } else {
            return LINE_DETECTION_BACK_LEFT;
        }
    } else if (backRight) {
        return LINE_DETECTION_BACK_RIGHT;
    }
    return LINE_DETECTION_NONE;
}

bool enemy_detected_in_front()
{
    return get_voltage(VOLTAGE_FRONT_RANGE_SENSOR) < MAX_VOLTAGE_RANGE_SENSOR;
}

void set_motor_power(motor_t motor, int power)
{
    assert(-100 <= power && power <= 100);
    float motor_voltage = ((float)power / 100.0f) * MAX_VOLTAGE_MOTOR;
    switch (motor) {
    case MOTOR_FRONT_LEFT:
        set_voltage(VOLTAGE_FRONT_LEFT_MOTOR, motor_voltage);
        break;
    case MOTOR_BACK_LEFT:
        set_voltage(VOLTAGE_BACK_LEFT_MOTOR, motor_voltage);
        break;
    case MOTOR_FRONT_RIGHT:
        set_voltage(VOLTAGE_FRONT_RIGHT_MOTOR, motor_voltage);
        break;
    case MOTOR_BACK_RIGHT:
        set_voltage(VOLTAGE_BACK_RIGHT_MOTOR, motor_voltage);
        break;
    }
}

typedef enum drive
{
    STOP,
    FORWARD,
    REVERSE,
    ROTATE_LEFT,
    ROTATE_RIGHT
} drive_t;

void set_drive(drive_t drive)
{
    switch (drive)
    {
    case STOP:
        set_motor_power(MOTOR_FRONT_LEFT, 0);
        set_motor_power(MOTOR_BACK_LEFT, 0);
        set_motor_power(MOTOR_FRONT_RIGHT, 0);
        set_motor_power(MOTOR_BACK_RIGHT, 0);
        break;
    case FORWARD:
        set_motor_power(MOTOR_FRONT_LEFT, 100);
        set_motor_power(MOTOR_BACK_LEFT, 100);
        set_motor_power(MOTOR_FRONT_RIGHT, 100);
        set_motor_power(MOTOR_BACK_RIGHT, 100);
        break;
    case REVERSE:
        set_motor_power(MOTOR_FRONT_LEFT, -100);
        set_motor_power(MOTOR_BACK_LEFT, -100);
        set_motor_power(MOTOR_FRONT_RIGHT, -100);
        set_motor_power(MOTOR_BACK_RIGHT, -100);
        break;
    case ROTATE_LEFT:
        set_motor_power(MOTOR_FRONT_LEFT, -50);
        set_motor_power(MOTOR_BACK_LEFT, -50);
        set_motor_power(MOTOR_FRONT_RIGHT, 50);
        set_motor_power(MOTOR_BACK_RIGHT, 50);
        break;
    case ROTATE_RIGHT:
        set_motor_power(MOTOR_FRONT_LEFT, 50);
        set_motor_power(MOTOR_BACK_LEFT, 50);
        set_motor_power(MOTOR_FRONT_RIGHT, -50);
        set_motor_power(MOTOR_BACK_RIGHT, -50);
        break;
    }
}

bool enemy_out_of_ring() {
    return true;
    //return enemy_detected_in_front() && line_detected_in_front();
}

/* TODO: The commands should be in terms of rough rotate angles instead... (sleep times may differ between targets...) */
void retreat_manuever(line_detection_t line_detection)
{
    switch (line_detection) {
    case LINE_DETECTION_NONE:
        break;
    case LINE_DETECTION_FRONT_LEFT:
        set_drive(REVERSE);
        sleep_ms(300);
        break;
    case LINE_DETECTION_FRONT_RIGHT:
        set_drive(REVERSE);
        sleep_ms(300);
        break;
    case LINE_DETECTION_BACK_LEFT:
        set_drive(FORWARD);
        sleep_ms(300);
        break;
    case LINE_DETECTION_BACK_RIGHT:
        set_drive(FORWARD);
        sleep_ms(300);
        break;
    case LINE_DETECTION_FRONT:
        set_drive(REVERSE);
        sleep_ms(300);
        break;
    case LINE_DETECTION_BACK:
        set_drive(FORWARD);
        sleep_ms(300);
        break;
    case LINE_DETECTION_LEFT:
        set_drive(ROTATE_RIGHT);
        sleep_ms(100);
        set_drive(FORWARD);
        sleep_ms(150);
        break;
    case LINE_DETECTION_RIGHT:
        set_drive(ROTATE_LEFT);
        sleep_ms(100);
        set_drive(FORWARD);
        sleep_ms(150);
        break;
    }
}

state_t currentState = SEARCH;

void _main()
{
    while(true) {
        sleep_ms(1); // Sleep a bit to offload the host CPU :)
        state_t nextState = currentState;
        line_detection_t line_detection = get_line_detection();
        switch (currentState)
        {
        case SEARCH:
            if (line_detection != LINE_DETECTION_NONE) {
                nextState = RETREAT;
                break;
            }
            if (enemy_detected_in_front()) {
                nextState = ATTACK;
                set_drive(STOP);
                break;
            }
            set_drive(ROTATE_RIGHT);
            /* Enemy in front -> go to attack? */
            /* Enemy on left side? -> rotate right */
            /* Enemy on right side? -> rotate left */
            /* No enemy? -> rotate right */
            break;
        case ATTACK:
            if (line_detection != LINE_DETECTION_NONE) {
                nextState = RETREAT;
                set_drive(STOP);
                break;
            }
            if (enemy_detected_in_front()) {
                set_drive(FORWARD);
            } else {
                set_drive(STOP);
                nextState = SEARCH;
            }
            break;
        case RETREAT:
            /* Should handle having an enemy and a line detected... */
            retreat_manuever(line_detection);
            /* Can we avoid retrieving line here again? */
            line_detection = get_line_detection();
            if (line_detection == LINE_DETECTION_NONE) {
                nextState = SEARCH;
            }
            break;
        }
        (void)nextState;

        if (enemy_out_of_ring()) {
        }

        if (nextState != currentState) {
            currentState = nextState;
        }
    }
}
