#include "FontManager.hpp"
#include <iostream>

FontManager::FontManager() {
    if (TTF_Init() == -1) {
        cerr << "FontManager: TTF_Init failed: " << TTF_GetError() << endl;
    }
}

FontManager::~FontManager() {
    Clear();
    TTF_Quit();
}

TTF_Font* FontManager::GetFont(const string& fontPath, int pointSize) {
    string key = MakeKey(fontPath, pointSize);

    //find if it exists
    auto it = fontMap.find(key);
    if (it != fontMap.end()) {
        return it->second;
    }

    //if doesnt exist, make it
    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), pointSize);
    if (!font) {
        cerr << "FontManager: failed to load font: " << fontPath
            << " at size " << pointSize << " — " << TTF_GetError() << endl;
        return nullptr;
    }

    fontMap[key] = font;
    return font;
}

void FontManager::Clear() {
    for (auto& pair : fontMap) {
        if (pair.second) {
            TTF_CloseFont(pair.second);
        }
    }
    fontMap.clear();
}

string FontManager::MakeKey(const string& fontPath, int pointSize) {
    return fontPath + "|" + to_string(pointSize);
}