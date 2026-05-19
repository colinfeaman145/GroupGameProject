#ifndef FONT_MANAGER_HPP
#define FONT_MANAGER_HPP

#include <string>
#include <map>
#include <SDL_ttf.h>

using namespace std;
class FontManager {
public:
    FontManager();
    ~FontManager();

    TTF_Font* GetFont(const string& fontPath, int pointSize);
    void Clear();

private:
    map<string, TTF_Font*> fontMap;
    static string MakeKey(const string& fontPath, int pointSize);
};

#endif