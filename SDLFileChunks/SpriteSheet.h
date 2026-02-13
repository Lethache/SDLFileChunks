// SpriteSheet.h
#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include "Texture.h"
#include "SpriteAnim.h"
typedef unsigned char Byte1;
enum AnimationNames
{
    EN_AN_IDLE = 0,
    EN_AN_RUN,
    EN_AN_TWO_COMBO_ATTACK,
    EN_AN_DEATH,
    EN_AN_HURT,
    EN_AN_JUMP_UP_FALL,
    EN_AN_EDGE_GRAB,
    EN_AN_EDGE_IDLE,
    EN_AN_WALL_SIDE,
    EN_AN_CROUCH,
    EN_AN_DASH,
    EN_AN_DASH_ATTACH,
    EN_AN_SLIDE,
    EN_AN_LADDER_GRAB
};

class SpriteSheet : public Texture
{
public:
    // Constructors/Destructors
    SpriteSheet();
    ~SpriteSheet();

    // Methods
    void Serialize(std::ostream& _stream) override;
    void Deserialize(std::istream& _stream) override;
    void ToString() override;

    void SetSize(Byte1 _rows, Byte1 _columns, Byte1 _clipSizeX, Byte1 _clipSizeY);
    void AddAnimation(AnimationNames _name, short _clipStart, short _clipCount, float _clipSpeed);
    SDL_FRect Update(AnimationNames _name);

    // Members
    static ObjectPool<SpriteSheet>* Pool;

private:
    // Members
    Byte1 m_rows;
    Byte1 m_columns;
    Byte1 m_clipSizeX;
    Byte1 m_clipSizeY;

    map<AnimationNames, SpriteAnim*> m_animations;
};

#endif // SPRITESHEET_H
