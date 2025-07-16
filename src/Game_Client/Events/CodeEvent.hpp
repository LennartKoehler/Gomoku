#pragma once
#include "Event.hpp"
#include <SDL2/SDL.h>

// enum class Code : uint8_t {
//     DEFAULT = 0,
//     AI_EVENT_RECIEVED = 1,

//     // add more as needed
// };


// class CodeEvent : public Event{ // generic event that uses the code of sdl event
// public:
//     CodeEvent(SDL_Event& event) : code(static_cast<Code>(event.user.code)){
//     }
//     EVENT_CLASS_TYPE(Code);
//     Code code;
// };