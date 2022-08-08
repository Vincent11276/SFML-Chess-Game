#pragma once

#include <iostream>

#include "GameStateManager.hpp"
#include "GameState.hpp"

#include "Client/Scenes/Chess.hpp"
#include "Core/Utility/Helper.hpp"


class InOfflineGameState : public GameState
{
public:
	void init() override;
	void cleanup() override { };
	void pause() override { };
	void resume() override { };

	void handleEvent(sf::Event& e) override;
	void update(float deltaTime) override;
	void physicsUpdate(float deltaTime) override;
	void draw(sf::RenderTarget& target) const override;

	static InOfflineGameState* getInstance();

protected:
	InOfflineGameState();

private:
	ChessGame m_chessGame;
};

