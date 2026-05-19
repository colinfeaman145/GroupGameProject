#ifndef ANIMATED_SPRITE_HPP
#define ANIMATED_SPRITE_HPP
#include "Sprite.hpp"

class AnimatedSprite : public Sprite {
public:
    AnimatedSprite();
    AnimatedSprite(const AnimatedSprite& other);
    Sprite* Clone() const;
    ~AnimatedSprite();

    bool Initialize(SDL_Texture* tex, int srcWidth, int srcHeight, int srcX, int srcY, int drawWidth, int drawHeight, int fPerRow, int totalf);
    void Process(float deltaTime) override;
    void Draw(Renderer* renderer) override;

    void SetFrameDuration(float seconds);
    void SetLooping(bool loop);
    void SetLeaveOnLastFrame(bool g);
    void Animate();
    void Pause();
    void Restart();
    bool IsAnimating() const;
    void SetFrame(int i);

private:
    int frameWidth;
    int frameHeight;
    int srcOffsetX;
    int srcOffsetY;
    int totalFrames;
    int framesPerRow;
    int currentFrame;
    float frameDuration;
    float timeElapsed;
    bool isAnimating;
    bool isLooping;
    bool leaveOnLastFrame;
};


#endif