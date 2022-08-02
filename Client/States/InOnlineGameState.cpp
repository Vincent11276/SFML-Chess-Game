#include "InOnlineGameState.hpp"
#include "Client/ChessClient.hpp"


void InOnlineGameState::init(GameStateManager* game)
{
	m_chessGame.init();
	m_chessGame.setScale(0.5, 0.5);
	m_chessGame.setPosition(50, 50);
}

void InOnlineGameState::handleEvent(GameStateManager* game, sf::Event& e)
{
	switch (e.type)
	{
	default:
		break;
	}
	m_chessGame.handleEvents(e);
}

void InOnlineGameState::update(GameStateManager* game, float deltaTime)
{
	this->m_chessGame.update(deltaTime);

	if (MouseInput::isButtonJustPressed(sf::Mouse::Button::Left))
	{
		sf::Vector2i selected = m_chessGame.getMouseHoveringPiece();

		if (m_chessGame.selectPiece(selected))
		{
			// auto& validMoves = m_chessGame.getValidMoves();

			// Logger::getInstance().log(LogLevel::INFO, 
			// 	"Valid moves for the selected piece are: " + Helper::ToString(validMoves));
		}
	}
	else if (MouseInput::isButtonJustReleased(sf::Mouse::Button::Left))
	{
		sf::Vector2i target = m_chessGame.getMouseHoveringPiece();

		if (m_chessGame.moveSelectedPiece(target)) {
			// if (m_chessGame.isCheckMate())
			// {
			// 	Logger::getInstance().log(LogLevel::INFO, "Check Mate!");
			// }
		}
	}

}

void InOnlineGameState::draw(GameStateManager* game, sf::RenderTarget& target) const
{
	target.draw(m_chessGame);
}

InOnlineGameState* InOnlineGameState::getInstance(ChessClient* client)
{
	static InOnlineGameState m_inGameState(client);

	return &m_inGameState;
}

InOnlineGameState::InOnlineGameState(ChessClient* client)
{
	// amogus
}
