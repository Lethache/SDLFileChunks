#include "RigidBody.h"

ObjectPool<RigidBody>* RigidBody::Pool;

RigidBody::RigidBody()
{
    Reset();
}

void RigidBody::Reset()
{
    m_position = { };
    m_velocity = { };
    m_acceleration = { };
    m_buoyancy = { };
    m_buoyancyDecay = { };
    m_buoyancyMin = { 0, 0 };
    m_randomForce = { 0, 0 };
    m_wind = { };
    m_mass = 0;
    m_dead = false;
}
void RigidBody::Update(float _deltaTime, glm::vec2 _force)
{
   
    if (m_dead || m_mass <= 0.0f) return;

   
    m_acceleration = _force / m_mass;

    
    m_velocity += m_acceleration * _deltaTime;


    m_position += m_velocity * _deltaTime;
}