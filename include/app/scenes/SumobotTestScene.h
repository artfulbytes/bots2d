#ifndef SUMOBOT_TEST_SCENE_H_
#define SUMOBOT_TEST_SCENE_H_

#include "Scene.h"

#include "playgrounds/Dohyo.h"
#include "shapes/QuadObject.h"

class QuadObject;
class Dohyo;
class Sumobot;

class SumobotTestScene : public Scene
{
public:
    SumobotTestScene();
    ~SumobotTestScene();

private:
    struct Background {
        std::unique_ptr<QuadObject> leftSide;
        std::unique_ptr<QuadObject> rightSide;
    };
    void createBackground();
    std::unique_ptr<Background> m_background = std::make_unique<Background>();
    std::unique_ptr<Dohyo> m_dohyo;
    std::unique_ptr<Sumobot> m_sumobot;
};

#endif /* SUMOBOT_TEST_SCENE_H_ */
