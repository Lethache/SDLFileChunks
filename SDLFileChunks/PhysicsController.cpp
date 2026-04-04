#include "PhysicsController.h"
#include "Renderer.h"

PhysicsController::PhysicsController()
{
    m_gravity = -9.81f; // Earth's gravitational pull constant
    m_force = glm::vec2{ 0, 0 };
    Particle::Pool = new ObjectPool<Particle>();
}

PhysicsController::~PhysicsController()
{
    delete Particle::Pool;
    Particle::Pool = nullptr;
}

void PhysicsController::Update(float _deltaTime)
{
    for (int count = 0; count < m_particles.size(); count++)
    {
        Particle* p = m_particles[count];
        m_force.y = p->GetMass() * m_gravity;
        p->Update(_deltaTime, m_force);

        if (!p->GetDead()) continue;

        Particle::Pool->ReleaseResource(p);
        m_particles.erase(m_particles.begin() + count);
        count--;
    }
}

Particle* PhysicsController::AddParticle(glm::vec2 _position, float _lifeTime)
{
    Particle* particle = Particle::Pool->GetResource();
    particle->SetPosition(_position);
    particle->SetLifeTime(_lifeTime);
    particle->SetMass(1.0f);
    m_particles.push_back(particle);
    return particle;
}

string PhysicsController::ToString()
{
    return "Particles count: " + std::to_string(m_particles.size());
}