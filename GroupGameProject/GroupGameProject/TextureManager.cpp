#include <SDL_image.h>
#include <iostream>
#include "Renderer.hpp"
#include "TextureManager.hpp"

using namespace std;
TextureManager::TextureManager() {}

TextureManager::~TextureManager() {
    Clear();
}

//loads texture if not already cached
SDL_Texture* TextureManager::LoadTexture(Renderer* renderer, const std::string& filename) {
    //check if exists
    auto tx = textureMap.find(filename);
    if (tx != textureMap.end()) {
        return tx->second;
    }

    //if not, load
    SDL_Texture* tex = IMG_LoadTexture(renderer->GetSDLRenderer(), filename.c_str());
    if (!tex) {
        cerr << "Failed to load texture: " << filename << " SDL_image error: " << IMG_GetError() << endl;
        return nullptr;
    }

    textureMap[filename] = tex;
    return tex;
}

void TextureManager::AddTexture(const string& key, SDL_Texture* texture) {
    if (textureMap.find(key) == textureMap.end()) {
        textureMap[key] = texture;
    }
}

void TextureManager::Clear() {
    for (auto& pair : textureMap) {
        if (pair.second) {
            SDL_DestroyTexture(pair.second);
        }
    }
    textureMap.clear();
}