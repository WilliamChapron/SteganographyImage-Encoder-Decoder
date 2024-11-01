#include <iostream>
#include <fstream>
#include <windows.h>
#include <algorithm> 

#include "BitmapImage.h"
#include "ClientApp.h"
#include "Window.h"
#include "UIElement.h"
#include "UIManager.h"
#include "Steganography.h"
#include "Thread.h"

// UTILS
wchar_t* ConvertToWChar(const char* charStr) {
    if (charStr == nullptr) {
        return nullptr;
    }
    size_t charStrLen = strlen(charStr);
    size_t wideCharBufferSize = charStrLen + 1;
    wchar_t* wideCharStr = new wchar_t[wideCharBufferSize];
    size_t convertedChars = 0;
    errno_t err = mbstowcs_s(&convertedChars, wideCharStr, wideCharBufferSize, charStr, _TRUNCATE);
    if (err != 0) {
        delete[] wideCharStr;
        return nullptr;
    }
    return wideCharStr;
}

const char* ConvertToCChar(const std::wstring& wstr) {
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
    char* buffer = new char[size_needed + 1];
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), buffer, size_needed, NULL, NULL);
    buffer[size_needed] = '\0';
    return buffer;
}

std::wstring ConvertSlashesToDoubleBackslashes(const std::wstring& inputText) {
    std::string inputStr(inputText.begin(), inputText.end());
    std::string outputStr;

    for (char c : inputStr) {
        if (c == '/') {
            outputStr += '\\';
            outputStr += '\\';
        }
        else {
            outputStr += c;
        }
    }

    return std::wstring(outputStr.begin(), outputStr.end());
}
// UTILS


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    ClientApp* clientApp = new ClientApp();

    // Init Window / UIManager
    if (!clientApp->Initialize(hInstance)) {
        return 1;
    }

    // Init UI
    HWND hwndWindow = clientApp->GetWindow()->GetHWND();
    UIManager* uiManager = clientApp->GetUIManager();


    RECT windowRect;
    GetClientRect(hwndWindow, &windowRect);
    int windowWidth = windowRect.right;
    int windowHeight = windowRect.bottom;

    int halfWidth = windowWidth / 2;
    int panelHeight = windowHeight;

    COLORREF leftPanelColor = RGB(34, 34, 34); 
    COLORREF topRightPanelColor = RGB(45, 45, 48); 
    COLORREF bottomRightPanelColor = RGB(64, 64, 64);
    COLORREF buttonColor = RGB(220, 20, 60); 
    COLORREF textFieldColor = RGB(245, 245, 245); 
    COLORREF labelColor = RGB(255, 255, 255); 
    COLORREF labelHighlightColor = RGB(255, 69, 0); 


    // Left
    UIPanel* leftPanel = uiManager->CreatePanel(hwndWindow, 1, 50, 90, halfWidth - 100, panelHeight - 200, leftPanelColor, "");
    UITextField* leftLabel = uiManager->CreateTextField(hwndWindow, 2, windowWidth / 6 - 100, windowHeight - 100, 300, 60, RGB(255,255,255), "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL | ES_AUTOVSCROLL, true);

    // Top right
    UIPanel* topRightPanel = uiManager->CreatePanel(hwndWindow, 3, halfWidth + 20, 50, halfWidth, panelHeight / 2 - 100, topRightPanelColor, "");
    UILabel* topRightTitle = uiManager->CreateLabel(hwndWindow, 4, halfWidth + 300, 50, 200, 30, RGB(0, 0, 0), "DECODE IMAGE");
    UILabel* topRightInputFieldLabel = uiManager->CreateLabel(hwndWindow, 5, halfWidth + 300, 120, 200, 30, topRightPanelColor, "File path : ");
    UITextField* topRightInputField = uiManager->CreateTextField(hwndWindow, 6, halfWidth + 250, 150, 300, 30, textFieldColor, "");
    UIButton* buttonLoadFile = uiManager->CreateButton(hwndWindow, 7, halfWidth + 300, 200, 200, 60, buttonColor, "Load the file");

    // Bottom right
    UIPanel* Panel = uiManager->CreatePanel(hwndWindow, 8, halfWidth + 20, panelHeight / 2, halfWidth, panelHeight / 2 - 100, bottomRightPanelColor, "");
    UILabel* Title = uiManager->CreateLabel(hwndWindow, 9, halfWidth + 300, panelHeight / 2, 200, 30, RGB(0, 0, 0), "ENCODE IMAGE");
    UILabel* InputFieldLabel = uiManager->CreateLabel(hwndWindow, 10, halfWidth + 130, 70 + panelHeight / 2, 200, 30, bottomRightPanelColor, "Text To Encode :");
    UITextField* InputField = uiManager->CreateTextField(hwndWindow, 11, halfWidth + 180, 100 + panelHeight / 2, 200, 30, textFieldColor, "");
    UILabel* inputFieldLabelOutputFile = uiManager->CreateLabel(hwndWindow, 12, halfWidth + 340, 70 + panelHeight / 2, 200, 30, bottomRightPanelColor, "Output file : ");
    UITextField* inputFieldOutputFile = uiManager->CreateTextField(hwndWindow, 13, halfWidth + 400, 100 + panelHeight / 2, 200, 30, textFieldColor, "");
    UIButton* bottomRightButtonEncodeFile = uiManager->CreateButton(hwndWindow, 14, halfWidth + 180, panelHeight / 2 + 160, 200, 60, buttonColor, "1. Encode file");
    UIButton* bottomRightButtonSaveFile = uiManager->CreateButton(hwndWindow, 15, halfWidth + 400, panelHeight / 2 + 160, 200, 60, buttonColor, "2. Save file");

    // Apply Logic Interactions To button
    {
        // Load File Btn
        buttonLoadFile->SetOnClickCallback([clientApp, hwndWindow, halfWidth]() {
            std::wstring inputText = clientApp->GetUIManager()->GetText(hwndWindow, 6);

            if (!inputText.empty()) {
                inputText = ConvertSlashesToDoubleBackslashes(inputText);
            }

            clientApp->m_bitmapImgLoader = new BitmapImage();
            

            const char* loadCharBuffer = ConvertToCChar(inputText);
            if (clientApp->m_bitmapImgLoader->loadFile(loadCharBuffer)) {
                InvalidateRect(hwndWindow, NULL, TRUE); 
                const char* loadScndCharBuffer = Steganography::decode(clientApp->m_bitmapImgLoader);
                if (loadScndCharBuffer != NULL)
                {
                    wchar_t* wLoadCharBuffer = ConvertToWChar(loadScndCharBuffer);
                    clientApp->GetUIManager()->SetText(hwndWindow, 2, wLoadCharBuffer);
                    delete[] wLoadCharBuffer;
                }
                else
                    clientApp->GetUIManager()->SetText(hwndWindow, 2, L"");
                clientApp->GetUIManager()->CreateButtonWithTimer(hwndWindow, halfWidth + 330, 250, 200, 30, L"Image loaded Successfully");
            }           
            else {
                clientApp->GetUIManager()->CreateButtonWithTimer(hwndWindow, halfWidth + 330, 250, 200, 30, L"Error : Image not found ");
            }
            delete[] loadCharBuffer;
            
            // Show encoded message if available
        });

        // Encode img Btn
        bottomRightButtonEncodeFile->SetOnClickCallback([clientApp, hwndWindow, halfWidth, panelHeight]() {
            std::wstring inputText = clientApp->GetUIManager()->GetText(hwndWindow, 11);
            if (!inputText.empty()) {
                inputText = ConvertSlashesToDoubleBackslashes(inputText);
            }
            
            if (clientApp->m_bitmapImgLoader) {
                const char* charBuffer = ConvertToCChar(inputText);
                InvalidateRect(hwndWindow, NULL, TRUE); // Demande un redessin
                Steganography::encode(clientApp->m_bitmapImgLoader, charBuffer);
                const char* scndCharBuffer = Steganography::decode(clientApp->m_bitmapImgLoader);
                if (scndCharBuffer != NULL)
                {
                    wchar_t* wCharBuffer = ConvertToWChar(scndCharBuffer);
                    clientApp->GetUIManager()->SetText(hwndWindow, 2, wCharBuffer);
                    clientApp->GetUIManager()->CreateButtonWithTimer(hwndWindow, halfWidth + 180, 240 + panelHeight / 2 - 20, 200, 30, L"Image Encoded Successfully");
                    delete[] wCharBuffer;
                }
                else
                    clientApp->GetUIManager()->SetText(hwndWindow, 2, L"Sentence too long to be encoded");
                delete[] charBuffer;
            }
            else 
                MessageBoxA(hwndWindow, "No loaded bmp", "ERROR", MB_OK | MB_ICONINFORMATION);     
            });

        // Save File btn
        bottomRightButtonSaveFile->SetOnClickCallback([clientApp, hwndWindow, halfWidth, panelHeight]() {
            std::wstring inputText = clientApp->GetUIManager()->GetText(hwndWindow, 13);
            if (!inputText.empty()) {
                inputText = ConvertSlashesToDoubleBackslashes(inputText);
            }
            if (clientApp->m_bitmapImgLoader->saveFile(ConvertToCChar(inputText)))
            {
                clientApp->GetUIManager()->CreateButtonWithTimer(hwndWindow, halfWidth + 400, 240 + panelHeight / 2 - 20, 200, 30, L"Image Saved Successfully");
            }
            else
                MessageBoxA(hwndWindow, "Cannot write file on gived path", "ERROR", MB_OK | MB_ICONINFORMATION);
            });  
    }


    // Set Control WM_COMMAND active
    clientApp->isControlEventStarted = true;

    // Run on Principal Thread
    clientApp->GetWindow()->Run();

    return 0;
}