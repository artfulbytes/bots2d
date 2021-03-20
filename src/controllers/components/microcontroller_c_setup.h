#ifndef MICROCONTROLLER_C_SETUP_H_
#define MICROCONTROLLER_C_SETUP_H_

/**
 * Separate from micrcontroller_c_functions.h because this function shouldn't be visible to
 * the user (microcontroller main.c) of the other functions.
 */
typedef float (*get_voltage_function)(int, void *);
typedef void (*set_voltage_function)(int, float, void *);
typedef void (*sleep_function)(int, void *);
/**
 * "Connects" the C++ functions to the C functions, so that we can indirectly call the
 * C++ functions of the Microcontroller class from the C-code.
 */
void setup(get_voltage_function get_voltage_fcn, set_voltage_function set_voltage_fcn, sleep_function sleep_fcn, void *userdata);

#endif /* MICROCONTROLLER_C_SETUP_H_ */
