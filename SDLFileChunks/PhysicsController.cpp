#include "PhysicsController.h"
#include "Renderer.h"
#include "Particle.h"
#include "RigidBody.h"

PhysicsController::PhysicsController()
{
    m_gravity = -9.81f; // Earth's gravitational pull constant
    m_force = glm::vec2{ 0, 0 };
    Particle::Pool = new ObjectPool<Particle>();
    RigidBody::Pool = new ObjectPool<RigidBody>();
}

PhysicsController::~PhysicsController()
{
    delete Particle::Pool;
    delete RigidBody::Pool;
}

void PhysicsController::Reflect(glm::vec2& _out, const glm::vec2& _incident, const glm::vec2& _normal)
{
    _out = _incident - 2.f * glm::dot(_incident, _normal) * _normal;
}

// https://en.wikipedia.org/wiki/Elastic_collision
void PhysicsController::HandleCollision(RigidBody* _b1, RigidBody* _b2, float _distance)
{
    glm::vec2 x = _b1->GetPosition() - _b2->GetPosition(); // Collision Direction
    glm::vec2 v = _b1->GetVelocity() - _b2->GetVelocity(); // Movement Direction
    glm::vec2 v1 = _b1->GetVelocity();
    v1 -= (2 * _b2->GetMass() / (_b1->GetMass() + _b2->GetMass())) * (glm::dot(v, x) / ((float)pow(glm::length(x), 2))) * x;
    _b1->SetVelocity(v1);

    x *= -1;
    v *= -1;
    glm::vec2 v2 = _b2->GetVelocity();
    v2 -= (2 * _b1->GetMass() / (_b1->GetMass() + _b2->GetMass())) * (glm::dot(v, x) / ((float)pow(glm::length(x), 2))) * x;
    _b2->SetVelocity(v2);
}

void PhysicsController::Update(float _deltaTime)
{
    for (int count = 0; count < m_particles.size(); count++)
    {
        Particle* p = m_particles[count];
        m_force.y = p->GetMass() * m_gravity;
        p->Update(_deltaTime, m_force);
        if (!p->GetDead()) continue;
        p->Reset();
        Particle::Pool->ReleaseResource(p);
        m_particles.erase(m_particles.begin() + count);
        count--;
    }

    Point p = Renderer::Instance().GetWindowSize();
    glm::vec2 out;
    for (int count = 0; count < m_bodies.size(); count++)
    {
        RigidBody* b = m_bodies[count];
        b->Update(_deltaTime, glm::vec2{ 0, 0 });
        if (b->GetPosition().x >= p.x - 16)
        {
            Reflect(out, b->GetVelocity(), glm::vec2{ -1, 0 });
            b->SetVelocity(out);
        }
        else if (b->GetPosition().x <= 16)
        {
            Reflect(out, b->GetVelocity(), glm::vec2{ 1, 0 });
            b->SetVelocity(out);
        }
        else if (b->GetPosition().y >= p.y - 16)
        {
            Reflect(out, b->GetVelocity(), glm::vec2{ 0, -1 });
            b->SetVelocity(out);
        }
        else if (b->GetPosition().y <= 16)
        {
            Reflect(out, b->GetVelocity(), glm::vec2{ 0, 1 });
            b->SetVelocity(out);
        }
    }


    if (m_bodies.empty()) return;
    for (int c1 = 0; c1 < m_bodies.size() - 1; c1++)
    {
        for (int c2 = c1 + 1; c2 < m_bodies.size(); c2++)
        {
            RigidBody* b1 = m_bodies[c1];
            RigidBody* b2 = m_bodies[c2];
            float dist = glm::distance(b1->GetPosition(), b2->GetPosition());
            if (dist <= 32)
            {
                HandleCollision(b1, b2, dist);
            }
        }
    }
}

RigidBody* PhysicsController::AddRigidBody(glm::vec2 _position, glm::vec2 _direction, float _mass)
{
    RigidBody* body = RigidBody::Pool->GetResource();
    body->SetPosition(_position);
    body->SetVelocity(_direction);
    body->SetMass(_mass);
    m_bodies.push_back(body);
    return body;
}

string PhysicsController::ToString()
{
    string retVal = "Particles: ";
    retVal += to_string(m_particles.size());
    retVal += "   RigidBodies: ";
    retVal += to_string(m_bodies.size());
    return retVal;
}