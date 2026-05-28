#include "Renderer.hpp"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <algorithm>
#include "Camera.hpp"

Renderer::Renderer() {
    window = nullptr;
    renderer = nullptr;
}

Renderer::~Renderer() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

bool Renderer::Initialize(const string& windowTitle, int width, int height, bool fullscreen) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    //create window
    window = SDL_CreateWindow(
        windowTitle.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0
    );
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        return false;
    }
    SDL_GetWindowSize(window, &WIDTH, &HEIGHT);

    //create renderer
    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    //create camera
    cam = new Camera();

    return true;
}


//ADDING TO QUEUE
void Renderer::DrawTexture(SDL_Texture* texture, SDL_Rect* srcRect, SDL_Rect* dstRect, Color c, int rotation, RenderLayer layer, int subOrder, bool bm, SDL_RendererFlip f) {
    if (!texture || !renderer) return;

    RenderCommand cmd;
    cmd.texture = texture;
    cmd.srcRect = srcRect ? *srcRect : SDL_Rect{ 0,0,0,0 };
    cmd.dstRect = dstRect ? *dstRect : SDL_Rect{ 0,0,0,0 };
    cmd.rotation = rotation;
    cmd.layer = layer;
    cmd.subOrder = subOrder;
    cmd.color = c;
    cmd.blendMode = bm;
    cmd.flip = f;
    renderQueue.push_back(cmd);
}

void Renderer::AddDrawRect(float x, float y, float w, float h, Color c, RenderLayer layer) {
    boundingQueue.push_back({ ShapeType::Square, x, y, w, h, 0, {0,0}, 0, c, layer });
}

void Renderer::AddDrawCircle(float cx, float cy, float radius, Color c, RenderLayer layer) {
    boundingQueue.push_back({ ShapeType::Circle, cx, cy, 0, 0, radius, {0,0}, 0, c, layer });
}

void Renderer::AddDrawCone(float cx, float cy, float radius, Vector2 direction, float halfAngle, Color c, RenderLayer layer) {
    boundingQueue.push_back({ ShapeType::Cone, cx, cy, 0, 0, radius, direction, halfAngle, c, layer });
}

void Renderer::AddFilledRect(float x, float y, float w, float h, Color c, RenderLayer layer) {
    boundingQueue.push_back({ ShapeType::FilledSquare, x, y, w, h, 0, {0,0}, 0, c, layer });
}


//PROCESSING QUEUE
void Renderer::BeginFrame() {
    SDL_SetRenderDrawColor(renderer, 10, 10, 50, 255);
    SDL_RenderClear(renderer);
}

void Renderer::EndFrame() {
    //organize queues
    stable_sort(renderQueue.begin(), renderQueue.end(), CompareRenderCommands);
    stable_sort(boundingQueue.begin(), boundingQueue.end(), CompareBoundingCommands);

    int bi = 0;
    for (auto& cmd : renderQueue) {
        //flush any bounding commands that belong before this render layer
        while (bi < boundingQueue.size() && static_cast<int>(boundingQueue[bi].layer) < static_cast<int>(cmd.layer)) {
            DrawSingleBoundingCommand(boundingQueue[bi]);
            bi++;
        }
        DrawSingleRenderCommand(cmd);
    }

    //flush any remaining bounding commands after all sprites
    while (bi < boundingQueue.size()) {
        DrawSingleBoundingCommand(boundingQueue[bi]);
        bi++;
    }

    renderQueue.clear();
    boundingQueue.clear();
    SDL_RenderPresent(renderer);
}

//draws normal sprites
void Renderer::DrawSingleRenderCommand(const RenderCommand& cmd) {
    if (cmd.blendMode)
        SDL_SetTextureBlendMode(cmd.texture, SDL_BLENDMODE_ADD);
    else
        SDL_SetTextureBlendMode(cmd.texture, SDL_BLENDMODE_BLEND);

    SDL_SetTextureColorMod(cmd.texture, cmd.color.r, cmd.color.g, cmd.color.b);
    SDL_SetTextureAlphaMod(cmd.texture, cmd.color.a);
    SDL_Rect v = cmd.layer >= RenderLayer::UI_SUBLAYER ? cmd.dstRect : cam->GetScreenRect(&cmd.dstRect);//UI elements are stagnant
    SDL_Point center = { v.w / 2, v.h / 2 };
    SDL_RenderCopyEx(renderer, cmd.texture, &cmd.srcRect, &v, cmd.rotation, &center, cmd.flip);
    SDL_SetTextureColorMod(cmd.texture, 255, 255, 255);
    SDL_SetTextureAlphaMod(cmd.texture, 255);
}

//draws shapes for UI or debuging bounding boxes
void Renderer::DrawSingleBoundingCommand(const BoundingDrawCommand& cmd) {
    SDL_SetRenderDrawColor(renderer, cmd.c.r, cmd.c.g, cmd.c.b, cmd.c.a);
    if (cmd.type == ShapeType::Square)
        DrawBoundingRect((int)cmd.x, (int)cmd.y, (int)cmd.w, (int)cmd.h);
    else if (cmd.type == ShapeType::Circle)
        DrawBoundingCircle((int)cmd.x, (int)cmd.y, cmd.radius);
    else if (cmd.type == ShapeType::Cone)
        DrawBoundingCone((int)cmd.x, (int)cmd.y, cmd.radius, cmd.direction, cmd.halfAngle);
    else if (cmd.type == ShapeType::FilledSquare) {
        DrawFilledRect((int)cmd.x, (int)cmd.y, (int)cmd.w, (int)cmd.h, cmd.c,  cmd.layer);
    }
}


//SHAPE DRAWING
void Renderer::DrawBoundingRect(int x, int y, int w, int h) {
    SDL_Rect world = { x, y, w, h };
    SDL_Rect screen = cam->GetScreenRect(&world);
    SDL_RenderDrawRect(renderer, &screen);
}

void Renderer::DrawBoundingCircle(int cx, int cy, int radius) {
    SDL_Rect centerWorld = { cx, cy, 0, 0 };
    SDL_Rect centerScreen = cam->GetScreenRect(&centerWorld);
    float zoom = cam->GetZoom(); // see note below
    const int segments = 32;
    for (int i = 0; i < segments; ++i) {
        float a0 = (2.0f * PI * i) / segments;
        float a1 = (2.0f * PI * (i + 1)) / segments;
        SDL_RenderDrawLineF(renderer,
            centerScreen.x + cos(a0) * radius * zoom,
            centerScreen.y + sin(a0) * radius * zoom,
            centerScreen.x + cos(a1) * radius * zoom,
            centerScreen.y + sin(a1) * radius * zoom);
    }
}

void Renderer::DrawBoundingCone(int cx, int cy, int radius, Vector2 direction, float halfAngle) {
    SDL_Rect centerWorld = { cx, cy, 0, 0 };
    SDL_Rect centerScreen = cam->GetScreenRect(&centerWorld);
    float zoom = cam->GetZoom();
    float baseAngle = atan2(direction.y, direction.x);
    const int segments = 32;
    for (int i = 0; i < segments; ++i) {
        float a0 = baseAngle - halfAngle + (2.0f * halfAngle * i) / segments;
        float a1 = baseAngle - halfAngle + (2.0f * halfAngle * (i + 1)) / segments;
        SDL_RenderDrawLineF(renderer,
            centerScreen.x + cos(a0) * radius * zoom,
            centerScreen.y + sin(a0) * radius * zoom,
            centerScreen.x + cos(a1) * radius * zoom,
            centerScreen.y + sin(a1) * radius * zoom);
    }
    float left = baseAngle - halfAngle;
    float right = baseAngle + halfAngle;
    SDL_RenderDrawLineF(renderer,
        centerScreen.x, centerScreen.y,
        centerScreen.x + cos(left) * radius * zoom,
        centerScreen.y + sin(left) * radius * zoom);
    SDL_RenderDrawLineF(renderer,
        centerScreen.x, centerScreen.y,
        centerScreen.x + cos(right) * radius * zoom,
        centerScreen.y + sin(right) * radius * zoom);
}

void Renderer::DrawFilledRect(int x, int y, int w, int h, Color c, RenderLayer layer) {
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
    SDL_Rect rect = { x, y, w, h };
    SDL_Rect screen = layer >= RenderLayer::UI_SUBLAYER ? rect : cam->GetScreenRect(&rect);//UI elements are static
    SDL_RenderFillRect(renderer, &screen);
}