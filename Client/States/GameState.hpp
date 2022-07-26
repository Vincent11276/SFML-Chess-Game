#pragma once

#include "GameStateManager.hpp"
#include <SFML/Graphics.hpp>

class GameState
{
public:
	GameState() {}

	virtual void init(GameStateManager* game) = 0;
	virtual void cleanup(GameStateManager* game) = 0;
	virtual void pause(GameStateManager* game) = 0;
	virtual void resume(GameStateManager* game) = 0;

	virtual void handleEvent(GameStateManager* game, [[maybe_unused]] sf::Event& event) = 0;
	virtual void update(GameStateManager* game, [[maybe_unused]] float deltaTime) = 0;
	virtual void physicsUpdate(GameStateManager* game, [[maybe_unused]] float deltaTime) = 0;
	virtual void draw(GameStateManager* game, [[maybe_unused]] sf::RenderTarget& target) const = 0;

	GameStateManager* getGameStateManager()
	{
		return m_gameStateManager;
	}

	void changeState(GameStateManager* game, GameState* state) 
	{
		game->changeState(state);
	}

	void setGameStateManager(GameStateManager* gameStateManager) 
	{
		m_gameStateManager = gameStateManager;
	}

	GameStateManager* m_gameStateManager;
};
