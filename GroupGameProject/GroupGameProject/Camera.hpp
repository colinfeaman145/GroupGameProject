#ifndef CAMERA_HPP
#define CAMERA_HPP
#include <SDL.h>
#include "Vector2.hpp"

inline int WIDTH = 0;
inline int HEIGHT = 0;
#define CAM_SPEED 10 //only used for Move function
#define RENDER_DISTANCE 10 

class Camera {
public:
    Camera();
    void Move(float dx, float dy, float deltaTime);
    void Follow(Vector2 pos);
    void SetZoom(float z);
    void SetSpeed(float spd);
    void AdjustZoom(float amount);

    SDL_Rect GetScreenRect(const SDL_Rect* spriteRect);
    float GetX() const { return x; }
    float GetY() const { return y; }
    int GetWidth() const { return WIDTH; }
    int GetHeight() const { return HEIGHT; }
    float GetZoom() const { return zoom; }
    SDL_Rect GetViewportWorldRect() const {
        return {
            (int)x,
            (int)y,
            (int)(WIDTH / zoom),
            (int)(HEIGHT / zoom)
        };
    }

private:
    float x, y;
    float zoom;
};

#endif