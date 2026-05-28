#ifndef PERCENTAGE_BAR_HPP
#define PERCENTAGE_BAR_HPP

#include "Renderer.hpp"

class PercentageBar {
public:
    PercentageBar(const PercentageBar& other);
    PercentageBar(float current, float max, int w, int h, Color fillColor, Color outlineColor, RenderLayer renderLayer);

    void SetValues(float current, float max);
    void SetPosition(int x, int y);
    void SetOffset(int offX, int offY);
    void SetFillColor(Color c);
    void SetOutlineColor(Color c);
    int GetWidth();
    int GetHeight();
    void Draw(Renderer* renderer);


private:
    float current;
    float max;
    int w;
    int h;
    int x;
    int y;
    int offsetX;
    int offsetY;
    Color fillColor;
    Color outlineColor;
    RenderLayer renderLayer;
};

#endif