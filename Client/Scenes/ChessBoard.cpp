#include "ChessBoard.hpp"


ChessBoard::ChessBoard()
{
	m_board_Spr.setTexture(engine::ResourceManager::getTexture(engine::ResourceKey::WoodChessBoard));
}

void ChessBoard::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_board_Spr, states);
}
