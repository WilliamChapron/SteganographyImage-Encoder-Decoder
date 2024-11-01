#include <iostream>

#include "ClientApp.h"
#include "Window.h"
#include "UIManager.h"

ClientApp::ClientApp() {
}


ClientApp::~ClientApp() {
}

bool ClientApp::Initialize(HINSTANCE hInstance) {

    AllocConsole();

    FILE* stream;
    if (freopen_s(&stream, "CONOUT$", "w", stdout) != 0) {
        std::cerr << "Erreur : Impossible d'allouer la sortie standard." << std::endl;
        return false;
    }

    if (freopen_s(&stream, "CONOUT$", "w", stderr) != 0) {
        std::cerr << "Erreur : Impossible d'allouer la sortie d'erreur." << std::endl;
        return false;
    }

    m_pWindow = new Window(hInstance, this);
    m_pUIManager = new UIManager(hInstance);

    return true;
}