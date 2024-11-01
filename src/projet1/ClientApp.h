#pragma once

#include <windows.h>

class Window;
class UIManager;
class BitmapImage;

class ClientApp {
public:
    ClientApp();
    ~ClientApp();

    bool Initialize(HINSTANCE hInstance);

    inline Window* GetWindow() const { return m_pWindow; }
    inline UIManager* GetUIManager() const { return m_pUIManager; }

    bool isControlEventStarted = false;

    BitmapImage* m_bitmapImgLoader = nullptr;

private:
    Window* m_pWindow;
    UIManager* m_pUIManager;
};
