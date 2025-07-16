#include "LayerStack.hpp"
#include <iostream>

LayerStack::~LayerStack(){
    layers.clear();
}

void LayerStack::handleEvent(Event& event){
    EventDispatcher dispatcher(event);
    for (std::shared_ptr<Layer> layer : layers){
        if(event.handled){
            break;
        }
        layer->onEvent(event);
    }
}

void LayerStack::addLayer(std::shared_ptr<Layer> layer){
    layers.push_back(layer);
}

void LayerStack::addLayerTop(std::shared_ptr<Layer> layer){
    layers.insert(layers.begin(), layer);
}

void LayerStack::updateLayers(){
    for (std::shared_ptr<Layer> layer : layers){
        layer->update();
    }
}

void LayerStack::refreshLayers(){
    for (std::shared_ptr<Layer> layer : layers){
        layer->refresh();
    }
}

void LayerStack::drawLayers(){
    for (std::shared_ptr<Layer> layer : layers){
        layer->draw();
    }
}