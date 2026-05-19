#include "Button.hpp"

Button::Button(int x, int y, int w, int h, Color fillColor, Color hoverFillColor, Color borderColor,
    Color borderHoverColor, int borderThickness, function<void()> onClick, float hoverScale, float alpha) {
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    this->fillColor = fillColor;
    this->hoverFillColor = hoverFillColor;
    this->borderColor = borderColor;
    this->borderHoverColor = borderHoverColor;
    this->borderThickness = borderThickness;
    this->onClick = onClick;
    this->hoverScale = hoverScale;
    isHovered = false;
    SetDrawLayer(RenderLayer::UI_SUBLAYER);
}

void Button::Process(float deltaTime) {
    if (image) image->Process(deltaTime);
    isHovered = context.im->IsMouseOver(x, y, w, h);

    if (isHovered) context.im->SetIsMouseOverUI(true);

    if (isHovered && context.im->IsMouseButtonReleased(SDL_BUTTON_LEFT)) {
        onClick();
    }
}

void Button::Draw(Renderer* renderer) {
    Color currentFill = isHovered ? hoverFillColor : fillColor;
    Color currentBorder = isHovered ? borderHoverColor : borderColor;

    int drawX = x;
    int drawY = y;
    int drawW = w;
    int drawH = h;

    if (isHovered) {
        // expand from center
        int expandW = (int)(w * hoverScale) - w;
        int expandH = (int)(h * hoverScale) - h;
        drawX -= expandW / 2;
        drawY -= expandH / 2;
        drawW += expandW;
        drawH += expandH;
    }

    //center image in button
    if (image) {
        int imgW = isHovered ? (int)(baseImageSize.x * hoverScale) : baseImageSize.x;
        int imgH = isHovered ? (int)(baseImageSize.y * hoverScale) : baseImageSize.y;
        image->SetDrawSize(imgW, imgH);

        //center inside button
        int imgX = drawX + ((drawW - imgW) / 2);
        int imgY = drawY + ((drawH - imgH) / 2);
        image->SetPosition(imgX, imgY);
        image->Draw(renderer);

    }

    //draw border as outer filled rect
    renderer->AddFilledRect(drawX, drawY, drawW, drawH, currentBorder, RenderLayer::UI_SUBLAYER);

    //draw filled inset by border thickness
    renderer->AddFilledRect(
        drawX + borderThickness,
        drawY + borderThickness,
        drawW - borderThickness * 2,
        drawH - borderThickness * 2,
        currentFill, RenderLayer::UI_SUBLAYER);
}

void Button::SetPosition(int x, int y) {
    this->x = x;
    this->y = y;
}

void Button::SetSize(int w, int h) {
    this->w = w;
    this->h = h;
}

void Button::SetImage(Sprite* img) {
    image = img;
    image->SetDrawLayer(RenderLayer::UI);
    baseImageSize = image->GetDrawSize();
}

void Button::SetBorderColor(Color c) {
    borderColor = c;
}