#pragma once

#include <TGUI/TGUI.hpp>
#include "Client/ClientMessage.hpp"
#include "Client/ServerMessage.hpp"
#include "GameStateManager.hpp"
#include "Client/ChessClient.hpp"
#include "Client/Scenes/Game.hpp"
#include "Client/States/RoomSelectionState.hpp"


class RegisterState : public GameState
{
public:
    void init() override;

	void cleanup() override { }
    void pause() override { }
    void resume() override { }

    void handleEvent(sf::Event& e) override;

    void update(float deltaTime) override;

    void physicsUpdate(float deltaTime) override { }

    void draw(sf::RenderTarget& target) const override;

    void initUI();
    
    static RegisterState* getInstance();

private:
    mutable tgui::Gui m_gui;
    
protected:
    RegisterState();
};