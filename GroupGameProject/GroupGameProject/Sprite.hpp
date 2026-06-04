#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <SDL.h>
#include "Renderer.hpp"

class GameContext;
class Sprite {
public:
    Sprite();
    Sprite(const Sprite& other);
    virtual Sprite* Clone() const;
    virtual ~Sprite();

    bool Initialize(SDL_Texture* tex, int srcWidth, int srcHeight, int srcX, int srcY, int drawWidth, int drawHeight, RenderLayer l = RenderLayer::OBJECTS, int sl = 0);

    virtual void Process(float deltaTime);
    virtual void Draw(Renderer* renderer);

    virtual void SetPosition(int x, int y);
    void SetPosition(Vector2 vec);
    void SetRotation(float angle);
    void Rotate(float amount);
    void SetColor(Color c);
    void SetAlpha(float a);
    void SetDrawSize(int w, int h);
    void SetDrawLayer(RenderLayer l, int sl = 0);
    void SetIsFlashing(bool flash);
    void SetFlip(bool flipH);
    void SetVerticalFlip(bool flipV);

    virtual int GetWidth() const { return dstRect.w; };
    virtual int GetHeight() const { return dstRect.h; };
    Vector2 GetPosition() const { return Vector2(dstRect.x, dstRect.y); }
    Vector2 GetDrawSize() const { return Vector2(dstRect.w, dstRect.h); }
    float GetRotation() const { return rotation; }
    Color GetColor() const { return color; }
    float GetAlpha() const { return color.a; }

    void Spin(float time);

protected:
    SDL_Texture* texture;
    Color color;
    float rotation;
    RenderLayer layer;
    int subLayer;
    SDL_RendererFlip flip;

    SDL_Rect srcRect;
    SDL_Rect dstRect;

    bool isFlashing;//on hit
    float spinDuration;
    float spinTimer;

};

#endif