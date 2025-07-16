#pragma once


#include <SDL2/SDL.h>


#include <SDL2/SDL.h>
#include <functional>
#include <typeindex>
#include <unordered_map>
#include <cassert>

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::type; }\
								virtual EventType getEventType() const override { return getStaticType(); }\
								virtual const char* getName() const override { return #type; }
#define HZ_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }



enum class EventType : Uint32
{
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    AppTick, AppUpdate, AppRender,
    KeyDown, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
    TextString, Quit,
    GameStateEventClass, GameStateRecieved, GameStatePlayer, SendGameStateToServer,
    TextChar, NetworkConnection, Code, TextStringNetwork
};

namespace EventUtils {
    static Uint32 getSDLEventType(EventType type);
}

class Event{
public:
    bool handled = false;
    virtual SDL_Event toSDLEvent() = 0;
    virtual EventType getEventType() const = 0;
    virtual const char* getName() const = 0;
    // Uint32 getSDLEventType() { return 0; }
    Uint32 getSDLEventType() { return EventUtils::getSDLEventType(getEventType());}
};


class EventDispatcher{
public:
    EventDispatcher (Event& event) : m_Event(event){}
    
    template<typename T, typename F>
    bool dispatch(const F& function){
        if (m_Event.getEventType() == T::getStaticType()){
            m_Event.handled |= function(static_cast<T&>(m_Event));
            return true;
        }
        return false;
    }

private:
    Event& m_Event;
};




                                
// #define EVENT_CLASS_TYPE(T) \
//     virtual EventType getEventType() const override {\
//         auto it = EventAPI::eventMap.find(CustomEventType::T);\
//         assert(it != EventAPI::eventMap.end() && "Event type not registered!"); \
//         return it->second; \
//     } \
//     virtual const char* getName() const override { return #T; }








