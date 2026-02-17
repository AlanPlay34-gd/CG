#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include "Window.h"

void CreateConsole()
{
    AllocConsole();

    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
    freopen_s(&f, "CONIN$", "r", stdin);

    std::cout.clear();
}

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE,
                   PSTR,
                   int nCmdShow)
{
    CreateConsole();

    Window window;

    if (!window.Initialize(hInstance, nCmdShow))
        return 0;

    return window.Run();
}
