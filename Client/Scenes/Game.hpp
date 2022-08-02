#pragma once

#include <SFML/Graphics.hpp>

#include "Core/Inputs/Mouse.hpp"
#include "Core/Inputs/Keyboard.hpp"
#include "Core/Engine/ResourceManager.hpp"

#include "Client/States/GameStateManager.hpp"
#include "Client/States/InOfflineGameState.hpp"
#include "Core/Engine/ResourceManager.hpp"

#include "Client/ChessClient.hpp"

class Game
{
public:

	bool isAuthenticated = false;

	void run();
	void loadResources();

	static Game* getInstance()
	{
		static Game instance;

		return &instance;
	}

	ChessClient& getClient()
	{
		return m_client;
	}
	
	sf::RenderWindow& getWindow()
	{
		return m_window;
	}

private:
	ChessClient m_client;
	GameStateManager m_gameStateManager;
	sf::RenderWindow m_window;
	Game();

};