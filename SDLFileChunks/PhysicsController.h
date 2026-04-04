#ifndef PHYSICS_CONTROLLER_H
#define PHYSICS_CONTROLLER_H

#include "StandartIncludes.h"

class Renderer;
class Particle;
class RigidBody;

class PhysicsController : public Singleton<PhysicsController>
{
public:
    // Constructors / Destructors
    PhysicsController();
    virtual ~PhysicsController();

    // Accessors
    float GetGravity() { return m_gravity; }
    void SetGravity(float _gravity) { m_gravity = _gravity; }
    vector<Particle*>& GetParticles() { return m_particles; }
    vector<RigidBody*>& GetBodies() { return m_bodies; }

    // Methods
    void Update(float _deltaTime);
    Particle* AddParticle(glm::vec2 _position, float _lifeTime);
    RigidBody* AddRigidBody(glm::vec2 _position, glm::vec2 _direction, float _mass);
    void Reflect(glm::vec2& _out, const glm::vec2& _incident, const glm::vec2& _normal);
    void HandleCollision(RigidBody* _b1, RigidBody* _b2, float _distance);
    string ToString();

private:
    // Members
    float m_gravity;
    glm::vec2 m_force;
    vector<Particle*> m_particles;
    vector<RigidBody*> m_bodies;
};

#endif // PHYSICS_CONTROLLER_H