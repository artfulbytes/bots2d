#ifndef MICROCONTROLLER_C_FUNCTIONS_H_
#define MICROCONTROLLER_C_FUNCTIONS_H_

#include <stdint.h>

/**
 * Helper functions that lets a microcontroller written in C communicate
 * with the rest of the simulator. If your microcontroller class derives
 * from MicrocontrollerCBinding, you MUST use these functions.
 */

/**
 * Sleeps for X number of physics steps until roughly sleep_ms milliseconds
 * have passed (more accurate if physics step time is smaller).
 *
 * NOTE: If you need to sleep in your microcontroller code you should use
 * THIS function, and not the OS sleep function! */
void sleep_ms(uint32_t sleep_ms);

/**
 * Get voltage level for the voltage line connected to index idx.
 */
float get_voltage(int idx);
/**
 * Set voltage level for the voltage line connected to index idx.
 */
void set_voltage(int idx, float level);

#endif /* MICROCONTROLLER_C_FUNCTIONS_H_ */
