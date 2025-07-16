#include "ChatLayer.hpp"

extern const Uint32 TEXT_RECIEVED_EVENT;


// make buttoncomponent, that when pressed executes the given function? -> e.g. set a value
ChatLayer::ChatLayer(){
    Entity& backgroundImage(manager.addEntity());
    backgroundImage.addComponent<RectComponent>(1000,0,1000,1000);
    backgroundImage.addComponent<SpriteComponent>(textures::grey);

    Entity& chatTextFieldEntity = manager.addEntity(); // TODO well this is just unnecessary
    chatTextFieldEntity.addComponent<RectComponent>(1000,900,100,500);
    chatTextFieldEntity.addComponent<FunctionComponent>([&chatTextFieldEntity](){
        chatTextFieldEntity.getComponent<TextFieldComponent>().onClick();
    });
    chatTextFieldEntity.addComponent<TextFieldComponent>("chat", textures::tile, 28);
    chatTextFieldEntity.getComponent<TextFieldComponent>().addKeyFunction(SDLK_RETURN, std::bind(&ChatLayer::setReturn, this)); // overwrite the default return to additionally send the event
    chatTextFieldEntity.getComponent<TextFieldComponent>().setBackgroundTexture(textures::white_background);

    chatTextFieldEntity.addGroup(groupButtons);
    chatTextField = &chatTextFieldEntity;

    Entity& chatHistoryEntity = manager.addEntity(); // TODO well this is just unnecessary
    chatHistoryEntity.addComponent<RectComponent>(1000,800,100,200);
    chatHistoryEntity.addComponent<MultilineTextComponent>(28);
    chatHistory = &chatHistoryEntity;
}

void ChatLayer::onEvent(Event& event){
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(ChatLayer::onMouseButtonPressed));
    dispatcher.dispatch<TextStringNetworkEvent>(HZ_BIND_EVENT_FN(ChatLayer::onTextRecieved));
    chatTextField->getComponent<TextFieldComponent>().onEvent(event);

}


bool ChatLayer::onTextRecieved(TextStringNetworkEvent& event){
    chatHistory->getComponent<MultilineTextComponent>().addText(event.text);
    return true;
}



bool ChatLayer::onMouseButtonPressed(MouseButtonPressedEvent& event){
    Entity* entity = getEntityAtPosition(event.x, event.y, groupButtons);
    if (entity != nullptr){
        entity->getComponent<FunctionComponent>().executeFunction();
        return true;
    }
    return false;
}

void ChatLayer::setReturn(){
    std::string text = chatTextField->getComponent<TextFieldComponent>().getText();
    TextStringNetworkEvent event{text};
    SDL_Event sdlevent = event.toSDLEvent();
    SDL_PushEvent(&sdlevent);
    // sendChatMessageNetworkEvent(chatTextField->getComponent<TextFieldComponent>().getText());
    chatTextField->getComponent<TextFieldComponent>().clear();
}