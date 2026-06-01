#include "AnimatedSprite.hpp"
#include "Renderer.hpp"

AnimatedSprite::AnimatedSprite() {
    frameWidth = 0;
    frameHeight = 0;
    srcOffsetX = 0;
    srcOffsetY = 0;
    totalFrames = 0;
    framesPerRow = 0;
    currentFrame = 0;
    frameDuration = 0.1f;
    timeElapsed = 0.0f;
    isAnimating = false;
    isLooping = true;
}

AnimatedSprite::AnimatedSprite(const AnimatedSprite& other)
    : Sprite(other) {
    frameWidth = other.frameWidth;
    frameHeight = other.frameHeight;
    srcOffsetX = other.srcOffsetX;
    srcOffsetY = other.srcOffsetY;
    totalFrames = other.totalFrames;
    framesPerRow = other.framesPerRow;
    currentFrame = other.currentFrame;
    frameDuration = other.frameDuration;
    timeElapsed = other.timeElapsed;
    isAnimating = other.isAnimating;
    isLooping = other.isLooping;
    leaveOnLastFrame = other.leaveOnLastFrame;
}

Sprite* AnimatedSprite::Clone() const {
    return new AnimatedSprite(*this); // calls AnimatedSprite copy constructor
}

AnimatedSprite::~AnimatedSprite() {}

bool AnimatedSprite::Initialize(SDL_Texture* tex, int srcWidth, int srcHeight, int srcX, int srcY, int drawWidth, int drawHeight, int fPerRow, int totalf) {
    frameWidth = srcWidth;
    frameHeight = srcHeight;
    srcOffsetX = srcX;
    srcOffsetY = srcY;
    totalFrames = totalf;
    framesPerRow = fPerRow;

    leaveOnLastFrame = false;

    Sprite::Initialize(tex, srcWidth, srcHeight, srcX, srcY, drawWidth, drawHeight);

    return true;
}

void AnimatedSprite::Process(float deltaTime) {
    Sprite::Process(deltaTime);
    if (!isAnimating) return;

    timeElapsed += deltaTime;

    if (timeElapsed >= frameDuration) {
        timeElapsed = 0.0f;
        ++currentFrame;

        if (currentFrame >= totalFrames) {
            if (isLooping) {
                Restart();
            }
            else {
                currentFrame = totalFrames - 1;//leave on last frame
                isAnimating = false;
            }
        }

        //update frame
        int col = currentFrame % framesPerRow;
        int row = currentFrame / framesPerRow;

        srcRect.x = srcOffsetX + (col * srcRect.w);
        srcRect.y = srcOffsetY + (row * srcRect.h);
    }
}

void AnimatedSprite::Draw(Renderer* renderer) {
    if (isAnimating || leaveOnLastFrame) Sprite::Draw(renderer);
}

void AnimatedSprite::Animate() { isAnimating = true; }
void AnimatedSprite::Pause() { isAnimating = false; }
void AnimatedSprite::Restart() { currentFrame = 0; timeElapsed = 0.0f; }
bool AnimatedSprite::IsAnimating() const { return isAnimating; }
void AnimatedSprite::SetLooping(bool loop) { isLooping = loop; }
void AnimatedSprite::SetLeaveOnLastFrame(bool g) { leaveOnLastFrame = g; }
void AnimatedSprite::SetFrameDuration(float s) { frameDuration = s; }

void AnimatedSprite::SetFrame(int i) {

    currentFrame = i;

    int col = currentFrame % framesPerRow;
    int row = currentFrame / framesPerRow;

    srcRect.x = srcOffsetX + (col * srcRect.w);
    srcRect.y = srcOffsetY + (row * srcRect.h);
}