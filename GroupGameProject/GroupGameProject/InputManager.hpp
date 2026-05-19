#ifndef INPUT_MANAGER_HPP
#define INPUT_MANAGER_HPP

#include <SDL.h>
#include <unordered_map>
#include <vector>
#include <string>
#include "Vector2.hpp"
#include "Camera.hpp"

using namespace std;
class InputManager {
public:
    InputManager() = default;
    void Initialize(const string& filepath);
    void Process();
    void HandleEvent(const SDL_Event& event);

    bool IsKeyDown(const string& action);
    bool IsKeyPressed(const string& action);
    bool IsKeyReleased(const string& action);

    bool IsMouseButtonDown(Uint8 button);
    bool IsMouseButtonPressed(Uint8 button);
    bool IsMouseButtonReleased(Uint8 button);
    Vector2 GetMousePosition() const;
    Vector2 GetMouseWorldPosition(Camera* cam) const;
    bool IsMouseOver(int x, int y, int w, int h) const;
    void SetIsMouseOverUI(bool b);
    bool IsMouseOverUI() const;

    int GetScrollDelta() const;

private:
    int mouseX = 0;
    int mouseY = 0;
    int prevMouseX = 0;
    int prevMouseY = 0;
    int scrollDelta = 0;

    bool mouseOverUI;

    //mouse buttons (1=Left, 2=Middle, 3=Right)
    unordered_map<Uint8, bool> currentMouseButtons;
    unordered_map<Uint8, bool> previousMouseButtons;
    //key states
    unordered_map<SDL_Keycode, bool> currentKeys;
    unordered_map<SDL_Keycode, bool> previousKeys;

    //action bindings
    unordered_map<string, vector<SDL_Keycode>> actionBindings;

    bool CheckKeyState(const string& action, const unordered_map<SDL_Keycode, bool>& state);
};

#endif