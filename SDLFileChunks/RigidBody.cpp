#include "RigidBody.h"

RigidBody::RigidBody()
{
    m_position = {};
    m_velocity = {};
    m_mass = 0;
    m_dead = false;
}

void RigidBody::Update(float _deltaTime, glm::vec2 _force)
{
    
    _force.y *= -1;

    glm::vec2 acceleration = glm::vec2{
        _force.x / m_mass,
        _force.y / m_mass
    };

    m_velocity.x += acceleration.x * _deltaTime;
    m_velocity.y += acceleration.y * _deltaTime;

    m_position.x += m_velocity.x * _deltaTime;
    m_position.y += m_velocity.y * _deltaTime;
}