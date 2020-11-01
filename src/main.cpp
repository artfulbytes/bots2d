#include "SimulatorApp.h"
#include <cstdlib>

int main()
{
    /* TODO: Smart pointer? */
    SimulatorApp *app = new SimulatorApp();
    app->run();
    delete app;
}
