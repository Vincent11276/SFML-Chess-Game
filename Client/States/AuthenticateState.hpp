#pragma once

#include <TGUI/TGUI.hpp>
#include "Client/ClientMessage.hpp"
#include "Client/ServerMessage.hpp"
#include "Client/States/RoomSelectionState.hpp"
#include "TGUI/Backends/SFML.hpp"
#include "GameStateManager.hpp"
#include "Client/States/GameState.hpp"
#include "Client/ChessClient.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "Client/Scenes/Game.hpp"

class AuthenticateState : public GameState
{
public:
    void init(GameStateManager* game) override 
    { 
        this->initUI();

        Game::getInstance()->getClient().onMessageReceived([&] (ServerMessage* message)
        {
            if (message->type == ServerMessage::Type::AuthenticateSuccess)
            {
                auto content = message->getData<ServerMessage::AuthenticateSuccess>();
                std::cout << "Succesfully authenticated and your ID is: " << content.assignedId;
                
                Game::getInstance()->isAuthenticated = true;
                game->changeState(RoomSelectionState::getInstance());
            }
            else if (message->type == ServerMessage::Type::AuthenticateFailed)
            {
                auto content = message->getData<ServerMessage::AuthenticateFailed>();

                std::cout << "Failed to authenticate. Reason: ";

                switch (content.reason)
                {
                    case ServerMessage::AuthenticateFailed::ServerFull:
                        std::cout << "Server is full" << std::endl;
                        break;

                    case ServerMessage::AuthenticateFailed::NameAlreadyExists:
                        std::cout << "Name already taken" << std::endl;
                        break;

                    case ServerMessage::AuthenticateFailed::AlreadyAuthenticated:
                        std::cout << "Already authenticated" << std::endl;
                        break;
                }
            }
        });
    }
	void cleanup(GameStateManager* game) override { }
    void pause(GameStateManager* game) override { }
    void resume(GameStateManager* game) override { }

	void handleEvent(GameStateManager* manager, sf::Event& e) override
    {
        m_gui.handleEvent(e);
    }

	void update(GameStateManager* manager, float deltaTime) override { }
    void physicsUpdate(GameStateManager* manager, float deltaTime) override { }

    void draw(GameStateManager* manager, sf::RenderTarget& target) const override
    {
        m_gui.draw();
    }

    void onClientReceiveMessage()
    {

    }

    void initUI()                
    {
        m_gui.loadWidgetsFromFile("Assets/form2.txt");

        auto editBox1 = m_gui.get<tgui::EditBox>("EditBox1");
        
        auto button = m_gui.get<tgui::Button>("Button1");
        button->onPress([&] { 
            //Game::getInstance()->client.authenticate(editBox1->getText().toStdString());
            Game::getInstance()->getClient().authenticate("Vincent");
        });
    }
    
    static AuthenticateState* getInstance()
    {
        static AuthenticateState instance;

        return &instance;
    }

private:
    ChessClient* client;
    mutable tgui::Gui m_gui;
    
    AuthenticateState()
    {
        m_gui.setTarget(Game::getInstance()->getWindow());
    }
};