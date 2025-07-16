#include "Game.hpp"
#include "../Networking/NetworkingClient.h"

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
    



Game::Game(){
}


void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen){
    TTF_Init();
    SDL_StartTextInput();
    EventUtils::registerEventConstructors();

    int flags = 0;
    if (fullscreen){
        flags = SDL_WINDOW_FULLSCREEN;
    }
    
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        std::cout << "Subsystem initialized" << std::endl;

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
        if (window){
            std::cout << "Window created" << std::endl;
        }
        

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            std::cout << "Renderer created" << std::endl;
        }

        isRunning = true;
    }
    sceneManager = new SceneManager();
    sceneManager->setActiveScene(sceneManager->mainMenu);

}

//TODO handleEvents can be adjusted to fit the event dispatcher system
void Game::handleEvents(SDL_Event& event){
    std::unique_ptr<Event> convertedEvent = EventUtils::convertEventFromSDL( event );
    EventDispatcher dispatcher(*convertedEvent);
    EventType type = convertedEvent->getEventType();
    // if (convertedEvent->getEventType() == EventType::Code) {
    //     update();
    //     render();
    // }

    sceneManager->getActiveScene()->handleEvent(*convertedEvent);

        // GameStatePlayerEvent* gsEvent = dynamic_cast<GameStatePlayerEvent*>(convertedEvent.get());
    dispatcher.dispatch<GameStatePlayerEvent>(HZ_BIND_EVENT_FN(sendGameStateToServer)); // TODO i dont think i need the if statements when using the dispatcher?
    
        // NetworkConnectionEvent* netEvent = dynamic_cast<NetworkConnectionEvent*>(convertedEvent.get());
    dispatcher.dispatch<NetworkConnectionEvent>(HZ_BIND_EVENT_FN(setClient)); // TODO i dont think i need the if statements when using the dispatcher?

        // setClient(netEvent->IPAddress);
    
    // if (convertedEvent->getEventType() == EventType::Text) {
    //     TextEvent* netEvent = dynamic_cast<TextEvent*>(convertedEvent.get());
    //     sceneManager->getActiveScene()->handleEvent(*convertedEvent);
    // }
    dispatcher.dispatch<TextStringNetworkEvent>(HZ_BIND_EVENT_FN(sendTextToServer)); // TODO i dont think i need the if statements when using the dispatcher?

        // TextEvent* textEvent = dynamic_cast<TextEvent*>(convertedEvent.get());
        // sendTextToServer(*textEvent);
    
    if (convertedEvent->getEventType() == EventType::Quit) {
        isRunning = false;
    }

}

void Game::update(){
    sceneManager->getActiveScene()->refresh();
    sceneManager->getActiveScene()->update();
}

void Game::render(){
    SDL_RenderClear(renderer);
    sceneManager->getActiveScene()->draw();
    SDL_RenderPresent(renderer);

}

void Game::clean(){
    // delete gameState;
    delete sceneManager;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}



bool Game::isHandledEvent(SDL_Event& event){
    bool found = (EventUtils::fromSDLType.find(event.type) != EventUtils::fromSDLType.end());
    return found;
}
void Game::handleNetwork(){
    std::lock_guard<std::mutex> lock(clientMutex);
    if (client != nullptr){
        client->iteration(); // check for new data from the server
    }
}

bool Game::sendGameStateToServer(GameStatePlayerEvent& event){
    if (client != nullptr){
        std::vector<uint8_t> data = Serializer::serializeGameState(event.gameState, event.playerID);
        Package package{MessageType::GAME_STATE_UPDATE, data};
        sendPackageToServer(package);
        return true;
    }
    return false;
}

bool Game::sendTextToServer(TextStringNetworkEvent& event){
    if (client != nullptr){
        std::vector<uint8_t> data = Serializer::serializeText(std::string(event.text));
        Package package{MessageType::TEXT_MESSAGE, data};
        sendPackageToServer(package);
        return true;
    }
    return false;
}

bool Game::setClient(NetworkConnectionEvent& event){
    const char* serverAddress = event.IPAddress;
    SteamNetworkingIPAddr IPaddr;
    if (!get_address(serverAddress, IPaddr) | client != nullptr){
        std::cout << "Problemski connecting to Server IP Address: " << serverAddress << std::endl;
    }
    else {client = run_client(IPaddr);}
    return true;
}

void Game::sendPackageToServer(Package package){
    client->sendToServer(package);
}

