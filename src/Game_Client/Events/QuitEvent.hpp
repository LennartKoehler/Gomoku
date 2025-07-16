#pragma once
#include "Event.hpp"


class QuitEvent : public Event{
public:
    QuitEvent(SDL_Event& event)
        : quit(true){}
    QuitEvent()
        : quit(true){}
        
    SDL_Event toSDLEvent(){
        SDL_Event sdlEvent;
        SDL_zero(sdlEvent);
        sdlEvent.type = getSDLEventType();
        return sdlEvent;
    }
    EVENT_CLASS_TYPE(Quit);
    
public:
    bool quit;
};