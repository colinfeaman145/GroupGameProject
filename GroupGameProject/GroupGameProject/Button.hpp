#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <functional>
#include "Renderer.hpp"
#include "GameContext.hpp"
#include "Element.hpp"
#include "Sprite.hpp"
#include "Text.hpp"

//NOTE: image will always be centered. If a button requires more customization:
//Create a container, set it to 0 alpha, add a button, add customizations to container

using namespace std;
class Button : public Sprite {
public:
    Button(int x, int y, int w, int h, Color fillColor, Color hoverFillColor, Color borderColor,
        Color borderHoverColor, int borderThickness, function<void()> onClick, float hoverScale, float alpha = 255);

    void Process(float deltaTime) override;
    void Draw(Renderer* renderer) override;

    void SetPosition(int x, int y);
    void SetSize(int w, int h);
    void SetImage(Sprite* img);
    void SetBorderColor(Color c);

    int GetWidth() const override { return w; };
    int GetHeight() const override { return h; };

private:
    int x, y, w, h;
    Color fillColor, hoverFillColor;
    Color borderColor, borderHoverColor;
    int borderThickness;
    function<void()> onClick;
    bool isHovered;
    float hoverScale;

    Sprite* image;//can be text
    Vector2 baseImageSize;
};

#endif