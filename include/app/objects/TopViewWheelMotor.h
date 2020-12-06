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
        const float voltageInConstant;
        /* Torque constant / Motor resistance */
        const float angularSpeedConstant;
        const float maxVoltage;
        const float diameter;
        const float width;
        const float mass;
        const float maxDriveForce;
        /* This determines the sideway friction */
        const float maxLateralCancelingImpulse = 40.5f;
    };

    TopViewWheelMotor(AppScene &appScene, const PhysicsWorld &world, const Specification &unscaledSpec, const Vec2<float> &unscaledStartPos);
    ~TopViewWheelMotor();

    void setVoltageIn(float voltage);
    void setDutyCycle(float dutyCycle);
    void onFixedUpdate(double stepTime) override;
    float *getVoltageLine();
    const Body2D *getBody() const { return m_body2D; }
private:
    static Specification scaleSpec(const Specification &unscaledSpec);

    void updateForce();
    Body2D *m_body2D = nullptr;
    Specification m_scaledSpec;
    float m_voltageIn = 0.0f;
};

#endif /* TOP_VIEW_WHEEL_MOTOR_H_ */
