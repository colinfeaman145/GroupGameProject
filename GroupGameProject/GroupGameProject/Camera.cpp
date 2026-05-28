#include "Camera.hpp"
#include "Grid.hpp"
#include "GameContext.hpp"

Camera::Camera() {
    x = y = 0;
    zoom = .5;
}

void Camera::Move(float dx, float dy, float deltaTime) {
    x += dx * CAM_SPEED * deltaTime;
    y += dy * CAM_SPEED * deltaTime;
}

void Camera::Follow(Vector2 pos) {
    x = pos.x - WIDTH / (2.0 * zoom);
    y = pos.y - HEIGHT / (2.0 * zoom);

    //clamp so camera doesnt go outside grid
    float worldWidth = context.grid->GetGridWidth() * context.grid->GetCellSize();
    float worldHeight = context.grid->GetGridHeight() * context.grid->GetCellSize();

    float viewWidth = WIDTH / zoom;
    float viewHeight = HEIGHT / zoom;

    x = max(0.0f, min(x, worldWidth - viewWidth));
    y = max(0.0f, min(y, worldHeight - viewHeight));
}

void Camera::SetZoom(float z) {
    zoom = z;
}

void Camera::AdjustZoom(float amount) {
    float worldWidth = context.grid->GetGridWidth() * context.grid->GetCellSize();
    float worldHeight = context.grid->GetGridHeight() * context.grid->GetCellSize();

    //can't zoom out past world bounds
    float minZoom = max((float)WIDTH / worldWidth, (float)HEIGHT / worldHeight) * 1.01f;

    zoom += amount;
    zoom = max(zoom, minZoom);
    zoom = min(zoom, 2.0f);
}

SDL_Rect Camera::GetScreenRect(const SDL_Rect* spriteRect) {
    return SDL_Rect{
        (int)round((spriteRect->x - x) * zoom),
        (int)round((spriteRect->y - y) * zoom),
        (int)round(spriteRect->w * zoom),
        (int)round(spriteRect->h * zoom)
    };
}