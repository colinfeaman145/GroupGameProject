#include "PlayerHUD.hpp"

#include <string>
#include "Text.hpp"
#include "Sprite.hpp"
#include "Camera.hpp"
#include "Player.hpp"
#include "GameContext.hpp"
#include "InventoryOverlay.hpp"
#include "StatSheetOverlay.hpp"
#include "CashoutOverlay.hpp"
#include "DifficultyOverlay.hpp"



PlayerHUD::PlayerHUD(Player* player)
    : player(player)
    , BulletTexture(0)
    , WeaponTexture(0)
    , BulletSpriteInitialized(false)
    , WeaponSpriteInitialized(false)
    , bulletType()
    , weaponType()
    , bulletSprite(0)
    , weaponSprite(0)
{
}

PlayerHUD::~PlayerHUD()
{
    for (auto& element : playerHudElements) {
        delete element;
        element = nullptr;
    }

    //delete BulletTexture;
    //delete WeaponTexture;
}

bool PlayerHUD::Initialize()
{
    bulletType = MachineGunBullet;
    weaponType = Ar_1;

    SetBulletTexture();
    SetBulletSprite();

    SetWeaponTexture();
    SetWeapoinSprite();


    SetInventoryHUD();
    SetStatSheetHUD();
    SetDifficultyHUD();

    SetCashoutHUD();

    return true;
}

void PlayerHUD::Process(float deltaTime)
{
    HandleGunSpriteRotation();
    HandleHUDElementsProcess(deltaTime);
    HandleTimerUpdate();
    for (auto& p : popups) p.timer -= deltaTime;
    std::erase_if(popups, [](ItemPopup& p) {
        if (p.timer <= 0) { delete p.nameText; delete p.descText; return true; }
        return false;
        });
}

void PlayerHUD::Draw(Renderer* renderer)
{
    if (WeaponSpriteInitialized && weaponSprite)
    {
        weaponSprite->Draw(renderer);
    }

    HandleHUDElementsDraw(renderer);
    int popupY = HEIGHT - 100;
    for (int i = (int)popups.size() - 1; i >= 0; i--) {
        auto& p = popups[i];
        float alpha = min(p.timer / 3.0f, 1.0f) * 255.0f;
        int popupX = WIDTH - 420;
        renderer->AddFilledRect(popupX - 10, popupY - 5, 410, 55, { 0, 0, 0, alpha * 0.6f }, RenderLayer::UI);
        p.nameText->SetColor({ 255, 220, 50, alpha });
        p.nameText->SetPosition(popupX, popupY);
        p.nameText->Draw(renderer);
        p.descText->SetColor({ 200, 200, 200, alpha });
        p.descText->SetPosition(popupX, popupY + 28);
        p.descText->Draw(renderer);
        popupY -= 65;
    }
}


void PlayerHUD::HandleHUDElementsProcess(float deltaTime) {
    for (auto& hudElement : playerHudElements) {
        hudElement->Process(deltaTime);
    }
}

void PlayerHUD::HandleHUDElementsDraw(Renderer* renderer) {
    for (auto& hudElement : playerHudElements) {
        hudElement->Draw(renderer);
    }
}

void PlayerHUD::HandleGunSpriteRotation() {
    if (weaponSprite)
    {
        Vector2 playerCenter = player->GetPosition();
        Vector2 weaponSize = weaponSprite->GetDrawSize();

        weaponSprite->SetPosition((int)(playerCenter.x - weaponSize.x * 0.5f), (int)(playerCenter.y - weaponSize.y * (-1.15f)));

        //follows mouse position, rotates weapon to point to it
        Vector2 mouseWorldPosition = context.im->GetMouseWorldPosition(context.renderer->cam);
        float angle = atan2(mouseWorldPosition.y - playerCenter.y, mouseWorldPosition.x - playerCenter.x) * 180.0f / PI;
        weaponSprite->SetRotation(angle);

        weaponSprite->SetFlip(true);
        if (angle > 90 || angle < -90)//flips weapon if the mouse is on the left side of the player to prevent it from being upside down
        {
            weaponSprite->SetVerticalFlip(true);
        }
    }
}



void PlayerHUD::SetBulletTexture()
{
    switch (bulletType)
    {
    case BulletType::MachineGunBullet:
        //delete BulletTexture;
        //BulletTexture = nullptr;
        BulletTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Bullets/AR_SR_DMR_Machinegun_bullet.png");
        break;
    case BulletType::ShotgunBullet:
        //delete BulletTexture;
        //BulletTexture = nullptr;
        BulletTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Bullets/Shotguns_bullets.png");
        break;
    case BulletType::PistolBullet:
        //delete BulletTexture;
        //BulletTexture = nullptr;
        BulletTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Bullets/SMG_Pistols_bullets.png");
        break;
    }
}

void PlayerHUD::SetBulletSprite() {
    int bulletTextureW = 0;
    int bulletTextureH = 0;
    if (BulletTexture)
    {
        SDL_QueryTexture(BulletTexture, nullptr, nullptr, &bulletTextureW, &bulletTextureH);
    }

    bulletSprite = new Sprite();
    bulletSprite->Initialize(BulletTexture, bulletTextureW, bulletTextureH, 0, 0, 50, 50, RenderLayer::PLAYER);
    bulletSprite->SetDrawLayer(RenderLayer::PLAYER);
    BulletSpriteInitialized = true;

}

void PlayerHUD::SetWeaponTexture()
{
    switch (weaponType)
    {
    case WeaponType::Ar_1:
        //delete WeaponTexture;
        //WeaponTexture = nullptr;
        WeaponTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Weapons/Ar_1.png");
        break;
    case WeaponType::Ar_2:
        //delete WeaponTexture;
        //WeaponTexture = nullptr;
        WeaponTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Weapons/Ar_2.png");
        break;
    case WeaponType::Ar_3:
        //delete WeaponTexture;
        //WeaponTexture = nullptr;
        WeaponTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Weapons/Ar_3.png");
        break;
    case WeaponType::Ar_4:
        //delete WeaponTexture;
        //WeaponTexture = nullptr;
        WeaponTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Weapons/Ar_4.png");
        break;
    case WeaponType::Ar_5:
        //delete WeaponTexture;
        //WeaponTexture = nullptr;
        WeaponTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Weapons/Ar_5.png");
        break;
    case WeaponType::Ar_6:
        //delete WeaponTexture;
        //WeaponTexture = nullptr;
        WeaponTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Weapons/Ar_6.png");
        break;
    case WeaponType::Ar_7:
        //delete WeaponTexture;
        //WeaponTexture = nullptr;
        WeaponTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Weapons/Ar_7.png");
        break;
    case WeaponType::Ar_8:
        //delete WeaponTexture;
        //WeaponTexture = nullptr;
        WeaponTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Weapons/Ar_8.png");
        break;
    case WeaponType::Ar_9:
        //delete WeaponTexture;
        //WeaponTexture = nullptr;
        WeaponTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Weapons/Ar_9.png");
        break;
    case WeaponType::Ar_10:
        //delete WeaponTexture;
        //WeaponTexture = nullptr;
        WeaponTexture = context.txm->LoadTexture(context.renderer, "../../assets/sprites/Weapons/Ar_10.png");
        break;
    }
}

void PlayerHUD::SetWeapoinSprite() {
    int weaponTextureW = 0;
    int weaponTextureH = 0;
    if (WeaponTexture)
    {
        SDL_QueryTexture(WeaponTexture, nullptr, nullptr, &weaponTextureW, &weaponTextureH);
    }

    weaponSprite = new Sprite();
    weaponSprite->Initialize(WeaponTexture, weaponTextureW, weaponTextureH, 0, 0, 100, 100, RenderLayer::PLAYER);
    weaponSprite->SetDrawLayer(RenderLayer::PLAYER);
    WeaponSpriteInitialized = true;

}

void PlayerHUD::SetCashoutHUD() {
    // place statsheet HUD
    auto cashoutOverlay = new CashoutOverlay(
        WIDTH * 0.25,
        HEIGHT * 0.95,
        WIDTH * 0.5,
        HEIGHT * 0.05,
        {0,0,0,0},
        {0,0,0,0},
        0,
        0
    );
;
    cashoutOverlay->Initialize(player->m_inventory);
    playerHudElements.push_back(cashoutOverlay);
}

void PlayerHUD::SetStatSheetHUD() {
    // place statsheet HUD
    auto statSheetOverlay = new StatSheetOverlay(
        0,
        0,
        WIDTH * 0.2,
        HEIGHT * 0.5,
        {0,0,0,0},
        {0,0,0,0},
        0,
        0
    );
    statSheetOverlay->Initialize(player->m_pStats, player->m_inventory, 50, 150);
    playerHudElements.push_back(statSheetOverlay);
}

void PlayerHUD::SetInventoryHUD() {
    // place inventory HUD
    auto inventoryOverlay = new InventoryOverlay(
        WIDTH * 0.80,
        0,
        WIDTH * 0.20,
        HEIGHT * 0.6,
        {0,0,0,0},
        {0,0,0,0},
        0,
        0
    );
    inventoryOverlay->Initialize(player->m_inventory, 50, 150);
    playerHudElements.push_back(inventoryOverlay);
}

void PlayerHUD::HandleTimerUpdate() {

}

void PlayerHUD::PushPopup(const std::string& name, const std::string& desc) {
    const std::string fontPath = "../../assets/fonts/pixelpurl/PixelPurl.ttf";
    ItemPopup p;
    p.nameText = new Text();
    p.nameText->Initialize(name, fontPath, 28);
    p.descText = new Text();
    p.descText->Initialize(desc, fontPath, 20);
    p.timer = 3.0f;
    popups.push_back(p);
}

void PlayerHUD::SetDifficultyHUD() {
    // place inventory HUD
    auto difficultyOverlay = new DifficultyOverlay(
        WIDTH * 0.33,
        0,
        WIDTH * 0.33,
        HEIGHT * 0.1,
        {0,0,0,0},
        {0,0,0,0},
        0,
        0
    );
    difficultyOverlay->Initialize();
    playerHudElements.push_back(difficultyOverlay);
}
