#pragma once
#include "Event.hpp"

#include <string>

class TextStringEvent : public Event{
public:
    TextStringEvent(SDL_Event& event)
        : text(event.text.text){}
    TextStringEvent(std::string text)
        : text(text){}
    SDL_Event toSDLEvent(){
        SDL_Event sdlEvent;
        SDL_zero(sdlEvent);
        sdlEvent.type = getSDLEventType();
        strncpy(sdlEvent.text.text, text.c_str(), sizeof(sdlEvent.text.text) - 1);
        sdlEvent.text.text[sizeof(sdlEvent.text.text) - 1] = '\0';
        return sdlEvent;
    }

    EVENT_CLASS_TYPE(TextString);
    
public:
    std::string text;
};

class TextStringNetworkEvent : public TextStringEvent{
public:
    TextStringNetworkEvent(SDL_Event& event) : TextStringEvent(event){}
    TextStringNetworkEvent(std::string text) : TextStringEvent(text){}
    EVENT_CLASS_TYPE(TextStringNetwork)
};