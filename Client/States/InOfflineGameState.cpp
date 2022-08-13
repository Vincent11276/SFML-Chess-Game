#include "InOfflineGameState.hpp"


namespace states
{
	void InOfflineGameState::init()
	{
		Logger::debug("InOfflineGameState has been initialized");

		m_chessGame.init(chess::PieceColor::White);
		m_chessGame.setScale(0.6, 0.6);
		m_chessGame.setPosition(50, 50);
	}

	void InOfflineGameState::handleEvent(sf::Event& e)
	{
		m_chessGame.handleEvents(e);
	}

	void InOfflineGameState::update(float deltaTime)
	{
		this->m_chessGame.update(deltaTime);

		if (MouseInput::isButtonJustPressed(sf::Mouse::Button::Left))
		{
			sf::Vector2i selected = m_chessGame.getMouseHoveringCoords();

			m_chessGame.trySelectPiece(selected);
		}
		else if (MouseInput::isButtonJustReleased(sf::Mouse::Button::Left))
		{
			sf::Vector2i target = m_chessGame.getMouseHoveringCoords();

			m_chessGame.tryMoveSelectedPiece(target);
		}
	}

	void InOfflineGameState::physicsUpdate(float deltaTime)
	{
		// std::cout << "Physics Update: " << deltaTime << std::endl;
	}

	void InOfflineGameState::draw(sf::RenderTarget& target) const
	{
		target.draw(m_chessGame);
	}

	InOfflineGameState* InOfflineGameState::getInstance()
	{
		static InOfflineGameState m_inGameState;

		return &m_inGameState;
	}

	InOfflineGameState::InOfflineGameState() = default;
}