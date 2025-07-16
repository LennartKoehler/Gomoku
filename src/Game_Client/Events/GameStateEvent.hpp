#pragma once
#include "Event.hpp"
#include "../VectorMatrix.hpp"



class GameStateEvent : public Event{
public:
    GameStateEvent(SDL_Event& event)
        : gameState(*static_cast<Matrix<int>*>(event.user.data1)){}
    GameStateEvent(const Matrix<int>& gameState)
        : gameState(gameState){}

    EVENT_CLASS_TYPE(GameStateEventClass);

    SDL_Event toSDLEvent() override {
        SDL_Event sdlEvent;
        SDL_zero(sdlEvent);
        sdlEvent.type = getSDLEventType();
        sdlEvent.user.data1 = new Matrix<int>(gameState);
        return sdlEvent;
    }
public:
    Matrix<int> gameState;
};



class GameStatePlayerEvent : public Event {
public:
    GameStatePlayerEvent(const Matrix<int>& gameState, int playerID)
        : gameState(gameState),
          playerID(playerID){}

    GameStatePlayerEvent(SDL_Event& event)
        : gameState(*static_cast<Matrix<int>*>(event.user.data1)),
          playerID(*static_cast<int*>(event.user.data2)){}


    SDL_Event toSDLEvent() override {
        SDL_Event sdlEvent;
        SDL_zero(sdlEvent);
        sdlEvent.type = getSDLEventType();
        sdlEvent.user.data1 = new Matrix<int>(gameState);
        int* playerID_Heap = new int(playerID);
        sdlEvent.user.data2 = playerID_Heap;
        return sdlEvent;
    }
    EVENT_CLASS_TYPE(GameStatePlayer);

public:
    Matrix<int> gameState;
    int playerID;
};

class SendGameStateToServerEvent : public GameStatePlayerEvent{
public:
    SendGameStateToServerEvent(const Matrix<int>& gameState, int playerID)
        : GameStatePlayerEvent(gameState, playerID){}

    SendGameStateToServerEvent(SDL_Event& event)
        : GameStatePlayerEvent(event){}
    EVENT_CLASS_TYPE(SendGameStateToServer);

};