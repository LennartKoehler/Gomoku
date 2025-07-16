#pragma once
#include "Event.hpp"

class NetworkConnectionEvent : public Event{
public:
    NetworkConnectionEvent(SDL_Event& event)
        : IPAddress(static_cast<const char*>(event.user.data1)){}
    NetworkConnectionEvent(const char* IPAddress)
        : IPAddress(IPAddress){} // this can be a string event with a code?

    SDL_Event toSDLEvent(){
        SDL_Event sdlEvent;
        SDL_zero(sdlEvent);
        sdlEvent.type = getSDLEventType();
        char* ipCopy = new char[strlen(IPAddress) + 1];
        strcpy(ipCopy, IPAddress);
        sdlEvent.user.data1 = ipCopy;
        return sdlEvent;
    }
    
    EVENT_CLASS_TYPE(NetworkConnection);
    
public:
    const char* IPAddress;
};

