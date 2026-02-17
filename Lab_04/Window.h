#pragma once
#include <windows.h>

class Window
{
public:
    bool Initialize(HINSTANCE instance, int showCmd);
    int Run();

private:
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

private:
    HWND m_hwnd = nullptr;
    HINSTANCE m_instance = nullptr;
    bool m_exitRequested = false;
};
