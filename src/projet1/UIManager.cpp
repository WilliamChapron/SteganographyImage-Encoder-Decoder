#include "UIManager.h"
#include "UIElement.h"
#include "Utils.h"

#include <iostream>
#include <string>

UIManager::UIManager(HINSTANCE hInstance) : m_hInstance(hInstance) {}

UIManager::~UIManager() {
    for (UIElement* element : m_pElements) {
        delete element; 
    }
}


UIButton* UIManager::CreateButton(HWND parentHwnd, int id, int x, int y, int width, int height, COLORREF color, const std::string& text) {
    UIButton* button = new UIButton(m_hInstance, id, parentHwnd, x, y, width, height, color, text);

    if (m_elementCount < MAX_ELEMENTS) {
        m_pElements[m_elementCount] = button;
        ++m_elementCount;
    }

    return button;
}


UITextField* UIManager::CreateTextField(HWND parentHwnd, int id, int x, int y, int width, int height, COLORREF color, const std::string& text, DWORD style, bool isTextFieldText) {
    UITextField* textField = new UITextField(m_hInstance, id, parentHwnd, x, y, width, height, color, text, style, isTextFieldText);

    if (m_elementCount < MAX_ELEMENTS) {
        m_pElements[m_elementCount] = textField;
        ++m_elementCount;
    }

    return textField;
}


UICheckBox* UIManager::CreateCheckBox(HWND parentHwnd, int id, int x, int y, int width, int height, COLORREF color, const std::string& text) {
    UICheckBox* checkBox = new UICheckBox(m_hInstance, id, parentHwnd, x, y, width, height, color, text);

    if (m_elementCount < MAX_ELEMENTS) {
        m_pElements[m_elementCount] = checkBox;
        ++m_elementCount;
    }

    return checkBox;
}


UIRadioButton* UIManager::CreateRadioButton(HWND parentHwnd, int id, int x, int y, int width, int height, COLORREF color, const std::string& text) {
    UIRadioButton* radioButton = new UIRadioButton(m_hInstance, id, parentHwnd, x, y, width, height, color, text);

    if (m_elementCount < MAX_ELEMENTS) {
        m_pElements[m_elementCount] = radioButton;
        ++m_elementCount;
    }

    return radioButton;
}

UILabel* UIManager::CreateLabel(HWND parentHwnd, int id, int x, int y, int width, int height, COLORREF color, const std::string& text, DWORD style) {
    UILabel* label = new UILabel(m_hInstance, id, parentHwnd, x, y, width, height, color, text, style);

    if (m_elementCount < MAX_ELEMENTS) {
        m_pElements[m_elementCount] = label;
        ++m_elementCount;
    }

    return label;
}

UIPanel* UIManager::CreatePanel(HWND parentHwnd, int id, int x, int y, int width, int height, COLORREF color, const std::string& text) {
    UIPanel* panel = new UIPanel(m_hInstance, id, parentHwnd, x, y, width, height, color, text);

    if (m_elementCount < MAX_ELEMENTS) {
        m_pElements[m_elementCount] = panel;
        ++m_elementCount;
    }

    return panel;
}

HWND UIManager::GetHWND(int id) const {
    for (UIElement* element : m_pElements) {
        if (element->GetID() == id) {
            return element->GetHWND();
        }
    }
    return NULL; 
}

void UIManager::CreateButtonWithTimer(HWND parentHwnd, int x, int y, int width, int height, const std::wstring& text, int durationMs) {
    if (m_elementCount >= MAX_ELEMENTS) {
        return;
    }

    int buttonId = 50 + m_timersCount;

    HWND buttonHwnd = CreateWindow(
        L"STATIC",  
        text.c_str(), 
        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  
        x, y, width, height,  
        parentHwnd,  
        (HMENU)buttonId,  
        (HINSTANCE)GetWindowLongPtr(parentHwnd, GWLP_HINSTANCE),
        NULL);

    ShowWindow(buttonHwnd, SW_SHOW);
    SetTimer(parentHwnd, buttonId, durationMs, NULL); 

    m_activeTimers[buttonId] = buttonHwnd;

    m_timersCount++;
}

void UIManager::HandleTimerDispawn(UINT timerId) {
    auto it = m_activeTimers.find(timerId);

    if (it != m_activeTimers.end()) {
        HWND buttonHwnd = it->second; 

        ShowWindow(buttonHwnd, SW_HIDE);
        KillTimer(GetParent(buttonHwnd), timerId);
    }
}


std::wstring UIManager::GetText(HWND parentHwnd, int controlId) {
    HWND controlHwnd = GetDlgItem(parentHwnd, controlId); // Get element handle

   
    if (controlHwnd) {
        int length = GetWindowTextLength(controlHwnd);
        if (length > 0) {

            // Buffer
            std::wstring text(length + 1, L'\0'); 
            GetWindowText(controlHwnd, &text[0], length + 1); 

            return text;
        }
    }

    return L"";
}



void UIManager::SetText(HWND parentHwnd, int elementId, const std::wstring& text) {
    HWND hwndElement = GetDlgItem(parentHwnd, elementId);
    if (hwndElement) {
        SetWindowText(hwndElement, text.c_str());

        
        for (int i = 0; i < m_elementCount; i++) {
            if (m_pElements[i] && m_pElements[i]->GetHWND() == hwndElement) {
                std::string str = WStringToString(text);
                m_pElements[i]->SetText(str); 
                break;
            }
        }
    }
}