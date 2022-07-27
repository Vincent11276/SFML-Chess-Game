#include "InOfflineGameState.hpp"


void InOfflineGameState::init(GameStateManager* game)
{
	m_chessGame.init();

	m_chessGame.setScale(0.5, 0.5);
	m_chessGame.setPosition(50, 50);
}

void InOfflineGameState::cleanup(GameStateManager* game) {}
void InOfflineGameState::pause(GameStateManager* game) {}
void InOfflineGameState::resume(GameStateManager* game) {}

void InOfflineGameState::handleEvent(GameStateManager* game, sf::Event& e)
{
	switch (e.type)
	{
	default:
		break;
	}
	m_chessGame.handleEvents(e);
}

void InOfflineGameState::update(GameStateManager* game, float deltaTime)
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

void InOfflineGameState::physicsUpdate(GameStateManager* game, float deltaTime)
{
	// std::cout << "Physics Update: " << deltaTime << std::endl;
}

void InOfflineGameState::draw(GameStateManager* game, sf::RenderTarget& target) const
{
	target.draw(m_chessGame);
}

InOfflineGameState* InOfflineGameState::getInstance()
{
	static InOfflineGameState m_inGameState;

	return &m_inGameState;
}

InOfflineGameState::InOfflineGameState() = default;
	// amogus
