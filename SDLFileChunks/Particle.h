#ifndef PARTICLE_H
#define PARTICLE_H

#include "RigidBody.h"
#include "ObjectPool.h"

class Particle : public RigidBody
{
public:
    // Constructors / Destructors
    Particle();
    virtual ~Particle() {}

    // Accessors
    float GetLifeTime() { return m_lifeTime; }
    void SetLifeTime(float _lifeTime) { m_lifeTime = _lifeTime; }

    // Methods
    void Update(float _deltaTime, glm::vec2 _force) override;

    
    void Reset() override;

    //Members
    static ObjectPool<Particle>* Pool;

private:
    // Members
    float m_currentTime;
    float m_lifeTime;
};

#endif // PARTICLE_H