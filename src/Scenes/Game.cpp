#include "Game.hpp"


Game::Game()
	: m_window(sf::VideoMode(600, 600), "Chess Game")
{
	m_window.setKeyRepeatEnabled(false);
	m_window.setFramerateLimit(60);

	MouseInput::setWindow(m_window);
	Logger::getInstance().setLogLevel(LogLevel::DEBUG);
}

void Game::run()
{
	m_gameStateManager.changeState(InGameState::getInstance());

	const sf::Time fixedUpdateInterval = sf::seconds(1.0f / 50.0f);

	sf::Clock timer;
	sf::Time lag;

	while (m_window.isOpen())
	{
		sf::Time elapsed = timer.restart();
		lag += elapsed;

		for (sf::Event event; m_window.pollEvent(event); )
		{
			KeyboardInput::handleEvent(event);
			MouseInput::handleEvent(event);
			
			switch (event.type)
			{
			case::sf::Event::Closed:
				m_window.close();
				break;

			default:
				break;
			}
			m_gameStateManager.handleEvent(event);
			
		}
		m_gameStateManager.update(elapsed.asSeconds());

		while (lag >= fixedUpdateInterval)
		{
			lag -= fixedUpdateInterval;

			m_gameStateManager.physicsUpdate(fixedUpdateInterval.asSeconds());
		}

		m_window.clear();
		m_gameStateManager.render(m_window);
		m_window.display();
	}
}

void Game::loadResources()
{
	ResourceManager::addResource(new RTexture, "chess board",
		"../../../Assets/Chess_Artwork/Chess Board/Wood/Chess_Board.png");
}