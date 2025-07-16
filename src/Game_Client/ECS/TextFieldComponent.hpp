#pragma once
#include "../TextureManager.hpp"
#include "Components.hpp"
#include <SDL2/SDL.h>
#include <iostream>

#include "../Events/EventUtils.hpp"



class TextFieldComponent : public Component{
protected:
    static TextFieldComponent* focusedField;
    std::string text;
    std::string defaultText;
    RectComponent* rectComp;
    SDL_Rect srcRect, destRect;
    SDL_Texture* textTexture;
    SDL_Texture* backgroundTexture;
    int fontsize = 28;
    std::unordered_map<SDL_Keycode, std::function<void()>> keyFunctions;
    std::unordered_map<EventType, std::function<bool(Event&)>> onEventFunctions;

public:
    TextFieldComponent() = default;
    TextFieldComponent(std::string text, const char* backgroundPath, int fontsize): fontsize(fontsize), defaultText(text){
        setTextTexture(text);
        setBackgroundTexture(backgroundPath);
    }

    void init(){
        rectComp = &entity->getComponent<RectComponent>(); 
        srcRect.x = srcRect.y = 0;
        srcRect.w = srcRect.h = 50;
        addKeyFunction(SDLK_BACKSPACE, std::bind(&TextFieldComponent::setBackspace, this));
        addKeyFunction(SDLK_RETURN, std::bind(&TextFieldComponent::setReturn, this));
        addEventFunction<TextStringEvent>([this](TextStringEvent& e) { this->addLetter(e);
        return true; });
        addEventFunction<KeyDownEvent>(std::bind(&TextFieldComponent::onKeyDownEvent, this, std::placeholders::_1));
    }

    void update() override{
        destRect.x = (int)rectComp->x;
        destRect.y = (int)rectComp->y;
    }


    void draw() override{
        if (backgroundTexture != nullptr){
            TextureManager::Draw(backgroundTexture, srcRect, destRect);
        }
        TextureManager::Draw(textTexture, srcRect, destRect);
    }


    void setBackgroundTexture(const char* path){
        backgroundTexture = TextureManager::LoadTexture(path);
    }

    void setTextTexture(const std::string& newtext){
        textTexture = TextureManager::WriteText(newtext, fontsize);
        SDL_QueryTexture(textTexture, NULL, NULL, &destRect.w, &destRect.h);
    }

    std::string getText() const{
        return text;
    }

    void addLetter(TextCharEvent& text){
        this->text.append(std::string{text.text});
    }
    void addLetter(TextStringEvent& text){
        this->text.append(text.text);
    }

    void addLetter(const KeyDownEvent& event){ // unused
        if (event.key >= SDLK_SPACE && event.key <= SDLK_z) {
            char c = static_cast<char>(event.key);
            text.append(std::string{c});
        }
    }

    bool onKeyDownEvent(KeyDownEvent& event){
        auto it = keyFunctions.find(event.key);
        if (it != keyFunctions.end()) {
            it->second(); // Call the registered function
            return true;
        }
        return false;
    }

    bool onTextStringEvent(TextStringEvent& event){
        text.append(event.text);
        setTextTexture(text);
        return true;
    }

    void reset(){
        text.clear();
        text = defaultText;
        setTextTexture(text);
    }

    void clear(){
        text.clear();
        setTextTexture(text);
    }


    void setBackspace(){
        if (text.length() > 0){
            text.pop_back();
        }
    }

    void setReturn(){
        clear();
    }

    bool onClick(){
        if (focusedField != nullptr){
            focusedField->reset();
        }
        focusedField = this;
        clear();
        return true;
    }
    
    void addKeyFunction(SDL_Keycode key, std::function<void()> func){
        keyFunctions[key] = std::move(func);
    }


    template<typename EventT>
    void addEventFunction(std::function<bool(EventT&)> func) {
        static_assert(std::is_base_of<Event, EventT>::value, "EventT must derive from Event");
        EventType type = EventT::getStaticType(); 
        onEventFunctions[type] = [func](Event& e) -> bool {
            return func(static_cast<EventT&>(e));
        };
    }

    bool onEvent(Event& event){
        auto it = onEventFunctions.find(event.getEventType());
        bool r = false;
        if (it != onEventFunctions.end() & isFocused()) {
            r = it->second(event); // Call the registered function
            setTextTexture(this->text);
        }
        return r;
    }

    bool isFocused(){
        return focusedField == this;
    }


};



