#pragma once
#include "Event.hpp"


class TextCharEvent : public Event{
public:
    TextCharEvent(SDL_Event& event)
        : text(static_cast<const char*>(event.user.data1)){}
    TextCharEvent(const char* text)
        : text(text){}
        
    SDL_Event toSDLEvent(){
        SDL_Event sdlEvent;
        SDL_zero(sdlEvent);
        sdlEvent.type = getSDLEventType();
        char* heapText = new char[strlen(text) + 1];
        strcpy(heapText, text);
        sdlEvent.user.data1 = static_cast<void*>(heapText);
        return sdlEvent;
    }
    EVENT_CLASS_TYPE(TextChar);
    
public:
    const char* text;
};

