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

	sf::RenderWindow& getWindow();

	void setCursor(sf::Cursor::Type type);

	static Game* getInstance();

private:
	Game();

	sf::Cursor m_cursor;
	sf::RenderWindow m_window;
};