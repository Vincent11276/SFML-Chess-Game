#pragma once

#include <SFML/Graphics.hpp>

#include "Core/Chess/PieceColor.hpp"
#include "Core/Chess/PieceType.hpp"
#include "Core/Chess/Piece.hpp"
#include "Core/Chess/PieceAction.hpp"
#include "Core/Chess/PieceMovement.hpp"

#include "Core/Engine/ResourceManager.hpp"
#include "Core/Engine/TileMap.hpp"

#include "Core/Utility/Logger.hpp"
#include <iostream>

class ChessPieces : public sf::Drawable, public sf::Transformable
{
public:
    ChessPieces();
    void initialize(const chess::PieceColor layoutColor);

    const void setPiece(const chess::Piece& piece, const sf::Vector2i& coords);
    const chess::Piece& getPiece(const sf::Vector2i& coords);
    void removePiece(const sf::Vector2i& coords);
    const chess::Piece& executeMovement(const chess::Piece& selectedPiece, const chess::PieceMovement& targetMove);

    void movePiece(const sf::Vector2i& selected, const sf::Vector2i& target);
    void castleLeft(const sf::Vector2i& selected, const sf::Vector2i& target);
    void castleRight(const sf::Vector2i& selected, const sf::Vector2i& target);
    void EnPessantUp(const sf::Vector2i& selected, const sf::Vector2i& target);
    void enPessantDown(const sf::Vector2i& selected, const sf::Vector2i& target);

    bool isPieceExists(const sf::Vector2i& coords);

    chess::PieceType getType(const sf::Vector2i& coords);
    chess::PieceColor getColor(const sf::Vector2i& coords);
    chess::PieceColor getLayoutColor();
    sf::Vector2i getWhiteKingCoords();
    sf::Vector2i getBlackKingCoords();
    engine::TileMap* getTileMap();

private:
    std::pair<sf::Vector2i, sf::Vector2i> m_kingCoords;
    chess::PieceColor m_layoutColor = chess::PieceColor::Neutral;
    std::vector<std::vector<chess::Piece>> m_chessPieces_Data;

    std::shared_ptr<engine::TileSet> m_tileSet;
    engine::TileMap m_chessPieces_TlMap;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};
