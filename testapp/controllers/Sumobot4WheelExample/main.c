#include "Sumobot4WheelExample/main_function.h"
#include "microcontroller_c_bindings.h"
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#define LINE_SENSOR_VOLTAGE_THRESHOLD (0.0f)
#define MAX_VOLTAGE_RANGE_SENSOR (1.0f)
#define MAX_VOLTAGE_MOTOR (6.0f);

#if 0
static const char *drive_str(drive_t drive)
{
    switch (drive)
    {
    case STOP: return "STOP";
    case FORWARD: return "FORWARD";
    case REVERSE: return "REVERSE";
    case ROTATE_LEFT: return "ROTATE_LEFT";
    case ROTATE_RIGHT: return "ROTATE_RIGHT";
    case ARCTURN_LEFT: return "ARCTURN_LEFT";
    case ARCTURN_RIGHT: return "ARCTURN_RIGHT";
    }
    return "";
}
#endif

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

typedef enum drive {
    DRIVE_STOP,
    DRIVE_FORWARD,
    DRIVE_REVERSE,
    DRIVE_ROTATE_LEFT,
    DRIVE_ROTATE_RIGHT,
    DRIVE_ARCTURN_LEFT,
    DRIVE_ARCTURN_RIGHT
} drive_t;

/* Enum can be used as bitwise flags */
typedef enum enemy_detection {
    ENEMY_DETECTION_LEFT = 0,
    ENEMY_DETECTION_FRONT_LEFT = 1,
    ENEMY_DETECTION_FRONT = 2,
    ENEMY_DETECTION_FRONT_RIGHT = 4,
    ENEMY_DETECTION_RIGHT = 8
} enemy_detection_t;

typedef enum retreat_manuever {
    RETREAT_REVERSE_LEFT,
    RETREAT_REVERSE_RIGHT,
    RETREAT_FORWARD_LEFT,
    RETREAT_FORWARD_RIGHT,
    RETREAT_LEFT,
    RETREAT_RIGHT,
    RETREAT_NONE
} retreat_manuever_t;

typedef enum state {
    STATE_SEARCH_1,
    STATE_SEARCH_2,
    STATE_ATTACK,
    STATE_RETREAT
} state_t;

/* Move to motor driver file */
void motor_set_duty_cycle(motor_t motor, int32_t duty_cycle, void *userdata)
{
    assert(-100 <= duty_cycle && duty_cycle <= 100);
    float motor_voltage = ((float)duty_cycle / 100.0f) * MAX_VOLTAGE_MOTOR;
    switch (motor) {
    case MOTOR_FRONT_LEFT:
        set_voltage_ud(VOLTAGE_FRONT_LEFT_MOTOR, motor_voltage, userdata);
        break;
    case MOTOR_BACK_LEFT:
        set_voltage_ud(VOLTAGE_BACK_LEFT_MOTOR, motor_voltage, userdata);
        break;
    case MOTOR_FRONT_RIGHT:
        set_voltage_ud(VOLTAGE_FRONT_RIGHT_MOTOR, motor_voltage, userdata);
        break;
    case MOTOR_BACK_RIGHT:
        set_voltage_ud(VOLTAGE_BACK_RIGHT_MOTOR, motor_voltage, userdata);
        break;
    }
}

line_detection_t get_line_detection(void *userdata)
{
    const bool frontLeft = get_voltage_ud(VOLTAGE_FRONT_LEFT_LINE_DETECTOR, userdata) > LINE_SENSOR_VOLTAGE_THRESHOLD;
    const bool frontRight = get_voltage_ud(VOLTAGE_FRONT_RIGHT_LINE_DETECTOR, userdata) > LINE_SENSOR_VOLTAGE_THRESHOLD;
    const bool backLeft = get_voltage_ud(VOLTAGE_BACK_LEFT_LINE_DETECTOR, userdata) > LINE_SENSOR_VOLTAGE_THRESHOLD;
    const bool backRight = get_voltage_ud(VOLTAGE_BACK_RIGHT_LINE_DETECTOR, userdata) > LINE_SENSOR_VOLTAGE_THRESHOLD;

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
            //printf("DETECTED FRONT RIGHT\n");
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

uint8_t get_enemy_detection(void *userdata)
{
    const bool detectedLeft = get_voltage_ud(VOLTAGE_LEFT_RANGE_SENSOR, userdata) < MAX_VOLTAGE_RANGE_SENSOR;
    const bool detectedFrontLeft = get_voltage_ud(VOLTAGE_FRONT_LEFT_RANGE_SENSOR, userdata) < MAX_VOLTAGE_RANGE_SENSOR;
    const bool detectedFront = get_voltage_ud(VOLTAGE_FRONT_RANGE_SENSOR, userdata) < MAX_VOLTAGE_RANGE_SENSOR;
    const bool detectedFrontRight = get_voltage_ud(VOLTAGE_FRONT_RIGHT_RANGE_SENSOR, userdata) < MAX_VOLTAGE_RANGE_SENSOR;
    const bool detectedRight = get_voltage_ud(VOLTAGE_RIGHT_RANGE_SENSOR, userdata) < MAX_VOLTAGE_RANGE_SENSOR;

    uint8_t enemyDetected = 0;
    if (detectedLeft) {
        enemyDetected |= ENEMY_DETECTION_LEFT;
    }
    if (detectedFrontLeft) {
        enemyDetected |= ENEMY_DETECTION_FRONT_LEFT;
    }
    if (detectedFront) {
        enemyDetected |= ENEMY_DETECTION_FRONT;
    }
    if (detectedFrontRight) {
        enemyDetected |= ENEMY_DETECTION_FRONT_RIGHT;
    }
    if (detectedRight) {
        enemyDetected |= ENEMY_DETECTION_RIGHT;
    }
    return enemyDetected;
}

void set_drive(drive_t drive, void *userdata)
{
    switch (drive)
    {
    case DRIVE_STOP:
        motor_set_duty_cycle(MOTOR_FRONT_RIGHT, 0, userdata);
        motor_set_duty_cycle(MOTOR_BACK_RIGHT, 0, userdata);
        motor_set_duty_cycle(MOTOR_FRONT_LEFT, 0, userdata);
        motor_set_duty_cycle(MOTOR_BACK_LEFT, 0, userdata);
        break;
    case DRIVE_FORWARD:
        motor_set_duty_cycle(MOTOR_FRONT_LEFT, 100, userdata);
        motor_set_duty_cycle(MOTOR_BACK_LEFT, 100, userdata);
        motor_set_duty_cycle(MOTOR_FRONT_RIGHT, 100, userdata);
        motor_set_duty_cycle(MOTOR_BACK_RIGHT, 100, userdata);
        break;
    case DRIVE_REVERSE:
        motor_set_duty_cycle(MOTOR_FRONT_LEFT, -100, userdata);
        motor_set_duty_cycle(MOTOR_BACK_LEFT, -100, userdata);
        motor_set_duty_cycle(MOTOR_FRONT_RIGHT, -100, userdata);
        motor_set_duty_cycle(MOTOR_BACK_RIGHT, -100, userdata);
        break;
    case DRIVE_ROTATE_LEFT:
        motor_set_duty_cycle(MOTOR_FRONT_LEFT, -20, userdata);
        motor_set_duty_cycle(MOTOR_BACK_LEFT, -20, userdata);
        motor_set_duty_cycle(MOTOR_FRONT_RIGHT, 20, userdata);
        motor_set_duty_cycle(MOTOR_BACK_RIGHT, 20, userdata);
        break;
    case DRIVE_ROTATE_RIGHT:
        motor_set_duty_cycle(MOTOR_FRONT_LEFT, 20, userdata);
        motor_set_duty_cycle(MOTOR_BACK_LEFT, 20, userdata);
        motor_set_duty_cycle(MOTOR_FRONT_RIGHT, -20, userdata);
        motor_set_duty_cycle(MOTOR_BACK_RIGHT, -20, userdata);
        break;
    case DRIVE_ARCTURN_LEFT:
        motor_set_duty_cycle(MOTOR_FRONT_LEFT, 30, userdata);
        motor_set_duty_cycle(MOTOR_BACK_LEFT, 30, userdata);
        motor_set_duty_cycle(MOTOR_FRONT_RIGHT, 80, userdata);
        motor_set_duty_cycle(MOTOR_BACK_RIGHT, 80, userdata);
        break;
    case DRIVE_ARCTURN_RIGHT:
        motor_set_duty_cycle(MOTOR_FRONT_LEFT, 80, userdata);
        motor_set_duty_cycle(MOTOR_BACK_LEFT, 80, userdata);
        motor_set_duty_cycle(MOTOR_FRONT_RIGHT, 30, userdata);
        motor_set_duty_cycle(MOTOR_BACK_RIGHT, 30, userdata);
        break;
    }
}

bool is_enemy_out(line_detection_t line_detection, enemy_detection_t enemy_detection)
{
    return (enemy_detection & (ENEMY_DETECTION_FRONT | ENEMY_DETECTION_FRONT_LEFT | ENEMY_DETECTION_FRONT_RIGHT)) &&
           (line_detection == LINE_DETECTION_FRONT);
}

/* TODO: Create shared tracing (different levels) function between MCU and simulation */
/* TODO: Create functions to print the enum values */
retreat_manuever_t get_retreat_manuever(line_detection_t line_detection)
{
    switch (line_detection) {
    case LINE_DETECTION_FRONT:
    case LINE_DETECTION_FRONT_LEFT:
        return RETREAT_REVERSE_RIGHT;
    case LINE_DETECTION_FRONT_RIGHT:
        return RETREAT_REVERSE_LEFT;
    case LINE_DETECTION_BACK_LEFT:
        return RETREAT_FORWARD_RIGHT;
    case LINE_DETECTION_BACK:
    case LINE_DETECTION_BACK_RIGHT:
        return RETREAT_FORWARD_LEFT;
    case LINE_DETECTION_LEFT:
        return RETREAT_RIGHT;
    case LINE_DETECTION_RIGHT:
        return RETREAT_LEFT;
    case LINE_DETECTION_NONE:
        break;
    }
    assert(0);
    return RETREAT_NONE;
}

#define RETREAT_FORWARD_TIME_MS (300)
#define RETREAT_REVERSE_TIME_MS RETREAT_FORWARD_TIME_MS
#define RETREAT_ROTATE_TIME_MS (150)
#define RETREAT_ARCTURN_TIME_MS (300)

/* Set drive for the current retreat manuever */
bool do_retreat_manuever(retreat_manuever_t manuever, uint32_t time_in_manuever, void *userdata)
{
    switch (manuever) {
    case RETREAT_FORWARD_LEFT:
        if (time_in_manuever < RETREAT_FORWARD_TIME_MS) {
            set_drive(DRIVE_FORWARD, userdata);
            return true;
        } else if (time_in_manuever < RETREAT_FORWARD_TIME_MS + RETREAT_ROTATE_TIME_MS) {
            set_drive(DRIVE_ROTATE_LEFT, userdata);
            return true;
        }
        break;
    case RETREAT_FORWARD_RIGHT:
        if (time_in_manuever < RETREAT_FORWARD_TIME_MS) {
            set_drive(DRIVE_FORWARD, userdata);
            return true;
        } else if (time_in_manuever < RETREAT_FORWARD_TIME_MS + RETREAT_ROTATE_TIME_MS) {
            set_drive(DRIVE_ROTATE_RIGHT, userdata);
            return true;
        }
        break;
    case RETREAT_REVERSE_LEFT:
        if (time_in_manuever < RETREAT_REVERSE_TIME_MS) {
            set_drive(DRIVE_REVERSE, userdata);
            return true;
        } else if (time_in_manuever < RETREAT_REVERSE_TIME_MS + RETREAT_ROTATE_TIME_MS) {
            set_drive(DRIVE_ROTATE_LEFT, userdata);
            return true;
        }
        break;
    case RETREAT_REVERSE_RIGHT:
        if (time_in_manuever < RETREAT_REVERSE_TIME_MS) {
            set_drive(DRIVE_REVERSE, userdata);
            return true;
        } else if (time_in_manuever < RETREAT_REVERSE_TIME_MS + RETREAT_ROTATE_TIME_MS) {
            set_drive(DRIVE_ROTATE_RIGHT, userdata);
            return true;
        }
        break;
    case RETREAT_RIGHT:
        set_drive(DRIVE_ARCTURN_RIGHT, userdata);
        /* "Block" sleep here to make sure we get away from the line */
        /* TODO: We shouldn't block sleep here */
        sleep_ms_ud(RETREAT_ARCTURN_TIME_MS, userdata);
        return false;
    case RETREAT_LEFT:
        set_drive(DRIVE_ARCTURN_LEFT, userdata);
        /* "Block" sleep here to make sure we get away from the line */
        /* TODO: We shouldn't block sleep here */
        sleep_ms_ud(RETREAT_ARCTURN_TIME_MS, userdata);
        return false;
    case RETREAT_NONE:
        break;
    }
    return false;
}


void test_retreat_manuevers(void *userdata)
{
    retreat_manuever_t current_manuever = RETREAT_FORWARD_RIGHT;
    uint32_t time_in_manuever = 0;
    uint32_t retreat_manuever_time_started = ms_elapsed(userdata);
    bool retreat_manuever_done = false;
    while (1)
    {
        time_in_manuever = ms_elapsed(userdata) - retreat_manuever_time_started;
        retreat_manuever_done = !do_retreat_manuever(current_manuever, time_in_manuever, userdata);

        if (retreat_manuever_done) {
            switch (current_manuever) {
                case RETREAT_FORWARD_LEFT:
                    current_manuever = RETREAT_FORWARD_RIGHT;
                    break;
                case RETREAT_FORWARD_RIGHT:
                    current_manuever = RETREAT_REVERSE_LEFT;
                    break;
                case RETREAT_REVERSE_LEFT:
                    current_manuever = RETREAT_REVERSE_RIGHT;
                    break;
                case RETREAT_REVERSE_RIGHT:
                    current_manuever = RETREAT_RIGHT;
                    break;
                case RETREAT_RIGHT:
                    current_manuever = RETREAT_LEFT;
                    break;
                case RETREAT_LEFT:
                case RETREAT_NONE:
                    current_manuever = RETREAT_FORWARD_LEFT;
                    break;
            }
            set_drive(DRIVE_STOP, userdata);
            sleep_ms_ud(1000, userdata);
            retreat_manuever_time_started = ms_elapsed(userdata);
        }
        sleep_ms_ud(1, userdata);
    }


}


#define STATE_SEARCH_1_TIME_MS (1500)
#define STATE_SEARCH_2_TIME_MS (5000)
void _main(void *userdata)
{
    state_t current_state = STATE_SEARCH_1;
    uint32_t time_at_state_change = ms_elapsed(userdata);
    uint32_t last_time_attack = time_at_state_change;
    retreat_manuever_t current_retreat_manuever = RETREAT_NONE;
    uint32_t retreat_manuever_time_started = time_at_state_change;

    while(true) {
        state_t next_state = current_state;
        uint32_t time_ms = ms_elapsed(userdata);
        /* To be able to gather the variables up here only requires us to not allow for
         * any longer sleep inside the state machine (TODO: make sure there is none)) */
        /* Say also that this is a bonus with havning no sleep and continuosly going around, we
         * don't need to gather variables over and over */
        /* TODO: Define all sleep constants as defines! */
        line_detection_t line_detection = get_line_detection(userdata);
        enemy_detection_t enemy_detection = get_enemy_detection(userdata);
        //printf("State %d\n", current_state);
        printf("Curretn state %d\n", current_state);
        switch (current_state)
        {
        case STATE_SEARCH_1:
            /* TODO: Handle line detection here, we might be pushed by the enemy */
            if (enemy_detection & ENEMY_DETECTION_FRONT) {
                next_state = STATE_ATTACK;
                set_drive(DRIVE_STOP, userdata);
                break;
            }
            if (line_detection != LINE_DETECTION_NONE || (time_ms - time_at_state_change > STATE_SEARCH_1_TIME_MS)) {
                next_state = STATE_SEARCH_2;
                break;
            }
            set_drive(DRIVE_ROTATE_RIGHT, userdata);

            break;
        case STATE_SEARCH_2: /* Drive around to find the enemy */
            if (enemy_detection & ENEMY_DETECTION_FRONT) {
                next_state = STATE_ATTACK;
                set_drive(DRIVE_STOP, userdata);
                break;
            }
            if (line_detection == LINE_DETECTION_NONE) {
                set_drive(DRIVE_FORWARD, userdata);
            } else {
                set_drive(DRIVE_STOP, userdata);
                next_state = STATE_RETREAT;
                break;
            }
            if (time_ms - time_at_state_change > STATE_SEARCH_2_TIME_MS) {
                next_state = STATE_SEARCH_1;
                break;
            }

            break;
        case STATE_ATTACK:
            last_time_attack = time_ms;
            (void)last_time_attack;
            if (line_detection != LINE_DETECTION_NONE) {
                next_state = STATE_RETREAT;
                set_drive(DRIVE_STOP, userdata);
                break;
            }
            if (enemy_detection & ENEMY_DETECTION_FRONT) {
                set_drive(DRIVE_FORWARD, userdata);
            } else {
                set_drive(DRIVE_STOP, userdata);
                next_state = STATE_SEARCH_1;
            }
            /* TODO: Timeout..., we may be stuck in a head on battle... */
            break;
        case STATE_RETREAT:
            if (current_retreat_manuever != RETREAT_NONE) {
                bool retreat_manuever_done = !do_retreat_manuever(current_retreat_manuever, time_ms - retreat_manuever_time_started, userdata);
                if (retreat_manuever_done) {
                    current_retreat_manuever = RETREAT_NONE;
                    set_drive(DRIVE_STOP, userdata);
                }
            }
            if (line_detection == LINE_DETECTION_NONE) {
                /* TODO: This is a bit dangerous, we may be going between retreat manuevers and never
                 * getting to RETREAT_NONE, e.g. if we drive back and forth detecting line before
                 * retreat maneuver "timeouts", perhaps detect such behaviour as line to left/right, and
                 * do arcturn manuever. Basically, if retreat manuever is 200ms drive back, then we should
                 * see any back<->forth detection that happens under 200ms as line being to left/right...*/
                if (current_retreat_manuever == RETREAT_NONE) {
                    next_state = STATE_SEARCH_1;
                    break;
                }
            } else {
                /* Line detected again, see if we should change retreat manuever */
                retreat_manuever_t new_manuever = get_retreat_manuever(line_detection);
                if (new_manuever != current_retreat_manuever) {
                    current_retreat_manuever = new_manuever;
                    retreat_manuever_time_started = time_ms;
                }
                break;
            }

            /* Should handle having an enemy and a line detected... */
            break;
        }
        if (is_enemy_out(line_detection, enemy_detection)) {
            printf("Enemy out!\n");
        }

        if (next_state != current_state) {
            time_at_state_change = ms_elapsed(userdata);
            current_state = next_state;
        }
        /* Sleep a bit to offload the host CPU :) */
        sleep_ms_ud(1, userdata);
    }
}
