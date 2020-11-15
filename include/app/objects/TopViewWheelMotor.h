#ifndef TOP_VIEW_WHEEL_MOTOR_H_
#define TOP_VIEW_WHEEL_MOTOR_H_

#include "AppObject.h"
#include "Body2D.h"
#include "PhysicsWorld.h"

/* TopViewWheelMotor simulates a wheel connected to a DC-motor in a top-view
 * gravity physics world. */
class TopViewWheelMotor : public AppObject
{
public:
    struct Specification {
        /* Simplified DC motor model
         * Tune these two constants to get different characteristics
         * (Torque constant * Voltage constant) / Motor resistance */
        float voltageInConstant;
        /* Torque constant / Motor resistance */
        float angularSpeedConstant;
        float maxVoltage;
        float diameter;
        float width;
        float mass;
        float maxDriveForce;
        float maxLateralImpulse;
    };

    TopViewWheelMotor(AppScene &scene, const PhysicsWorld &world, const Specification &spec, Vec2 startPos);
    ~TopViewWheelMotor();

    void setVoltageIn(float voltage);
    void setDutyCycle(float dutyCycle);
    void onFixedUpdate(double stepTime) override;
    float *getVoltageLine();
private:
    void updateForce();
    Body2D *m_body2D = nullptr;
    SceneObject *m_sceneObject = nullptr;
    Specification m_spec;
    float m_voltageIn = 0.0f;
};

#endif /* TOP_VIEW_WHEEL_MOTOR_H_ */
