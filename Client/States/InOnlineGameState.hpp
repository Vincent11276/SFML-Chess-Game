#pragma once

#include <iostream>

#include "GameStateManager.hpp"
#include "GameState.hpp"

#include "Client/Scenes/Chess.hpp"
#include "Core/Utility/Helper.hpp"
#include "Client/ServerMessage.hpp"
#include "Client/ChessClient.hpp"
#include <TGUI/TGUI.hpp>


class InOnlineGameState : public GameState
{
public:
	void init() override;
	void cleanup() override { };
	void pause() override { };
	void resume() override { };

	void initUI();

	void handleEvent(sf::Event& e) override;
	void update(float deltaTime) override;
	void physicsUpdate(float deltaTime) override;
	void draw(sf::RenderTarget& target) const override;

	static InOnlineGameState* getInstance();

protected:
	InOnlineGameState();

private:
	mutable tgui::Gui m_gui;

	ChessGame m_chessGame;
};



