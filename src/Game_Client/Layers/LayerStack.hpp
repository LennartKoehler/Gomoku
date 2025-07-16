#pragma once

#include "PieceLayer.hpp"
#include "BackgroundLayer.hpp"
#include "MainMenuLayer.hpp"
#include "ChatLayer.hpp"
#include <memory>

class LayerStack{
public:
    LayerStack(){}
    ~LayerStack();
    void addLayer(std::shared_ptr<Layer> layer);
    void addLayerTop(std::shared_ptr<Layer> layer);
    void updateLayers();
    void refreshLayers();
    void drawLayers();
    void handleEvent(Event& event);

private:
    std::vector<std::shared_ptr<Layer>> layers;
};