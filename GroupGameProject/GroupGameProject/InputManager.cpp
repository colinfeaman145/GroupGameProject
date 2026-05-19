#include "InputManager.hpp"
#include <fstream>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

//NOTE: input file located in data/input.json

void InputManager::Initialize(const string& filepath) {
    ifstream file(filepath);
    if (!file.is_open()) {
        cerr << "Failed to open bindings file: " << filepath << endl;
        return;
    }

    json data = json::parse(file);
    for (auto& [action, keys] : data.items()) { //for each action/binding pair
        for (const string& keyName : keys) { //iterate through each binding
            SDL_Keycode code = SDL_GetKeyFromName(keyName.c_str());
            if (code != SDLK_UNKNOWN)
                actionBindings[action].push_back(code); //add binding if valid keycode
            else
                cerr << "Unknown key name: " << keyName << endl;
        }
    }
}

void InputManager::Process() {
    previousKeys = currentKeys;
    previousMouseButtons = currentMouseButtons;
    prevMouseX = mouseX;
    prevMouseY = mouseY;
    scrollDelta = 0;
}

void InputManager::HandleEvent(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN)
        currentKeys[event.key.keysym.sym] = true;
    else if (event.type == SDL_KEYUP)
        currentKeys[event.key.keysym.sym] = false;
    else if (event.type == SDL_MOUSEBUTTONDOWN)
        currentMouseButtons[event.button.button] = true;
    else if (event.type == SDL_MOUSEBUTTONUP)
        currentMouseButtons[event.button.button] = false;
    else if (event.type == SDL_MOUSEMOTION) {
        mouseX = event.motion.x;
        mouseY = event.motion.y;
    }
    else if (event.type == SDL_MOUSEWHEEL) {
        scrollDelta = event.wheel.y; //up = positive
    }
}

//checks action's binding state in state(current/previous)
bool InputManager::CheckKeyState(const string& action, const unordered_map<SDL_Keycode, bool>& state) {
    auto it = actionBindings.find(action);
    if (it == actionBindings.end()) {
        cerr << "No key bound to action: " << action << endl;
        return false;
    }
    for (SDL_Keycode code : it->second) {
        auto keyIt = state.find(code);
        if (keyIt != state.end() && keyIt->second) return true;
    }
    return false;
}

bool InputManager::IsKeyDown(const string& action) {
    return CheckKeyState(action, currentKeys);
}

bool InputManager::IsKeyPressed(const string& action) {
    return CheckKeyState(action, currentKeys) && !CheckKeyState(action, previousKeys);
}

bool InputManager::IsKeyReleased(const string& action) {
    return !CheckKeyState(action, currentKeys) && CheckKeyState(action, previousKeys);
}

bool InputManager::IsMouseButtonDown(Uint8 button) {
    return currentMouseButtons.count(button) && currentMouseButtons.at(button);
}

bool InputManager::IsMouseButtonPressed(Uint8 button) {
    return IsMouseButtonDown(button) &&
        !(previousMouseButtons.count(button) && previousMouseButtons.at(button));
}

bool InputManager::IsMouseButtonReleased(Uint8 button) {
    return !IsMouseButtonDown(button) &&
        (previousMouseButtons.count(button) && previousMouseButtons.at(button));
}

Vector2 InputManager::GetMousePosition() const {
    return Vector2(mouseX, mouseY);
}

Vector2 InputManager::GetMouseWorldPosition(Camera* cam) const {
    return Vector2(
        cam->GetX() + mouseX / cam->GetZoom(),
        cam->GetY() + mouseY / cam->GetZoom()
    );
}

bool InputManager::IsMouseOver(int x, int y, int w, int h) const {
    return mouseX >= x && mouseX <= x + w &&
        mouseY >= y && mouseY <= y + h;
}

void InputManager::SetIsMouseOverUI(bool b) {
    mouseOverUI = b;
}

bool InputManager::IsMouseOverUI() const {
    return mouseOverUI;
}

int InputManager::GetScrollDelta() const {
    return scrollDelta;
}