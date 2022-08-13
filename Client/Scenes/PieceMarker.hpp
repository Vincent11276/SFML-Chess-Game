#pragma once

#include <SFML/Graphics.hpp>

#include "Core/Engine/TileMap.hpp"
#include "Core/Engine/ResourceManager.hpp"
#include "Core/Chess/PieceMovement.hpp"


class PieceMarker : public sf::Drawable, public sf::Transformable
{
public:
    PieceMarker();
    
    void init();

    void setSelectedPiece(const sf::Vector2i& coords);
    void setValidMoves(const std::vector<chess::PieceMovement>& validMoves);
    void setHoveringTile(const sf::Vector2i& coords);
    void setPreviousMove(const sf::Vector2i& selected, const sf::Vector2i& target);

    void resetSelectedPiece();
    void resetValidMoves();
    void resetHoveringTile();
    void resetPreviousMove();

private:
    sf::Vector2i m_selectedCoords;
    std::pair<sf::Vector2i, sf::Vector2i> m_prevMove;
    std::shared_ptr<engine::TileSet> m_tileset;

	engine::TileMap m_validMoves_TileMap;
    engine::TileMap m_highlights_TileMap;
    engine::TileMap m_hoveringTile_TileMap;

    static constexpr uint8_t HOVERING_MARK  = 0x00;
    static constexpr uint8_t ACTION_MARK    = 0x01;
    static constexpr uint8_t CAN_MOVE_MARK  = 0x02;
    static constexpr uint8_t CAN_TAKE_MARK  = 0x03;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
