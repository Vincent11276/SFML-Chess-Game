#pragma once

#include <TGUI/TGUI.hpp>
#include "TGUI/Backends/SFML.hpp"
#include "GameStateManager.hpp"
#include "Client/States/GameState.hpp"
#include "Client/ChessClient.hpp"
#include "Client/Scenes/Game.hpp"

class RoomSelectionState : public GameState
{
public:
    void init(GameStateManager* game) override 
    { 
        this->initUI();
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

	static RoomSelectionState* getInstance()
    {
        static RoomSelectionState newInstance;

        return &newInstance;
    }

    void initUI()
    {
        m_gui.loadWidgetsFromFile("../Assets/form1.txt");
    }
    
private:
    mutable tgui::Gui m_gui;

protected:
    RoomSelectionState()
    {
        m_gui.setTarget(Game::getInstance()->getWindow());
    }
};