#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

class GameState;

class GameStateManager
{
public:
	GameStateManager();

	void changeState(GameState* state);
	void cleanUp();
	void pushState(GameState* state);
	void popState();

	void handleEvent(sf::Event& event);
	void update(float deltaTime);
	void physicsUpdate(float deltaTime);
	void render(sf::RenderTarget& target);

private:
	// the stack of states
	std::vector<GameState*> m_states;
};