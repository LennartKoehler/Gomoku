#pragma once

#include "../ECS/Components.hpp"
#include "../VectorMatrix.hpp"
#include "../Events/MouseButtonPressedEvent.hpp"

//TODO layer system is bad, used incorrectly
class Layer{ 
public:
    virtual void draw(){
        manager.draw();
    }

    virtual void update(){
        manager.update();
    }

    virtual void refresh(){
        manager.refresh();
    }

    virtual void onEvent(Event& event) {}

    
    Entity* getEntityAtPosition(int x, int y, Group group);
    bool entityOverlap(Entity* entity, int x, int y);
protected:
    Manager manager;
};
