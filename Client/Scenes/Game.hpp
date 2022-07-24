#pragma once

#include <SFML/Graphics.hpp>

#include "Core/Inputs/Mouse.hpp"
#include "Core/Inputs/Keyboard.hpp"
#include "Core/Engine/ResourceManager.hpp"

#include "Client/States/GameStateManager.hpp"
#include "Client/States/InGameState.hpp"
#include "Core/Engine/ResourceManager.hpp"

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