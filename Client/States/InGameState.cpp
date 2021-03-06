#include "InGameState.hpp"


void InGameState::init(GameStateManager* game)
{
	m_chessGame.init();

	m_chessGame.setScale(0.5, 0.5);
	m_chessGame.setPosition(50, 50);
}

void InGameState::cleanup(GameStateManager* game) {}
void InGameState::pause(GameStateManager* game) {}
void InGameState::resume(GameStateManager* game) {}

void InGameState::handleEvent(GameStateManager* game, sf::Event& e)
{
	switch (e.type)
	{
	default:
		break;
	}
	m_chessGame.handleEvents(e);
}

void InGameState::update(GameStateManager* game, float deltaTime)
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

void InGameState::physicsUpdate(GameStateManager* game, float deltaTime)
{
	// std::cout << "Physics Update: " << deltaTime << std::endl;
}

void InGameState::draw(GameStateManager* game, sf::RenderTarget& target) const
{
	target.draw(m_chessGame);
}

InGameState* InGameState::getInstance()
{
	static InGameState m_inGameState;

	return &m_inGameState;
}

InGameState::InGameState() = default;
	// amogus
