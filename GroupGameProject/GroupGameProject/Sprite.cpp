#include "Sprite.hpp"
#include "Renderer.hpp"

Sprite::Sprite() {
    texture = nullptr;
    srcRect = { 0, 0, 0, 0 };
    dstRect = { 0, 0, 0, 0 };
    rotation = 0.0f;
    color = Color(255, 255, 255, 255);
}

Sprite::Sprite(const Sprite& other) {
    texture = other.texture;
    color = other.color;
    srcRect = other.srcRect;
    dstRect = other.dstRect;
    rotation = other.rotation;
    layer = other.layer;
    subLayer = other.subLayer;
    flip = other.flip;
}

Sprite* Sprite::Clone() const {
    return new Sprite(*this);
}

Sprite::~Sprite() {}

//srcRect source size and location
// srcX/Y is top left corner
//destRect where to draw and how big
bool Sprite::Initialize(SDL_Texture* tex, int srcWidth, int srcHeight, int srcX, int srcY, int drawWidth, int drawHeight, RenderLayer l, int sl) {
    texture = tex;

    srcRect.x = srcX;
    srcRect.y = srcY;
    srcRect.w = srcWidth;
    srcRect.h = srcHeight;

    dstRect.w = drawWidth;
    dstRect.h = drawHeight;

    layer = l;
    subLayer = sl;
    flip = SDL_FLIP_NONE;

    return true;
}

void Sprite::Process(float deltaTime) {
    if (spinTimer > 0) {
        float rotateAmount = (deltaTime / spinDuration) * 360;
        rotateAmount *= flip == SDL_FLIP_NONE ? -1 : 1;
        Rotate(rotateAmount);
        spinTimer -= deltaTime;
    }
}

//draw using renderer
void Sprite::Draw(Renderer* renderer) {
    if (texture) {
		if (isFlashing) {
			renderer->DrawTexture(texture, &srcRect, &dstRect, { 250, 250, 250, 200 }, rotation, layer, subLayer, isFlashing, flip);
		}
        else {
            renderer->DrawTexture(texture, &srcRect, &dstRect, color, rotation, layer, subLayer, false, flip);
        }
    }

}

void Sprite::SetPosition(int x, int y) {
    dstRect.x = x;
    dstRect.y = y;
}

void Sprite::SetPosition(Vector2 vec) {
    dstRect.x = vec.x;
    dstRect.y = vec.y;
}

void Sprite::SetRotation(float angle) {
    rotation = angle;
}

void Sprite::Rotate(float amount) {
    rotation += amount;
}

void Sprite::SetColor(Color c) {
    color = c;
}

void Sprite::SetAlpha(float a) {
    color.a = a;
}

void Sprite::SetDrawSize(int w, int h) {
    dstRect.w = w;
    dstRect.h = h;
}

void Sprite::SetDrawLayer(RenderLayer l, int sl) {
    layer = l;
    subLayer = sl;
}

void Sprite::SetIsFlashing(bool flash) {
    isFlashing = flash;
}

void Sprite::SetFlip(bool flipH) {
    flip = flipH ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
}

//this is for the vertical flip, adds on an existing horizontal flip if there is one
void Sprite::SetVerticalFlip(bool flipV) 
{ 
    flip = (SDL_RendererFlip)(flipV | SDL_FLIP_VERTICAL);
}

void Sprite::Spin(float time) {
    spinDuration = time;
    spinTimer = time;
}