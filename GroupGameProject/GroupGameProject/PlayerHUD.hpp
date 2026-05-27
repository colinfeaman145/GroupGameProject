#ifndef PLAYERHUD_HPP
#define PLAYERHUD_HPP

#include "Element.hpp"
#include "Player.hpp"
#include "Attackable.hpp"

class PlayerHUD : public Element 
{ 

    enum WeaponType {
        Ar_1,
        Ar_2,
        Ar_3,
        Ar_4,
        Ar_5,
        Ar_6,
        Ar_7,
        Ar_8,
        Ar_9,
        Ar_10
    };

    enum BulletType {
        MachineGunBullet,
        ShotgunBullet,
        PistolBullet
    };
public:
    PlayerHUD(Player* owner);
    ~PlayerHUD();

    bool Initialize();
    void Process(float deltaTime);
    void Draw(Renderer* renderer);

    void SetBulletTexture();
    void SetWeaponTexture();

private:
    Player* player;
    SDL_Texture* BulletTexture;
    SDL_Texture* WeaponTexture;
    bool BulletSpriteInitialized;
    bool WeaponSpriteInitialized;

    BulletType bulletType;
    WeaponType weaponType;

    Sprite* bulletSprite;
    Sprite* weaponSprite;
};
#endif // PLAYERHUD_HPP
