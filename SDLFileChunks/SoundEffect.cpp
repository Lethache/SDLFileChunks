#include "SoundEffect.h"
#include "AssetController.h" // Для Instance()
#include "Asset.h"           // Для ToString() и прочего

ObjectPool<SoundEffect>* SoundEffect::Pool = nullptr;

SoundEffect::SoundEffect() : m_effect(nullptr) {}
SoundEffect::~SoundEffect() {}

void SoundEffect::Load(string _guid)
{
    m_effect = AssetController::Instance().GetAsset(_guid);
}

void SoundEffect::Serialize(std::ostream& _stream)
{
    
}

void SoundEffect::Deserialize(std::istream& _stream)
{
    // DeserializeAsset(_stream, m_effect);
}

void SoundEffect::ToString()
{
    cout << "SOUND EFFECT" << endl;
    if (m_effect) m_effect->ToString();
    Resource::ToString();
}