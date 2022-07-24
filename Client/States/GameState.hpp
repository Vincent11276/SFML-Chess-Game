#pragma once

#include "GameStateManager.hpp"
#include <SFML/Graphics.hpp>

class GameState
{
public:
	GameState() {}

	virtual void init() = 0;
	virtual void cleanup() = 0;
	virtual void pause() = 0;
	virtual void resume() = 0;

	virtual void handleEvent(GameStateManager* game, [[maybe_unused]] sf::Event& event) = 0;
	virtual void update(GameStateManager* game, [[maybe_unused]] float deltaTime) = 0;
	virtual void physicsUpdate(GameStateManager* game, [[maybe_unused]] float deltaTime) = 0;
	virtual void draw(GameStateManager* game, [[maybe_unused]] sf::RenderTarget& target) const = 0;

	void changeState(GameStateManager* game, GameState* state) {
		game->changeState(state);
	}
};
