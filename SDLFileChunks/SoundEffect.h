#ifndef SOUND_EFFECT_H
#define SOUND_EFFECT_H

#include "Resource.h"
#include "ObjectPool.h" 

class Asset;

class SoundEffect : public Resource
{
public:
    SoundEffect();
    virtual ~SoundEffect();

    Asset* GetData() { return m_effect; }

    void Serialize(std::ostream& _stream) override;
    void Deserialize(std::istream& _stream) override;
    void ToString() override;
    void Load(string _guid); 

    static ObjectPool<SoundEffect>* Pool;

private:
    Asset* m_effect;
};

#endif