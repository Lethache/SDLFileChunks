#include "RigidBody.h"

RigidBody::RigidBody()
{
    Reset();
}

void RigidBody::Reset()
{
    m_position = { 0.0f, 0.0f };
    m_velocity = { 0.0f, 0.0f };
    m_mass = 0.0f;
    m_dead = false;
}

void RigidBody::Update(float _deltaTime, glm::vec2 _force)
{
    _force.y *= -1; // Invert force y direction, since the rendering y-axis is inverted
    glm::vec2 acceleration = glm::vec2{ _force.x / m_mass, _force.y / m_mass };
    m_velocity.x += acceleration.x * _deltaTime;
    m_velocity.y += acceleration.y * _deltaTime;
    m_position.x += m_velocity.x * _deltaTime;
    m_position.y += m_velocity.y * _deltaTime;
}