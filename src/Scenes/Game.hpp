#pragma once

#include <SFML/Graphics.hpp>

#include "../Inputs/Mouse.hpp"
#include "../Inputs/Keyboard.hpp"
#include "../Engine/ResourceManager.hpp"
#include "../States/GameStateManager.hpp"
#include "../States/InGameState.hpp"
#include "../Engine/ResourceManager.hpp"

class Game
{
public:
	Game();

	void run();

	void loadResources();

private:
	GameStateManager m_gameStateManager;
	sf::RenderWindow m_window;
};