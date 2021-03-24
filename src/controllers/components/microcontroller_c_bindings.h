#ifndef MICROCONTROLLER_C_BINDINGS_H_
#define MICROCONTROLLER_C_BINDINGS_H_

#include <stdint.h>

/**
 * Helper functions that lets a microcontroller written in C communicate
 * with the rest of the simulator. A C microcontroller MUST use these
 * functions. Include this file in the C-file of your C microcontroller.
 *
 * If you only simulate a single C microcontroller, then you can use the
 * functions without passing userdata. But if you simulate multiple ones,
 * only one of them can avoid passing userdata, the rest must use the
 * userdata (<>_ud) functions, otherwise there is no way to know which
 * controller that made the call.
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


/**
 * Same behaviour as sleep_ms, but can be used if multiple microcontroller C
 * bindings should run at the same time.
 */
void sleep_ms_ud(uint32_t sleep_ms, void *userdata);
/**
 * Same behaviour as get_voltage, but can be used if multiple microcontroller C
 * bindings should run at the same time.
 */
float get_voltage_ud(int idx, void *userdata);
/**
 * Same behaviour as set_voltage, but can be used if multiple microcontroller C
 * bindings should run at the same time.
 */
void set_voltage_ud(int idx, float level, void *userdata);


#endif /* MICROCONTROLLER_C_BINDINGS_H_ */
