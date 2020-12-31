#ifndef LINE_FOLLOWER_TEST_SCENE_H_
#define LINE_FOLLOWER_TEST_SCENE_H_

#include "Scene.h"

#include "shapes/RectObject.h"

class RectObject;
class LineFollower;
class LineFollowerPath;

class LineFollowerTestScene : public Scene
{
public:
    LineFollowerTestScene();
    ~LineFollowerTestScene();

private:
    std::unique_ptr<RectObject> m_background;
    std::unique_ptr<LineFollowerPath> m_lineFollowerPath;
    std::unique_ptr<LineFollower> m_lineFollower;
};

#endif /* LINE_FOLLOWER_TEST_SCENE_H_ */
