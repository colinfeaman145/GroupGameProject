#ifndef PLAYERHUD_HPP
#define PLAYERHUD_HPP

#include <SDL.h>
#include <vector>
#include "Element.hpp"
#include "Text.hpp"

struct ItemPopup {
    Text* nameText;
    Text* descText;
    float timer;
};

class Text;
class Sprite;
class InventoryOverlay;
class Player;
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

    void HandleHUDElementsProcess(float deltaTime);
    void HandleHUDElementsDraw(Renderer* renderer);
    void HandleGunSpriteRotation();

    void SetBulletTexture();
    void SetBulletSprite();
    void SetWeaponTexture();
    void SetWeapoinSprite();
    void SetCashoutHUD();
    void SetInventoryHUD();
    void HandleTimerUpdate();
    void SetDifficultyHUD();
    void SetStatSheetHUD();

    void PushPopup(const std::string& name, const std::string& desc);

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

    Text* timerText;
    std::vector<Sprite*> playerHudElements;

    std::vector<ItemPopup> popups;
};
#endif // PLAYERHUD_HPP