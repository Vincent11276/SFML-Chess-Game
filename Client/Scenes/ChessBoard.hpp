#pragma once

#include <SFML/Graphics.hpp>
#include "Core/Engine/ResourceManager.hpp"


class ChessBoard : public sf::Transformable, public sf::Drawable
{
public:
	ChessBoard();

private:
	sf::Sprite m_board_Spr;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};