#ifndef TOP_VIEW_SUMOBOT_4_WHEEL_H_
#define TOP_VIEW_SUMOBOT_4_WHEEL_H_

#include "AppObject.h"
#include "PhysicsWorld.h"

class Body2D;
class TopViewWheelMotor;

/* TopViewSumobot4Wheel simulates a sumo robot with four DC motors in a
 * top-view gravity physics world
 *
 */
class TopViewSumobot4Wheel : public AppObject
{
public:
    /*
     *    |-----width-----|
     *       ___________     ___
     *     __|         |__    |
     *    |  |         |  |   |
     *    |__|         |__|   l
     *     __|         |__    e
     *    |  |         |  |   n
     *    |__|         |__|   |
     *       |_________|     _|_
     */
    struct Specification {
        const float length;
        const float width;
        const float mass;
        const float frictionCoefficient = 0.05f;
    };

    TopViewSumobot4Wheel(AppScene &scene, const PhysicsWorld &world, const Specification &unscaledSpec, const Vec2 &unscaledStartPos);
    ~TopViewSumobot4Wheel();
    float *getFrontLeftMotorVoltageLine();
    float *getFrontRightMotorVoltageLine();
    float *getBackLeftMotorVoltageLine();
    float *getBackRightWheelVoltageLine();

    void onFixedUpdate(double stepTime) override;
private:
    /* TODO: Move these to spec, and add assert to ensure factors add up to 1.0f? */
    static constexpr float massBodyFactor = 0.9f;
    static constexpr float massWheelFactor = 1.0f - massBodyFactor;
    static constexpr float widthBodyFactor = 0.8f;
    static constexpr float widthWheelsFactor = 1.0f - widthBodyFactor;

    void createBody(AppScene &appScene, const PhysicsWorld &world, const Specification &unscaledSpec, const Vec2 &unscaledStartPos);
    void createWheelMotors(AppScene &appScene, const PhysicsWorld &world, const Specification &unscaledSpec, const Vec2 &unscaledBodyStartPos);
    static Specification scaleSpec(const Specification &unscaledSpec);
    Body2D *m_body2D = nullptr;
    Specification m_scaledSpec;

    TopViewWheelMotor *m_frontRightWheelMotor = nullptr;
    TopViewWheelMotor *m_frontLeftWheelMotor = nullptr;
    TopViewWheelMotor *m_backRightWheelMotor = nullptr;
    TopViewWheelMotor *m_backLeftWheelMotor = nullptr;
};

#endif /* TOP_VIEW_SUMOBOT_4_WHEEL_H_ */
