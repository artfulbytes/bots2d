#include "SimulatorApp.h"
#include <cstdlib>

int main()
{
    SimulatorApp *app = new SimulatorApp();
    app->run();
    delete app;
}
