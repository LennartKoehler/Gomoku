#pragma once
#include "Event.hpp"


class MouseButtonPressedEvent : public Event{
public:
    MouseButtonPressedEvent(SDL_Event& event)
        : x(event.button.x),
        y(event.button.y){}
    //MouseButtonPressedEvent(int x, int y) : x(x), y(y){} // should not be used because i dont creat these
    SDL_Event toSDLEvent() override {
        SDL_Event sdlEvent;
        SDL_zero(sdlEvent);
        sdlEvent.type = getSDLEventType();
        sdlEvent.button.x = *(new int(x)); // TODO is this heap?
        sdlEvent.button.y = *(new int(y));
        return sdlEvent;
    }
    EVENT_CLASS_TYPE(MouseButtonPressed);
    
    int x;
    int y;
};

