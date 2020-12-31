#ifndef DRAW_TEST_SCENE_H_
#define DRAW_TEST_SCENE_H_

#include "Scene.h"

class RectObject;
class QuadObject;
class CircleObject;
class LineObject;

class DrawTestScene : public Scene
{
public:
    DrawTestScene();

private:
    std::unique_ptr<RectObject> m_rectObject;
    std::unique_ptr<QuadObject> m_quadObject;
    std::unique_ptr<CircleObject> m_circleObject;
    std::unique_ptr<LineObject> m_lineObject;
};

#endif /* DRAW_TEST_SCENE_H_ */
