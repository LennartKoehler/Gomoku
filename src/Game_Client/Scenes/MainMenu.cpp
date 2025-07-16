#include "MainMenu.hpp"
#include "SceneManager.hpp"
#include "../Events/EventUtils.hpp"

extern const Uint32 NETWORK_CONNECTION_EVENT;

MainMenu::MainMenu(SceneManager* sceneManager) : Scene(sceneManager){
    mainMenuLayer = std::make_shared<MainMenuLayer>();
    chatLayer = std::make_shared<ChatLayer>();

    layerStack.addLayer(mainMenuLayer);
    layerStack.addLayer(chatLayer);

    mainMenuLayer->setGomokuButtonCallback([sceneManager, this]() {
        sceneManager->sendLayer(chatLayer, sceneManager->gomoku);
        sceneManager->setActiveScene(sceneManager->gomoku);
    });
    mainMenuLayer->setConnectServerButtonCallback([sceneManager, this](std::string address) {
        auto* addressCopy = new std::string(address);
        sendIPAddressEvent(addressCopy->c_str());
    });
}
void MainMenu::sendIPAddressEvent(const char* ipaddress){
    NetworkConnectionEvent event{ipaddress};
    SDL_Event sdlevent= event.toSDLEvent();
    SDL_PushEvent(&sdlevent);
}

// void MainMenu::sendIPAddressEvent(const char* ipaddress){
//     SDL_Event event;
//     SDL_zero(event);
//     event.type = NETWORK_CONNECTION_EVENT;
//     event.user.data1 = (void*)ipaddress;
//     SDL_PushEvent(&event);

// }

void MainMenu::update(){
    layerStack.updateLayers();
}