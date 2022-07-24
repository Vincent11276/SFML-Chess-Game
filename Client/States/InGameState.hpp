#pragma once

#include <iostream>

#include "GameStateManager.hpp"
#include "GameState.hpp"

#include "Client/Scenes/Chess.hpp"
#include "Core/Utility/Helper.hpp"


class InGameState : public GameState
{
public:
	void init() override;
	void cleanup() override;
	void pause() override;
	void resume() override;

	void handleEvent(GameStateManager* manager, sf::Event& e) override;
	void update(GameStateManager* manager, float deltaTime) override;
	void physicsUpdate(GameStateManager* manager, float deltaTime) override;
	void draw(GameStateManager* manager, sf::RenderTarget& target) const override;

	static InGameState* getInstance();

protected:
	InGameState();

private:
	ChessGame m_chessGame;
};

