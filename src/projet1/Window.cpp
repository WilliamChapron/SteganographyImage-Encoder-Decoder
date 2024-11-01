#include <iostream>
#include <stdexcept>
#include <string>

#include "Window.h"
#include "ClientApp.h"
#include "UIManager.h"
#include "UIElement.h"
#include "BitmapImage.h"
#include "Thread.h"

// Global Thread For click Event, but Event timer wait this thread finish to show timer
HANDLE threadClickHandle;

Window::Window(HINSTANCE hInstance, ClientApp* clientApp) : m_hInstance(hInstance) {
    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    m_hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Steganography",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        clientApp
    );

    ShowWindow(m_hwnd, SW_SHOWDEFAULT);
}

Window::~Window() {
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    static bool onePaint = false;

    switch (uMsg) {
    case WM_CREATE: {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        if (pCreate) {
            ClientApp* clientApp = (ClientApp*)pCreate->lpCreateParams;
            SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)clientApp);
        }
        break;
    }
    case WM_COMMAND: {

        if (HIWORD(wParam) == BN_CLICKED) {

            HWND buttonHwnd = (HWND)lParam;

            UIButton* element = (UIButton*)GetWindowLongPtr(buttonHwnd, GWLP_USERDATA);

            if (element) {

                InvalidateRect(hwnd, NULL, TRUE);


                // Alloc thread for Click (Decode or Encode Step) 
                // Asynchronou, concurrent execution work but not with the timer element

                //Thread thread;
                //threadClickHandle = thread.Init(element, &UIElement::PerformClick);

                element->PerformClick();

            }
        }
        //if (HIWORD(wParam) == EN_CHANGE) {
        //}
        //if (HIWORD(wParam) == EN_SETFOCUS) {
        //}
        
        break;
    }

    case WM_LBUTTONDOWN: { 
        HWND hwndEdit = GetDlgItem(hwnd, 2); 
        if (hwndEdit && (HWND)lParam == hwndEdit) {
            MessageBox(hwnd, L"Clic sur le champ de texte!", L"Information", MB_OK);
        }
        break;
    }

    case WM_TIMER: {


        ClientApp* clientApp = (ClientApp*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (!clientApp) {
            break;
        }

        int timerId = static_cast<int>(wParam);

        clientApp->GetUIManager()->HandleTimerDispawn(timerId);

        // Stop while not finish #NOT_WORK
        
        //DWORD result = WaitForSingleObject(threadClickHandle, INFINITE); 

        //if (result == WAIT_OBJECT_0) {
        //    ClientApp* clientApp = (ClientApp*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        //    if (!clientApp) {
        //        break;
        //    }

        //    int timerId = static_cast<int>(wParam);
        //    clientApp->GetUIManager()->HandleTimerDispawn(timerId);
        //} 

        break;
    }

    // Stylize All Element Correspond to our style param send
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Fill Window
        RECT clientRect;
        GetClientRect(hwnd, &clientRect); 
        HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0)); 
        FillRect(hdc, &clientRect, hBrush); 
        DeleteObject(hBrush); 


        // Check & Init part
        ClientApp* clientApp = (ClientApp*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        if (!clientApp) {
            EndPaint(hwnd, &ps);
            break;
        }
        if (!clientApp->isControlEventStarted) {
            EndPaint(hwnd, &ps);
            break;
        }
        UIManager* uiManager = clientApp->GetUIManager();
        if (!uiManager) {
            EndPaint(hwnd, &ps);
            break;
        }

        UIElement** elements = uiManager->GetAllElements();
        int elementCount = uiManager->GetElementsCount();

        // Draw Controls
        for (int i = 0; i < elementCount; ++i) {
            UIElement* element = elements[i];
            HWND elementHwnd = element->GetHWND();

            PAINTSTRUCT psElement;
            HDC hdcElement = BeginPaint(elementHwnd, &psElement);

            element->PerformPaint(hdcElement);

            EndPaint(elementHwnd, &psElement);
        }

        BitmapImage* img = clientApp->m_bitmapImgLoader;

        if (clientApp->m_bitmapImgLoader != nullptr) {
            HBITMAP hBitmap = nullptr;
            HDC imgDeviceContext = nullptr;
            HDC mainDeviceContext = hdc;

            hBitmap = CreateDIBitmap(
                mainDeviceContext,
                img->m_infoHeader,
                CBM_INIT,
                img->m_pixels,
                img->m_info,
                DIB_RGB_COLORS
            );

            if (!hBitmap) {
                return 0;
            } 

            imgDeviceContext = CreateCompatibleDC(mainDeviceContext);
            if (!imgDeviceContext) {
                DeleteObject(hBitmap);
                return 0;
            }

            HBITMAP oldBitmap = (HBITMAP)SelectObject(imgDeviceContext, hBitmap);

            BOOL result = StretchBlt(
                mainDeviceContext,
                100,
                100,
                500,
                500,
                imgDeviceContext,
                0,
                0,
                img->m_infoHeader->biWidth,
                img->m_infoHeader->biHeight,
                SRCCOPY
            );

            SelectObject(imgDeviceContext, oldBitmap);
            DeleteDC(imgDeviceContext);
        }


        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void Window::Run() {
    MSG msg = {};
    while (GetMessage(&msg, m_hwnd, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}