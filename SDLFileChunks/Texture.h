#ifndef TEXTURE_H
#define TEXTURE_H

#include "Resource.h"
#include "TGAReader.h"

class Asset;

class Texture : public Resource
{
public:
    // Constructors/Destructors
    Texture();
    virtual ~Texture();

    // Accessors
    Asset* GetData() { return m_texture; }
    ImageInfo* GetImageInfo() { return &m_imageInfo; }
    SDL_BlendMode GetBlendMode() { return m_blendMode; }
    void SetBlendMode(SDL_BlendMode _blendMode) { m_blendMode = _blendMode; }
    Byte1 GetBlendAlpha() { return m_blendAlpha; }
    void SetBlendAlpha(Byte1 _blendAlpha) { m_blendAlpha = _blendAlpha; }

    // Methods
    void Serialize(std::ostream& _stream) override;
    void Deserialize(std::istream& _stream) override;
    void ToString() override;
    void Load(string _guid);

    // Members
    static ObjectPool<Texture>* Pool;

private:
    ImageInfo m_imageInfo;
    Asset* m_texture;
    SDL_BlendMode m_blendMode;
    Byte1 m_blendAlpha;
};

#endif // TEXTURE_H