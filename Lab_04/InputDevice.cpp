#include "InputDevice.h"
#include <iostream>
#include <vector>

void InputDevice::Initialize(HWND hwnd)
{
    RAWINPUTDEVICE rid{};

    rid.usUsagePage = 0x01;
    rid.usUsage     = 0x02;
    rid.dwFlags     = 0;
    rid.hwndTarget  = hwnd;

    RegisterRawInputDevices(&rid, 1, sizeof(rid));
}

void InputDevice::ProcessRawInput(LPARAM lParam)
{
    UINT size = 0;

    GetRawInputData((HRAWINPUT)lParam,
                    RID_INPUT,
                    nullptr,
                    &size,
                    sizeof(RAWINPUTHEADER));

    if (size == 0)
        return;

    std::vector<BYTE> buffer(size);

    if (GetRawInputData((HRAWINPUT)lParam,
                        RID_INPUT,
                        buffer.data(),
                        &size,
                        sizeof(RAWINPUTHEADER)) != size)
        return;

    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer.data());

    if (raw->header.dwType == RIM_TYPEMOUSE)
    {
        LONG dx = raw->data.mouse.lLastX;
        LONG dy = raw->data.mouse.lLastY;

        std::cout << "Mouse dx: " << dx
                  << " dy: " << dy << std::endl;
    }
}
