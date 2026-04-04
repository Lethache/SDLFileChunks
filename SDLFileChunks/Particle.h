#ifndef PARTICLE_H
#define PARTICLE_H

#include "RigidBody.h"
#include "ObjectPool.h"

class Particle : public RigidBody
{
public:
    Particle();
    virtual ~Particle() {}

    float GetLifeTime() { return m_lifeTime; }
    void SetLifeTime(float _lifeTime) { m_lifeTime = _lifeTime; }

    void Update(float _deltaTime, glm::vec2 _force) override;

    static ObjectPool<Particle>* Pool;

private:
    float m_currentTime;
    float m_lifeTime;
};

#endif