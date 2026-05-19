#include "Text.hpp"
#include <iostream>
using namespace std;

Text::Text(){
    pointSize = 0;
    textColor = { 255, 255, 255, 255 };
}

Text::~Text() {
    texture = nullptr;
}

bool Text::Initialize(const string& inputText, const string& inputFontPath, int inputPointSize) {
    text = inputText;
    fontPath = inputFontPath;
    pointSize = inputPointSize;
    fontStyle = TTF_STYLE_NORMAL;

    SetDrawLayer(RenderLayer::UI);

    return BuildTexture();
}

bool Text::BuildTexture() {
    TTF_Font* font = context.fm->GetFont(fontPath, pointSize);
    if (!font) return false;

    TTF_SetFontStyle(font, fontStyle);
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    TTF_SetFontStyle(font, TTF_STYLE_NORMAL); //reset

    if (!surface) {
        cerr << "Text: TTF_RenderText_Blended failed: " << TTF_GetError() << endl;
        return false;
    }

    textureKey = text + "|" + fontPath + "|" + to_string(pointSize);

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(context.renderer->GetSDLRenderer(), surface);
    SDL_FreeSurface(surface);

    if (!newTexture) {
        cerr << "Text: SDL_CreateTextureFromSurface failed: " << SDL_GetError() << endl;
        return false;
    }

    context.txm->AddTexture(textureKey, newTexture);

    int w, h;
    SDL_QueryTexture(newTexture, nullptr, nullptr, &w, &h);
    Sprite::Initialize(newTexture, w, h, 0, 0, w, h);
    SetDrawLayer(RenderLayer::UI, 3);

    return true;
}

void Text::SetText(const string& newText) {
    if (newText == text) return;
    text = newText;
    BuildTexture();
}

void Text::SetFontSize(int size) {
    pointSize = size;
}

int Text::GetFontSize() {
    return pointSize;
}

/* OPTIONS
* TTF_STYLE_BOLD
* TTF_STYLE_NORMAL
* TTF_STYLE_UNDERLINE
*/
void Text::SetFontStyle(int style) {
    fontStyle = style;
    BuildTexture();
}