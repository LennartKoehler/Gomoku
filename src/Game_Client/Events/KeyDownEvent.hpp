#pragma once
#include "Event.hpp"
#include <SDL2/SDL.h>

class KeyDownEvent : public Event{
public:
    KeyDownEvent(SDL_Event& event)
        : key(event.key.keysym.sym){}
    KeyDownEvent(SDL_Keycode key)
        : key(key){}

    SDL_Event toSDLEvent() override {
        SDL_Event sdlEvent;
        SDL_zero(sdlEvent);
        sdlEvent.type = getSDLEventType();
        sdlEvent.key.keysym.sym = key;
        return sdlEvent;
    }
    EVENT_CLASS_TYPE(KeyDown);
    
public:
    SDL_Keycode key;
};

