#pragma once

#include "MainMenu.hpp"
#include "GomokuScene.hpp"
#include <memory>

// manages interactions between scenes
class SceneManager{
public:
    SceneManager(){
        gomoku = new GomokuScene(this);
        mainMenu = new MainMenu(this);
        activeScene = mainMenu;
    }
    Scene* getActiveScene(){ return activeScene; }
    void setActiveScene(Scene* activeScene) {
        this->activeScene = activeScene;
        activeScene->refresh();
        activeScene->update();
        activeScene->draw();
        }
    void sendLayer(std::shared_ptr<Layer> layer, Scene* scene){
        scene->addLayer(layer);
    }

    GomokuScene* gomoku;
    MainMenu* mainMenu;
private:
    Scene* activeScene;

};