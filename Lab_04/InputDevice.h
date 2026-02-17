#pragma once
#include <windows.h>

class InputDevice
{
public:
    void Initialize(HWND hwnd);
    void ProcessRawInput(LPARAM lParam);

private:
    void HandleKeyboard(const RAWINPUT* raw);
    void HandleMouse(const RAWINPUT* raw);
};
