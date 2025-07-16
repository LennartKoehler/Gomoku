#pragma once
#include "MouseButtonPressedEvent.hpp"
#include "GameStateEvent.hpp"
#include "NetworkConnectionEvent.hpp"
#include "TextStringEvent.hpp"
#include "KeyDownEvent.hpp"
#include "TextCharEvent.hpp"
#include "CodeEvent.hpp"
#include "QuitEvent.hpp"



// I tried to change the eventsystem to more tightly couple it with the sdl eventsystem
// however i kept coming back to this solution to keep it static etc.
// probably not worth looking into changing this

using SDLEventType = Uint32;
using EventConstructor = std::function<std::unique_ptr<Event>(SDL_Event&)>;
namespace EventUtils {
    extern std::unordered_map<EventType, EventConstructor> eventConstructors;
    extern std::unordered_map<EventType, SDLEventType> toSDLType;
    extern std::unordered_map<SDLEventType, EventType> fromSDLType;

    template<typename className>
    void addMappedEvent(EventType eventtype, SDLEventType SDL_eventtype){
        eventConstructors[eventtype] = [](SDL_Event& e) { return std::make_unique<className>(e); };
        toSDLType[eventtype] = SDL_eventtype;
        fromSDLType[SDL_eventtype] = eventtype;
    }

    template<typename className>
    void addCustomEvent(EventType eventtype){
        SDLEventType SDLeventtype = SDL_RegisterEvents(1);
        toSDLType[eventtype] = SDLeventtype;
        fromSDLType[SDLeventtype] = eventtype;
        eventConstructors[eventtype] = [](SDL_Event& e) {
            return std::make_unique<className>(e);
        };
    }

    static void registerEventConstructors() {
        addCustomEvent<GameStatePlayerEvent>(EventType::GameStatePlayer);
        addCustomEvent<GameStatePlayerEvent>(EventType::GameStateEventClass);
        
        addCustomEvent<NetworkConnectionEvent>(EventType::NetworkConnection);
        addCustomEvent<SendGameStateToServerEvent>(EventType::SendGameStateToServer);
        addCustomEvent<TextStringEvent>(EventType::TextString);
        addCustomEvent<TextStringNetworkEvent>(EventType::TextStringNetwork);

        addMappedEvent<MouseButtonPressedEvent>(EventType::MouseButtonPressed, SDL_MOUSEBUTTONDOWN);
        addMappedEvent<KeyDownEvent>(EventType::KeyDown, SDL_KEYDOWN);
        addMappedEvent<TextStringEvent>(EventType::TextString, SDL_TEXTINPUT);
        addMappedEvent<QuitEvent>(EventType::Quit, SDL_QUIT);
        // addCustomEvent<TextEvent>(EventType::TextString);
        // addCustomEvent<CodeEvent>(EventType::Code);
    }
    static SDLEventType getSDLEventType(EventType eventtype){
        auto it = toSDLType.find(eventtype);
        assert(it != toSDLType.end() && ("eventtype not registered " + std::to_string(static_cast<int>(eventtype))).c_str());

        return it->second;
    }

    static EventType getCustomEventType(SDLEventType eventtype){
        auto it = fromSDLType.find(eventtype);
        assert(it != fromSDLType.end() && ("SDL EventType not mapped to EventType: " + std::to_string(eventtype)).c_str());

        return it->second;
    }

    static std::unique_ptr<Event> convertEventFromSDL(SDL_Event& sdlEvent) {
        auto it_1 = fromSDLType.find(sdlEvent.type);
        assert(it_1 != fromSDLType.end() && ("SDL EventType not mapped to EventType: " + std::to_string(sdlEvent.type)).c_str());

        auto it = eventConstructors.find(it_1->second);
        assert(it != eventConstructors.end() && ("EventType has no registered constructor: " + std::to_string(sdlEvent.type)).c_str());
        return it->second(sdlEvent);
    }

}