#ifndef PARTICLE_H
#define PARTICLE_H

#include "StandartIncludes.h"
#include "RigidBody.h"
#include "ObjectPool.h"

class Particle : public RigidBody {
public:
    Particle();
    virtual ~Particle();

    static ObjectPool<Particle>* Pool;

    void Reset();
    
    void Update(float _dt, glm::vec2 _force);

    void SetDead(bool _dead);
    bool GetDead();

    void SetLifeTime(float _lt);
    float GetCurrentSize();

    void SetBuoyancy(glm::vec2 _v);
    void SetBuoyancyDecay(glm::vec2 _v);
    void SetRandomForce(glm::vec2 _v);
    void SetWind(glm::vec2 _v);

private:
    bool m_dead;
    float m_currentTime;
    float m_lifeTime;
    glm::vec2 m_buoyancy;
    glm::vec2 m_buoyancyDecay;
    glm::vec2 m_randomForce;
    glm::vec2 m_wind;
};

#endif