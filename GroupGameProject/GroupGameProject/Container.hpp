#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <vector>
#include "Renderer.hpp"
#include "Sprite.hpp"
#include "Vector2.hpp"

class Button;
class Text;

struct SpriteItem { Sprite* sprite; int localX, localY; };
struct ButtonItem { Button* button; int localX, localY; };
struct TextItem { Text* text; int localX, localY; };

//an html div that holds sprites
class Container : public Sprite {
public:
    Container(int x, int y, int w, int h, Color fillColor, Color borderColor, int alpha, int borderThickness);

    void AddSprite(Sprite* sprite, int localX, int localY);
    void AddButton(Button* button, int localX, int localY);
    void AddText(Text* text, int localX, int localY);

    virtual void Process(float deltaTime) override;
    virtual void Draw(Renderer* renderer) override;

    void SetPosition(int x, int y) override;
    void SetVisible(bool v);
    bool IsVisible() const;
    void Toggle();

protected:
    Color fillColor;
    Color borderColor;
    int alpha;
    int borderThickness;
    bool visible;

    Vector2 size;

    vector<SpriteItem> sprites;
    vector<ButtonItem> buttons;
    vector<TextItem>   texts;
};

#endif