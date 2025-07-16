#include "EventUtils.hpp"

namespace EventUtils {
    std::unordered_map<EventType, EventConstructor> eventConstructors;
    std::unordered_map<EventType, SDLEventType> toSDLType;
    std::unordered_map<SDLEventType, EventType> fromSDLType;
}