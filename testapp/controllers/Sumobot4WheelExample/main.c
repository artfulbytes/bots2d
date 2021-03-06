#include "Sumobot4WheelExample/c_bindings.h"

/* Must match with how the lines are attached to the microcontroller */
enum VoltageLine {
    FRONT_LEFT_MOTOR = 0,
    BACK_LEFT_MOTOR = 1,
    FRONT_RIGHT_MOTOR = 2,
    BACK_RIGHT_MOTOR = 3,
    FRONT_LEFT_LINE_DETECTOR = 4,
    BACK_LEFT_LINE_DETECTOR = 5,
    FRONT_RIGHT_LINE_DETCTOR = 6,
    BACK_RIGHT_LINE_DETECTOR = 7,
};

float **voltageLines;
void setup(float **const voltageLinesArr, int voltageLineCount)
{
    (void)voltageLineCount;
    voltageLines = voltageLinesArr;
    *voltageLines[FRONT_LEFT_MOTOR] = 4.0f;
    *voltageLines[BACK_LEFT_MOTOR] = 4.0f;
    *voltageLines[FRONT_RIGHT_MOTOR] = 4.0f;
    *voltageLines[BACK_RIGHT_MOTOR] = 4.0f;
}

void loop()
{
    /* Drive back and forth */
    if (*voltageLines[BACK_LEFT_LINE_DETECTOR] > 1.0f) {
        *voltageLines[FRONT_LEFT_MOTOR] = 4.0f;
        *voltageLines[BACK_LEFT_MOTOR] = 4.0f;
        *voltageLines[FRONT_RIGHT_MOTOR] = 4.0f;
        *voltageLines[BACK_RIGHT_MOTOR] = 4.0f;
    } else if (*voltageLines[FRONT_LEFT_LINE_DETECTOR] > 1.0f) {
        *voltageLines[FRONT_LEFT_MOTOR] = -4.0f;
        *voltageLines[BACK_LEFT_MOTOR] = -4.0f;
        *voltageLines[FRONT_RIGHT_MOTOR] = -4.0f;
        *voltageLines[BACK_RIGHT_MOTOR] = -4.0f;
    }
}
