#pragma once

#include <iostream>
#include <TGUI/Backends/SFML.hpp>
#include <TGUI/TGUI.hpp>

#include "Client/States/GameStateManager.hpp"
#include "Client/States/InOfflineGameState.hpp"
#include "Client/States/RoomSelectionState.hpp"
#include "Client/States/MainMenuState.hpp"
#include "Client/ChessClient.hpp"


class MainMenuState : public GameState
{
public:
	void init(GameStateManager* game) override;
	void cleanup(GameStateManager* game) override;
    void pause(GameStateManager* game) override;
    void resume(GameStateManager* game) override;

	void handleEvent(GameStateManager* manager, sf::Event& e) override;
	void update(GameStateManager* manager, float deltaTime) override;
    void physicsUpdate(GameStateManager* manager, float deltaTime) override;
    void draw(GameStateManager* manager, sf::RenderTarget& target) const override;

	static MainMenuState* getInstance(sf::RenderWindow* target, ChessClient* client);

    void initUI();

    void on_PlayBtn_Pressed();
    void on_SettingsBtn_pressed();
    void on_OnlineBtn_Pressed();
    void on_ExitBtn_Pressed();

protected:
	MainMenuState();

private:
    sf::RenderWindow* m_window;
    ChessClient* m_client;
    mutable tgui::Gui gui;
};
