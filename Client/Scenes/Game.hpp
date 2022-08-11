#pragma once

#include <SFML/Graphics.hpp>
#include "Core/Inputs/Mouse.hpp"
#include "Core/Inputs/Keyboard.hpp"
#include "Core/Engine/ResourceManager.hpp"
#include "Core/Engine/GameStateManager.hpp"
#include "Core/Engine/ResourceManager.hpp"
#include "Client/States/InOfflineGameState.hpp"
#include "Network/ChessClient.hpp"

class Game
{
public:
	void init();
	void run();
	void mainLoop();

	void loadResources();

	static Game* getInstance()
	{
		static Game instance;

		return &instance;
	}

	sf::RenderWindow& getWindow()
	{
		return m_window;
	}

private:
	Game();

	sf::RenderWindow m_window;
};