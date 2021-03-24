#ifndef WHEEL_MOTOR_H_
#define WHEEL_MOTOR_H_

#include "SceneObject.h"
#include <glm/glm.hpp>
#include <string>

class PhysicsWorld;
class SpriteAnimation;
class Body2D;

/**
 * A wheel with a built-in DC-motor. The DC-motor is modeled with basic DC-motor
 * equations computing torque/force from a voltage input. Tune voltageInConstant and angularSpeedConstant
 * for different acceleration and top speed.
 *
 * NOTE: Currently, top-view only!
 * Physics note: The model is simplified. As with a real DC motor, the force is larger when
 * a voltage is applied and the rotational speed is low. When the speed increases, the back
 * EMF increases, which makes the forward force lower. If we stop applying a voltage while the
 * motor and wheel is moving, the back EMF will slow the vehicle down. We mimic the motor loss
 * by applying coloumb friction in all direction. And to better mimic a real tire, we add an
 * additional sideway force negatively proportional to the sideway velocity (sidewayFrictionConstant).
 * This also determines the amount of sideway slipping. At the moment, there is no slipping
 * in the driving direction, that is, the grip is perfect no matter how large the
 * torque is.
 *
 */
class WheelMotor : public SceneObject
{
public:
    enum class Orientation { Left, Right };
    enum class TextureType { None, Green, Orange, Red };
    struct Specification {
        Specification(float width, float diameter, float wheelMass, TextureType textureType) :
            width(width), diameter(diameter), wheelMass(wheelMass), textureType(textureType) {}
        Specification(float voltageInConstant, float angularSpeedConstant, float maxVoltage,
                      float frictionCoefficient, float sidewayFrictionConstant, float width,
                      float diameter, float wheelMass, float loadedMass, TextureType textureType) :
            voltageInConstant(voltageInConstant), angularSpeedConstant(angularSpeedConstant),
            maxVoltage(maxVoltage), frictionCoefficient(frictionCoefficient), sidewayFrictionConstant(sidewayFrictionConstant),
            width(width), diameter(diameter), wheelMass(wheelMass), loadedMass(loadedMass), textureType(textureType) {}
        /** voltageInConstant = (Torque constant * Voltage constant) / Motor resistance */
        float voltageInConstant = 0.00628f;
        /** angularSpeedConstant = Torque constant / Motor resistance */
        float angularSpeedConstant = 0.00178f;
        /** Max voltage that can be applied to the voltage line */
        float maxVoltage = 6.0f;
        /** Friction (Coloumb) in ALL directions.
         * Not entirely realistic, because there is no friction force acting in the driving
         * direction. But it kind of introduces a "motor loss", which makes the vehicle
         * stop more realisticly, without it the robot lingers at low speed before making a
         * full stop.
         * */
        float frictionCoefficient = 0.1f;
        /** Made-up constant to tweak the sideway friction, larger value
         * means more friction and less skidding */
        float sidewayFrictionConstant = 100.0f;
        float width = 0.0f;
        float diameter = 0.0f;
        float wheelMass = 0.0f;
        /** The mass the attached body adds on the wheel  */
        float loadedMass = 0.0f;
        TextureType textureType = TextureType::None;
    };

    WheelMotor(Scene *scene, const Specification &spec, Orientation orientation,
               const glm::vec2 &startPosition, float startRotation);
    ~WheelMotor();

    void setVoltageIn(float voltage);
    void setDutyCycle(float dutyCycle);
    void setFrictionCoefficient(float frictionCoefficient);
    void setSidewayFrictionConstant(float sidewayFrictionConstant);
    float getFrictionCoefficient() const;
    float getSidewayFrictionConstant() const;
    void onFixedUpdate() override;
    float *getVoltageLine();
    const Body2D *getBody() const { return m_body2D; }
    void setLoadedMass(float loadedMass);
    void setMass(float mass);
    float getMass() const;
    void setMaxVoltage(float maxVoltage);
    void setAngularSpeedConstant(float angularSpeedConstant);
    void setVoltageInConstant(float voltageInConstant);
    float getVoltageInConstant() const;
    float getMaxVoltage() const;
    float getAngularSpeedConstant() const;

private:
    void setAnimation();
    std::string getTextureName(Orientation orientation, TextureType textureType) const;
    void updateForce();

    Specification m_spec;
    float m_voltageIn = 0.0f;
    std::unique_ptr<SpriteAnimation> m_animation;
    Body2D *m_body2D = nullptr;
};

#endif /* WHEEL_MOTOR_H_ */
