This is the controller code for simulating Nsumo, a mini-class sumobot with
four-wheel drive, four line-sensors and five range sensors. The real controller
code is written in C and runs on an MSP430 microcontroller, so we must use the
C-bindings for simulating it. The code for the real microcontroller is included
as a submodule (directory nsumo), and the simulated code uses the same high-level
functions (e.g. state_machine, line_detection), which makes it very easy to
switch between the simulator and the real sumobot.

