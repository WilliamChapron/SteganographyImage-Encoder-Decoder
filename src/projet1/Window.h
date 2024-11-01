#pragma once

#include <windows.h>
#include <string>

class ClientApp;

class Window {
public:
    Window(HINSTANCE hInstance, ClientApp* clientApp);
    ~Window();

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void Run();

    void Test() {
        std::cout << "Test" << std::endl;
    }

    inline HWND GetHWND() const { return m_hwnd; }

private:
    HWND m_hwnd;
    HINSTANCE m_hInstance;

    POINT m_lastClickedPoint;
};
