#ifndef SUMOBOT_4_WHEEL_TEST_SCENE_H_
#define SUMOBOT_4_WHEEL_TEST_SCENE_H_

#include "AppScene.h"

class TopViewSumobot4Wheel;

class Sumobot4WheelTestScene : public AppScene
{
public:
    Sumobot4WheelTestScene();
    ~Sumobot4WheelTestScene();
private:
    void createBackground(float dohyoOuterRadius);
    TopViewSumobot4Wheel *m_sumobot4WheelPlated = nullptr;
    TopViewSumobot4Wheel *m_sumobot4WheelCircuited = nullptr;
};

#endif /* SUMOBOT_4_WHEEL_TEST_SCENE_H_ */
