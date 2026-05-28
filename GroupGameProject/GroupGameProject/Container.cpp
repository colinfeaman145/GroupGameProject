#include "Container.hpp"
#include "Button.hpp"
#include "Text.hpp"
#include "Renderer.hpp"

Container::Container(int x, int y, int w, int h, Color fillColor, Color borderColor, int alpha, int borderThickness) {
    this->fillColor = fillColor;
    this->borderColor = borderColor;
    this->borderThickness = borderThickness;
    this->alpha = alpha;
    visible = true;
    dstRect = { x, y, w, h };
    SetDrawLayer(RenderLayer::UI, -1);
    size = { (float)w,(float)h };
}

void Container::AddSprite(Sprite* sprite, int localX, int localY) {
    sprites.push_back({ sprite, localX, localY });
    sprite->SetPosition(dstRect.x + localX, dstRect.y + localY);
    sprite->SetDrawLayer(RenderLayer::UI, 1);
}

void Container::AddButton(Button* button, int localX, int localY) {
    buttons.push_back({ button, localX, localY });
    button->SetPosition(dstRect.x + localX, dstRect.y + localY);
    button->SetDrawLayer(RenderLayer::UI_SUBLAYER);
}

void Container::AddText(Text* text, int localX, int localY) {
    texts.push_back({ text, localX, localY });
    text->SetPosition(dstRect.x + localX, dstRect.y + localY);
    text->SetDrawLayer(RenderLayer::UI, 2);
}

void Container::Process(float deltaTime) {
    if (!visible) return;

    for (auto& item : buttons)
        item.button->Process(deltaTime);

    for (auto& item : sprites)
        item.sprite->Process(deltaTime);

    for (auto& item : texts)
        item.text->Process(deltaTime);
}

void Container::Draw(Renderer* renderer) {
    if (!visible) return;

    //draw border as outer filled rect
    renderer->AddFilledRect(dstRect.x, dstRect.y, dstRect.w, dstRect.h, borderColor, RenderLayer::UI_SUBLAYER);

    //draw fill inset by border thickness
    renderer->AddFilledRect(
        dstRect.x + borderThickness, dstRect.y + borderThickness,
        dstRect.w - borderThickness * 2, dstRect.h - borderThickness * 2,
        fillColor, RenderLayer::UI_SUBLAYER);

    for (auto& item : sprites)
        item.sprite->Draw(renderer);

    for (auto& item : buttons)
        item.button->Draw(renderer);

    for (auto& item : texts)
        item.text->Draw(renderer);
}

void Container::SetPosition(int newX, int newY) {
    dstRect.x = newX;
    dstRect.y = newY;

    for (auto& item : sprites)
        item.sprite->SetPosition(dstRect.x + item.localX, dstRect.y + item.localY);

    for (auto& item : buttons)
        item.button->SetPosition(dstRect.x + item.localX, dstRect.y + item.localY);

    for (auto& item : texts)
        item.text->SetPosition(dstRect.x + item.localX, dstRect.y + item.localY);
}

void Container::SetVisible(bool v) {
    visible = v;
}

bool Container::IsVisible() const {
    return visible;
}

void Container::Toggle() {
    visible = !visible;
}