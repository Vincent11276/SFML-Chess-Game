#include "Game.hpp"

#include "Client/States/MainMenuState.hpp"


Game::Game()
	: m_window(sf::VideoMode(854, 480), "Chess Game")
{
	// m_window.setTitle("Chess Game");
	// m_window.setSize({ 800, 600 });

	m_window.setKeyRepeatEnabled(false);
	m_window.setFramerateLimit(60);

	MouseInput::setWindow(m_window);
	Logger::getInstance().setLogLevel(LogLevel::DEBUG);
}

void Game::run()
{
	this->loadResources();
	
	m_gameStateManager.changeState(MainMenuState::getInstance(&m_window));

	const sf::Time fixedUpdateInterval = sf::seconds(1.0f / 50.0f); 

	sf::Clock timer;
	sf::Time lag;

	while (m_window.isOpen())
	{
		sf::Time elapsed = timer.restart();
		lag += elapsed;

		KeyboardInput::resetStates();
		MouseInput::resetStates();

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
	ResourceManager::addTexture(ResourceKey::Background, "Assets/UI/Background.png");
	ResourceManager::addTexture(ResourceKey::ExitButton, "Assets/UI/Background.png");
	ResourceManager::addTexture(ResourceKey::Logo, "Assets/UI/Background.png");
	ResourceManager::addTexture(ResourceKey::OnlineButton, "Assets/UI/Background.png");
	ResourceManager::addTexture(ResourceKey::PlayButton, "Assets/UI/Background.png");
	ResourceManager::addTexture(ResourceKey::SettingsButton, "Assets/UI/Background.png");

	
	ResourceManager::addTexture(ResourceKey::ActionMark, "Assets/action_mark.png");
	ResourceManager::addTexture(ResourceKey::CanMoveMark, "Assets/can_move_mark.png");
	ResourceManager::addTexture(ResourceKey::CanTakeMark, "Assets/can_take_mark.png");

	ResourceManager::addTexture(ResourceKey::WoodChessBoard, "Assets/Chess_Artwork/Chess_Artwork/Chess Board/Wood/Chess_Board.png");

	ResourceManager::addTexture(ResourceKey::WoodPieceBishopB, "Assets/Chess_Artwork/Chess_Artwork/Chess Pieces/Wood/BishopB.png");
	ResourceManager::addTexture(ResourceKey::WoodPieceBishopW, "Assets/Chess_Artwork/Chess_Artwork/Chess Pieces/Wood/BishopW.png");
	ResourceManager::addTexture(ResourceKey::WoodPieceKingB, "Assets/Chess_Artwork/Chess_Artwork/Chess Pieces/Wood/KingB.png");
	ResourceManager::addTexture(ResourceKey::WoodPieceKingW, "Assets/Chess_Artwork/Chess_Artwork/Chess Pieces/Wood/KingW.png");
	ResourceManager::addTexture(ResourceKey::WoodPieceKnightB, "Assets/Chess_Artwork/Chess_Artwork/Chess Pieces/Wood/KnightB.png");
	ResourceManager::addTexture(ResourceKey::WoodPieceKnightW, "Assets/Chess_Artwork/Chess_Artwork/Chess Pieces/Wood/KnightW.png");
	ResourceManager::addTexture(ResourceKey::WoodPiecePawnB, "Assets/Chess_Artwork/Chess_Artwork/Chess Pieces/Wood/PawnB.png");
	ResourceManager::addTexture(ResourceKey::WoodPiecePawnW, "Assets/Chess_Artwork/Chess_Artwork/Chess Pieces/Wood/PawnW.png");
	ResourceManager::addTexture(ResourceKey::WoodPieceQueenB, "Assets/Chess_Artwork/Chess_Artwork/Chess Pieces/Wood/QueenB.png");
	ResourceManager::addTexture(ResourceKey::WoodPieceQueenW, "Assets/Chess_Artwork/Chess_Artwork/Chess Pieces/Wood/QueenW.png");
	ResourceManager::addTexture(ResourceKey::WoodPieceRookB, "Assets/Chess_Artwork/Chess_Artwork/Chess Pieces/Wood/RookB.png");
	ResourceManager::addTexture(ResourceKey::WoodPieceRookW, "Assets/Chess_Artwork/Chess_Artwork/Chess Pieces/Wood/RookW.png");
}