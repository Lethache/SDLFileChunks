#include "Particle.h"


ObjectPool<Particle>* Particle::Pool = nullptr;

Particle::Particle() {
    Reset();
}

Particle::~Particle() {
}

void Particle::Reset() {
    m_dead = false;
    m_currentTime = 0.0f;
    m_lifeTime = 1.0f;
    m_buoyancy = glm::vec2(0.0f);
    m_buoyancyDecay = glm::vec2(0.0f);
    m_randomForce = glm::vec2(0.0f);
    m_wind = glm::vec2(0.0f);
}

void Particle::Update(float _dt, glm::vec2 _force) {
    m_currentTime += _dt;
    if (m_currentTime > m_lifeTime) {
        m_dead = true;
    }


}


void Particle::SetDead(bool _dead) { m_dead = _dead; }
bool Particle::GetDead() { return m_dead; }
void Particle::SetLifeTime(float _lt) { m_lifeTime = _lt; }
float Particle::GetCurrentSize() { return (m_lifeTime > 0) ? (m_currentTime / m_lifeTime) : 1.0f; }
void Particle::SetBuoyancy(glm::vec2 _v) { m_buoyancy = _v; }
void Particle::SetBuoyancyDecay(glm::vec2 _v) { m_buoyancyDecay = _v; }
void Particle::SetRandomForce(glm::vec2 _v) { m_randomForce = _v; }
void Particle::SetWind(glm::vec2 _v) { m_wind = _v; }