#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SDL.h>
#include <string>
#include <vector>
#include "EnumTypes.hpp"


class Camera;
struct Color {
    float r;
    float g;
    float b;
    float a;
};

enum class RenderLayer {
    GROUND = 0,
    STRUCTURES = 1,
    OBJECTS = 2,
    ENEMIES = 3,
    ATTACK_CONE = 4,
    PLAYER = 5,
    NATURE = 6,
    PARTICLE = 7,
    PERCENTBAR = 8,
    DEBUG = 9,
    UI_SUBLAYER = 10,
    UI = 11
};

struct RenderCommand {
    SDL_Texture* texture;
    SDL_Rect srcRect;
    SDL_Rect dstRect;
    float rotation;
    RenderLayer layer;
    int subOrder;
    Color color;
    SDL_RendererFlip flip;
    bool blendMode;
};

struct BoundingDrawCommand {
    ShapeType type;
    float x, y, w, h;
    float radius;
    Vector2 direction;
    float halfAngle;
    Color c;
    RenderLayer layer;
};

using namespace std;
class Renderer {
public:
    Renderer();
    ~Renderer();
    Camera* cam;

    bool Initialize(const string& windowTitle, int width, int height, bool fullscreen = false);
    void BeginFrame();
    void EndFrame();

    // draw a texture
    void DrawTexture(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* dstRect, Color c, int rotation = 0,
        RenderLayer layer = RenderLayer::OBJECTS, int subOrder = 0, bool bm = false, SDL_RendererFlip f = SDL_FLIP_NONE);
    void AddDrawRect(float x, float y, float w, float h, Color c, RenderLayer layer);
    void AddDrawCircle(float cx, float cy, float radius, Color c, RenderLayer layer);
    void AddDrawCone(float cx, float cy, float radius, Vector2 direction, float halfAngle, Color c, RenderLayer layer);
    void AddFilledRect(float x, float y, float w, float h, Color c, RenderLayer layer);

    SDL_Renderer* GetSDLRenderer() const { return renderer; }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    vector<RenderCommand> renderQueue;
    vector<BoundingDrawCommand> boundingQueue;

    inline static bool CompareRenderCommands(const RenderCommand& a, const RenderCommand& b) {
        if (a.layer != b.layer)
            return static_cast<int>(a.layer) < static_cast<int>(b.layer);
        return a.subOrder < b.subOrder;
    }

    inline static bool CompareBoundingCommands(const BoundingDrawCommand& a, const BoundingDrawCommand& b) {
        return static_cast<int>(a.layer) < static_cast<int>(b.layer);
    }

    void DrawSingleRenderCommand(const RenderCommand& cmd);
    void DrawSingleBoundingCommand(const BoundingDrawCommand& cmd);

    void DrawBoundingRect(int x, int y, int w, int h);
    void DrawBoundingCircle(int cx, int cy, int radius);
    void DrawBoundingCone(int cx, int cy, int radius, Vector2 direction, float halfAngle);
    void DrawFilledRect(int x, int y, int w, int h, Color c, RenderLayer layer);
};

#endif