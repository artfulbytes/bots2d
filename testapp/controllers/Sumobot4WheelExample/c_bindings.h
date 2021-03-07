#ifndef C_BINDINGS_H_
#define C_BINDINGS_H_

typedef float (*get_voltage_function)(int idx, void *);
typedef void (*set_voltage_function)(int idx, float, void *);

void setup(get_voltage_function get_voltage_fcn, set_voltage_function set_voltage_fcn, void *userdata);
void loop(void);

#endif /* C_BINDINGS_H_ */
