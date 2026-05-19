#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>
#include "Sprite.hpp"
#include "FontManager.hpp"
#include "TextureManager.hpp"
#include "Renderer.hpp"
#include "Element.hpp"
#include "GameContext.hpp"

using namespace std;
class Text : public Sprite, public Element {
public:
    Text();
    ~Text();
    bool Initialize(const string& text, const string& fontPath, int pointSize);
    void SetText(const string& text);
    void SetFontSize(int size);
    void SetFontStyle(int style);
    int GetFontSize();
    void Draw(Renderer* renderer) override { Sprite::Draw(renderer); }
    void Process(float deltaTime) override { Sprite::Process(deltaTime); }

private:
    bool BuildTexture();

    string text;
    string fontPath;
    int pointSize;
    SDL_Color textColor;
    string textureKey;
    int fontStyle;
};

#endif