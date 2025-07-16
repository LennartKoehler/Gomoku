#pragma once
#include "Scene.hpp"
#include "../Players/Player.h"
#include "../GameState.hpp"
#include "../Players/GameController.h"

class SceneManager;

class GomokuScene : public Scene{
public:
    GomokuScene(SceneManager* sceneManager);
    ~GomokuScene();

    void onPieceSelected(int i, int j);
    void update() override;
    void handleEvent(Event& event) override;

    std::shared_ptr<PieceLayer> pieceLayer;
    std::shared_ptr<BackgroundLayer> backgroundLayer;
private:
    std::vector<Player> players;
    GameController* gameController;
};