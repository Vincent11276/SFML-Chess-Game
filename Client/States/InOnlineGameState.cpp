#include "InOnlineGameState.hpp"

void InOnlineGameState::init()
{
	Logger::debug("InOnlineGameState has been initialized");


	m_chessGame.init();

	m_chessGame.setScale(0.5, 0.5);
	m_chessGame.setPosition(50, 50);

	m_gui.setTarget(Game::getInstance()->getWindow());
	this->initUI();
}

void InOnlineGameState::handleEvent(sf::Event& e)
{
	switch (e.type)
	{
	default:
		break;
	}
	m_chessGame.handleEvents(e);
	m_gui.handleEvent(e);
}

void InOnlineGameState::update(float deltaTime)
{
	this->m_chessGame.update(deltaTime);

	if (MouseInput::isButtonJustPressed(sf::Mouse::Button::Left))
	{
		sf::Vector2i selected = m_chessGame.getMouseHoveringPiece();

		m_chessGame.trySelectPiece(selected);
	}
	else if (MouseInput::isButtonJustReleased(sf::Mouse::Button::Left))
	{
		sf::Vector2i target = m_chessGame.getMouseHoveringPiece();

		m_chessGame.tryMoveSelectedPiece(target);
	}
}

void InOnlineGameState::physicsUpdate(float deltaTime)
{
	// std::cout << "Physics Update: " << deltaTime << std::endl;
}

void InOnlineGameState::draw(sf::RenderTarget& target) const
{
	target.draw(m_chessGame);

	m_gui.draw();
}


void InOnlineGameState::initUI()
{
	try
	{
		m_gui.loadWidgetsFromFile("Assets/UI/InOnlineGame.ui");

		auto chessGame_Pnl = m_gui.get<tgui::Panel>("ChessGame_Pnl");
		auto yourProfile_Pnl = m_gui.get<tgui::Panel>("YourProfile_Pnl");
		auto enemyProfile_Pnl = m_gui.get<tgui::Panel>("EnemyProfile_Pnl");
		auto chatBox_Chtb = m_gui.get<tgui::ChatBox>("ChatBox_Chtb");
		auto moveHistory_LstV = m_gui.get<tgui::ListView>("MoveHistory_LstV");

		chatBox_Chtb->addLine("You: dasd", tgui::Color::Blue);
		chatBox_Chtb->addLine("Oponent: ada", tgui::Color::Yellow);
	}
	catch (const tgui::Exception& e)
	{
		Logger::critical("TGUI Exception: {}", e.what());
	}
}

InOnlineGameState* InOnlineGameState::getInstance()
{
	static InOnlineGameState m_inGameState;

	return &m_inGameState;
}

InOnlineGameState::InOnlineGameState() = default;
