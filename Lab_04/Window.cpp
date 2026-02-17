#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include "Window.h"

bool Window::Initialize(HINSTANCE instance, int showCmd)
{
    m_instance = instance;

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = instance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = L"MyWindowClass";

    if (!RegisterClassEx(&wc))
        return false;

    RECT rect = { 0,0,800,600 };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    m_hwnd = CreateWindowEx(
        0,
        L"MyWindowClass",
        L"Input Debug Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        instance,
        this
    );

    if (!m_hwnd)
        return false;

    ShowWindow(m_hwnd, showCmd);
    UpdateWindow(m_hwnd);

    return true;
}

int Window::Run()
{
    MSG msg = {};

    while (!m_exitRequested)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

    }

    return (int)msg.wParam;
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_CREATE)
    {
        CREATESTRUCT* create =
            reinterpret_cast<CREATESTRUCT*>(lParam);

        SetWindowLongPtr(hwnd, GWLP_USERDATA,
            reinterpret_cast<LONG_PTR>(create->lpCreateParams));
    }

    Window* window =
        reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

    if (window)
        return window->HandleMessage(hwnd, msg, wParam, lParam);

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT Window::HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_MOUSEMOVE:
    {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);

        std::cout << "Mouse Position: X = "
                  << x << " Y = "
                  << y << std::endl;

        return 0;
    }

    case WM_KEYDOWN:
    {
        std::cout << "Key Down: "
                  << wParam << std::endl;

        if (wParam == VK_ESCAPE)
        {
            m_exitRequested = true;
            PostQuitMessage(0);
        }

        return 0;
    }

    case WM_KEYUP:
    {
        std::cout << "Key Up: "
                  << wParam << std::endl;
        return 0;
    }

    case WM_CLOSE:
    case WM_DESTROY:
        m_exitRequested = true;
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}
