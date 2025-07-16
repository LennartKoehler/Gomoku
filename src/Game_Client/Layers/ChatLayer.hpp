#pragma once
#include "PieceLayer.hpp"
#include "Events/EventUtils.hpp"

using ButtonCallback = std::function<void()>;
using ButtonCallbackString = std::function<void(std::string)>;

class ChatLayer : public Layer{
public:
    ChatLayer();
    void onEvent(Event& event);
    bool onMouseButtonPressed(MouseButtonPressedEvent& event);
    bool onTextInput(TextStringEvent& event);
    bool onKeyDown(KeyDownEvent& event);
    bool onTextRecieved(TextStringNetworkEvent& event);
    void setConnectServerButtonCallback(ButtonCallbackString cb);
private:
    void setReturn();
    enum groupLabels : std::size_t{
        groupButtons,
        textFields
    };
    Entity* chatTextField;
    Entity* chatHistory;

};