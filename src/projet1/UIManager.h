#pragma once

#include <windows.h>
#include <vector>
#include <string>
#include <map>

class UIElement;

class UIButton;
class UITextField;
class UICheckBox;
class UIRadioButton;
class UILabel;
class UIPanel;

const int MAX_ELEMENTS = 100; 

class UIManager {
public:
    UIManager(HINSTANCE hInstance);
    ~UIManager();

    UIButton* CreateButton(HWND parentHwnd, int id, int x, int y, int width, int height, COLORREF color, const std::string& text);
    UITextField* CreateTextField(HWND parentHwnd, int id, int x, int y, int width, int height, COLORREF color, const std::string& text, DWORD style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL, bool isTextFieldText = false);
    UICheckBox* CreateCheckBox(HWND parentHwnd, int id, int x, int y, int width, int height, COLORREF color, const std::string& text);
    UIRadioButton* CreateRadioButton(HWND parentHwnd, int id, int x, int y, int width, int height, COLORREF color, const std::string& text);
    UILabel* CreateLabel(HWND parentHwnd, int id, int x, int y, int width, int height, COLORREF color, const std::string& text, DWORD style = WS_VISIBLE | WS_CHILD);
    UIPanel* CreatePanel(HWND parentHwnd, int id, int x, int y, int width, int height, COLORREF color, const std::string& text);


    // Getter
    HWND GetHWND(int id) const; 
    inline UIElement** GetAllElements() {return m_pElements; }
    inline int GetElementsCount() { return m_elementCount;  }


    // Timers elements system 
    void CreateButtonWithTimer(HWND parentHwnd, int x, int y, int width, int height, const std::wstring& text = L"Click Me", int durationMs = 2000);
    void HandleTimerDispawn(UINT timerId);

    // Work with elements
    std::wstring GetText(HWND parentHwnd, int controlId);
    void SetText(HWND parentHwnd, int elementId, const std::wstring& text);
private:

    HINSTANCE m_hInstance;
    UIElement* m_pElements[MAX_ELEMENTS];
    int m_elementCount;

    // Active Element Timers
    std::map<int, HWND> m_activeTimers;  // Map ID -> HWND of elements with timers
    int m_timersCount;
};
