#include "c_bindings.h"

/* Must match with how the lines are attached to the microcontroller */
enum VoltageLine {
    FRONT_LEFT_MOTOR = 0,
    BACK_LEFT_MOTOR = 1,
    FRONT_RIGHT_MOTOR = 2,
    BACK_RIGHT_MOTOR = 3,
    LEFT_RANGE_SENSOR = 4,
    FRONT_LEFT_RANGE_SENSOR = 5,
    FRONT_RANGE_SENSOR = 6,
    FRONT_RIGHT_RANGE_SENSOR = 7,
    RIGHT_RANGE_SENSOR = 8,
    FRONT_LEFT_LINE_DETECTOR = 9,
    BACK_LEFT_LINE_DETECTOR = 10,
    FRONT_RIGHT_LINE_DETECTOR = 11,
    BACK_RIGHT_LINE_DETECTOR = 12
};

float **voltageLines;
void setup(float **const voltageLinesArr, int voltageLineCount)
{
    voltageLines = voltageLinesArr;
    *voltageLines[FRONT_LEFT_MOTOR] = 3.0f;
    *voltageLines[BACK_LEFT_MOTOR] = 3.0f;
    *voltageLines[FRONT_RIGHT_MOTOR] = 3.0f;
    *voltageLines[BACK_RIGHT_MOTOR] = 3.0f;
}

void loop()
{
    /* Drive back and forth */
    if (*voltageLines[FRONT_LEFT_LINE_DETECTOR] > 1.0f) {
        *voltageLines[FRONT_LEFT_MOTOR] = -3.0f;
        *voltageLines[BACK_LEFT_MOTOR] = -3.0f;
        *voltageLines[FRONT_RIGHT_MOTOR] = -3.0f;
        *voltageLines[BACK_RIGHT_MOTOR] = -3.0f;
    } else if (*voltageLines[BACK_LEFT_LINE_DETECTOR] > 1.0f) {
        *voltageLines[FRONT_LEFT_MOTOR] = 3.0f;
        *voltageLines[BACK_LEFT_MOTOR] = 3.0f;
        *voltageLines[FRONT_RIGHT_MOTOR] = 3.0f;
        *voltageLines[BACK_RIGHT_MOTOR] = 3.0f;
    }
}
