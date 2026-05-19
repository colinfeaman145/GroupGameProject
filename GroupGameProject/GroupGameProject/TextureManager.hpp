#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <SDL.h>
#include <string>
#include <unordered_map>

class Renderer;

using namespace std;
class TextureManager {
public:
    TextureManager();
    ~TextureManager();

    SDL_Texture* LoadTexture(Renderer* renderer, const string& filename);
    void AddTexture(const string& key, SDL_Texture* texture);

    void Clear();

private:
    unordered_map<string, SDL_Texture*> textureMap;
};

#endif