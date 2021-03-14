#ifndef BOTS2D_TEST_APP_H_
#define BOTS2D_TEST_APP_H_

#include "Application.h"

/**
 * Test application with test scenes.
 */
class Bots2DTestApp : public Application
{
public:
    Bots2DTestApp();
    ~Bots2DTestApp();
    void onKeyEvent(const Event::Key &keyEvent) override;
    void onFixedUpdate() override;
};

#endif /* BOTS2D_TEST_APP_H_ */
