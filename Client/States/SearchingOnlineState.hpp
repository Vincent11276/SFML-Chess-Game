#pragma once

#include <TGUI/TGUI.hpp>
#include "TGUI/Backends/SFML.hpp"
#include "GameStateManager.hpp"
#include "Client/States/GameState.hpp"


class SearchingOnlineState : public GameState
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

	static SearchingOnlineState* getInstance(sf::RenderWindow* target)
    {
        static SearchingOnlineState newInstance { target };

        return &newInstance;
    }

    void initUI()
    {
        m_gui.loadWidgetsFromFile("../Assets/form1.txt");
    }
    
private:
    mutable tgui::Gui m_gui;

protected:
    SearchingOnlineState(sf::RenderWindow* target)
    {
        m_gui.setTarget(*target);
    }
};