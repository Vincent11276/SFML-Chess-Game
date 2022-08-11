#include "Game.hpp"

#include "Client/States/MainMenuState.hpp"


Game::Game()
	: m_window(sf::VideoMode(854, 480), "Chess Game")
{
	m_window.setKeyRepeatEnabled(false);
	m_window.setFramerateLimit(60);

	MouseInput::setWindow(m_window);
}

void Game::init()
{
	this->loadResources();


	// Wire up callbacks for the client
	netw::ChessClient::getInstance().registerCallback(netw::ServerMessage::Type::AuthenticateSuccess, [&](netw::ServerMessage* message) {
		auto* content = &message->getContent<netw::ServerMessage::AuthenticateSuccess>();
		netw::ChessClient::getInstance().session.token = content->assignedToken;
		netw::ChessClient::getInstance().session.isAuthenticated = true;
		Logger::info("You have been authenticated and your token is {}", content->assignedToken);
		});

	netw::ChessClient::getInstance().registerCallback(netw::ServerMessage::Type::AuthenticateFailed, [&](netw::ServerMessage* message) {
		netw::ChessClient::getInstance().session.isAuthenticated = false;
		Logger::error("Unable to authenticate you from the server");
	});

	netw::ChessClient::getInstance().registerCallback(netw::ServerMessage::Type::RegistrationSuccess, [&](netw::ServerMessage* message) {
		Logger::info("Succesfully registered!");

		auto& content = message->getContent<netw::ServerMessage::AuthenticateSuccess>();
		netw::ChessClient::getInstance().session.isRegistered = true;
	});

	netw::ChessClient::getInstance().registerCallback(netw::ServerMessage::Type::RegistrationFailed, [&](netw::ServerMessage* message) {
		auto& content = message->getContent<netw::ServerMessage::RegistrationFailed>();

		std::cout << "Failed to authenticate. Reason: ";

		switch (content.reason)
		{
		case netw::ServerMessage::RegistrationFailed::Reason::ServerFull:
			std::cout << "Server is full" << std::endl;
			break;

		case netw::ServerMessage::RegistrationFailed::Reason::NameAlreadyExists:
			std::cout << "Name is already taken" << std::endl;
			break;

		case netw::ServerMessage::RegistrationFailed::Reason::AlreadyRegistered:
			std::cout << "You are already registered" << std::endl;
			break;
		}
	});
}


void Game::run()
{
	if (netw::ChessClient::getInstance().connect())
	{
		netw::ChessClient::getInstance().authenticate();
	}
	// Proceed to starting state when loop starts
	GameStateManager::getInstance()->changeState(MainMenuState::getInstance());	

	// Start the main loop
	this->mainLoop();
}

void Game::mainLoop()
{
	const sf::Time fixedUpdateInterval = sf::seconds(1.0f / 50.0f);

	sf::Clock timer;
	sf::Time lag;

	while (m_window.isOpen())
	{
		sf::Time elapsed = timer.restart();
		lag += elapsed;

		KeyboardInput::resetStates();
		MouseInput::resetStates();

		for (sf::Event e; m_window.pollEvent(e); )
		{
			KeyboardInput::handleEvent(e);
			MouseInput::handleEvent(e);

			switch (e.type)
			{
			case::sf::Event::Closed:
				m_window.close();
				break;

			default:
				break;
			}
			GameStateManager::getInstance()->handleEvent(e);
		}
		GameStateManager::getInstance()->update(elapsed.asSeconds());

		while (lag >= fixedUpdateInterval)
		{
			lag -= fixedUpdateInterval;

			GameStateManager::getInstance()->physicsUpdate(fixedUpdateInterval.asSeconds());
		}

		m_window.clear();
		GameStateManager::getInstance()->render(m_window);
		m_window.display();
	}
}

void Game::loadResources()
{
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