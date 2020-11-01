#ifndef SIMULATOR_APP_H_
#define SIMULATOR_APP_H_

#include "Application.h"

class Test;
class TestMenu;

class SimulatorApp : public Application
{
public:
    SimulatorApp();
    ~SimulatorApp();
    void onUpdate() override;

private:
    Test *m_currentTest = nullptr;
    TestMenu *m_testMenu = nullptr;
};

#endif /* SIMULATOR_APP_H_ */
