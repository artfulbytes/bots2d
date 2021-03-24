#ifndef MICROCONTROLLER_C_SETUP_H_
#define MICROCONTROLLER_C_SETUP_H_

/**
 * Separate from micrcontroller_c_functions.h because this function shouldn't be visible to
 * the user (microcontroller main.c) of the other functions.
 */

/**
 * Set userdata statically to avoid having to pass userdata to each C function in your
 * simulated C microcontroller. NOTE, you can simulate at most ONE microcontroller at a time
 * that avoids passing userdata, the rest must pass userdata explicitly.
 */
void set_userdata(void *userdata);

#endif /* MICROCONTROLLER_C_SETUP_H_ */
