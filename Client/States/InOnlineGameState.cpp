#include "InOnlineGameState.hpp"

InOnlineGameState::InOnlineGameState()
{
	// default..
}

void InOnlineGameState::init()
{
	Logger::debug("InOnlineGameState has been initialized");

	this->initChess();
	this->initUI();
	this->initNetworking();

	// temporary only -- Tell the server player is ready to play
	auto message = netw::ClientMessage(netw::ClientMessage::Type::PlayerReadyToPlay);
	netw::ChessClient::getInstance().sendMessage(message);
}

void InOnlineGameState::initChess()
{
	m_chessGame.init(PieceColor::White);
	m_chessGame.setScale(0.5, 0.5);
	m_chessGame.setPosition(50, 50);
}

void InOnlineGameState::initUI()
{
	try
	{
		m_gui.setTarget(Game::getInstance()->getWindow());
		m_gui.loadWidgetsFromFile("Assets/UI/InOnlineGame.ui");

		m_yourProfile_Pnl = m_gui.get<tgui::Panel>("YourProfile_Pnl");
		m_enemyProfile_Pnl = m_gui.get<tgui::Panel>("EnemyProfile_Pnl");
		m_chatBox_Chtb = m_gui.get<tgui::ChatBox>("ChatBox_Chtb");
		m_moveHistory_LstV = m_gui.get<tgui::ListView>("MoveHistory_LstV");
		m_chatBox_Edt = m_gui.get<tgui::EditBox>("ChatBox_Edt");
		m_send_Btn = m_gui.get<tgui::Button>("Send_Btn");

		m_send_Btn->onPress([&] {
			netw::ChessClient::getInstance().sendChatMessage(m_chatBox_Edt->getText().toStdString());
			m_chatBox_Edt->setText("");
			});
	}
	catch (const tgui::Exception& e)
	{
		Logger::critical("TGUI Exception: {}", e.what());
	}
}

void InOnlineGameState::initNetworking()
{
	netw::ChessClient::getInstance().registerCallback(netw::ServerMessage::Type::PlayerSentMessage,
		[&](netw::ServerMessage* message) {
			auto content = &message->getContent<netw::ServerMessage::PlayerSentMessage>();
			m_chatBox_Chtb->addLine(content->author + ": " + content->text, tgui::Color::Red);
		});

	netw::ChessClient::getInstance().registerCallback(netw::ServerMessage::Type::PlayerSentMessage,
		[&](netw::ServerMessage* message) {
			auto content = &message->getContent<netw::ServerMessage::PlayerSentMessage>();
			m_chatBox_Chtb->addLine(content->author + ": " + content->text, tgui::Color::Red);
		});

	netw::ChessClient::getInstance().registerCallback(netw::ServerMessage::Type::PlayerMovedPiece,
		[&](netw::ServerMessage* message) {
			auto content = &message->getContent<netw::ServerMessage::PlayerMovedPiece>();
			m_chessGame.trySelectPiece(content->selected);
			m_chessGame.tryMoveSelectedPiece(content->target);
		});

	netw::ChessClient::getInstance().registerCallback(netw::ServerMessage::Type::PlayerHasDisconnected,
		[&](netw::ServerMessage* message) {
			Logger::info("Your oponnent has disconnected");
		});

	netw::ChessClient::getInstance().registerCallback(netw::ServerMessage::Type::PlayerHasReconnected,
		[&](netw::ServerMessage* message) {
			Logger::info("Your oponnent has reconnected");
		});

	netw::ChessClient::getInstance().registerCallback(netw::ServerMessage::Type::PlayerHasResigned,
		[&](netw::ServerMessage* message) {
			Logger::info("Your oponnent has resigned the game");
		});

	netw::ChessClient::getInstance().registerCallback(netw::ServerMessage::Type::PlayerRequestsForDraw,
		[&](netw::ServerMessage* message) {
			Logger::info("Your oponnent has requested for a draw");
		});

	netw::ChessClient::getInstance().registerCallback(netw::ServerMessage::Type::PlayerJoinedRoom,
		[&](netw::ServerMessage* message) {
			auto* content = &message->getContent<netw::ServerMessage::PlayerJoinedRoom>();
			Logger::info("{} has joined your room", content->name);
		});

	netw::ChessClient::getInstance().registerCallback(netw::ServerMessage::Type::ChessGameStarted,
		[&](netw::ServerMessage* message) {
			auto* content = &message->getContent<netw::ServerMessage::ChessGameStarted>();

			if (content->side == netw::Player::Color::White)
			{
				m_chessGame.init(PieceColor::White);
				Logger::info("Game has started! You are White");
			}
			else
			{
				m_chessGame.init(PieceColor::Black);
				Logger::info("Game has started! You are Black");
			}
		});
}

InOnlineGameState* InOnlineGameState::getInstance()
{
	static InOnlineGameState m_inGameState;

	return &m_inGameState;
}

void InOnlineGameState::handleEvent(sf::Event& e)
{
	m_chessGame.handleEvents(e);
	m_gui.handleEvent(e);
}

void InOnlineGameState::update(float deltaTime)
{
	if (MouseInput::isButtonJustPressed(sf::Mouse::Button::Left))
	{
		m_chessGame.trySelectPiece(m_chessGame.getMouseHoveringCoords());
	}
	else if (MouseInput::isButtonJustReleased(sf::Mouse::Button::Left))
	{
		const auto& targetCoords = m_chessGame.getMouseHoveringCoords();
		
		if (m_chessGame.tryMoveSelectedPiece(targetCoords))
		{
		/*	if (m_chessGame)
			ChessClient::getInstance().movePiece(ClientMessage::MovePiece{
				.selected = m_chessGame.getSelectedCoords(),
				.target = targetCoords
			});*/
		}
	}
	this->m_chessGame.update(deltaTime);
}

void InOnlineGameState::draw(sf::RenderTarget& target) const
{
	m_gui.draw();
	target.draw(m_chessGame);
}

