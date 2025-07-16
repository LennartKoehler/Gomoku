#pragma once

#include <memory>

#include "Scene.hpp"

class SceneManager;

class MainMenu : public Scene{
public:
    MainMenu(SceneManager* sceneManager);
    void update() override;
    void sendIPAddressEvent(const char* ipaddress);

private:
    std::shared_ptr<MainMenuLayer> mainMenuLayer;
    std::shared_ptr<ChatLayer> chatLayer;

};