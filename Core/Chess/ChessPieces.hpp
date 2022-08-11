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
    ChessPieces()
        : m_chessPieces_TlMap(sf::Vector2i(8, 8), sf::Vector2f(80, 80))
    {
        this->m_chessPieces_Data.resize(8, std::vector<Piece>(8, Piece()));
    }

    void initialize(const PieceColor layoutColor)
    {
        std::vector<std::vector<int>> chessLayout;

        if (layoutColor == PieceColor::White)
        {
            chessLayout = {
                {10, 4, 0, 8, 2, 0, 4, 10},
                { 6, 6, 6, 6, 6, 6, 6, 6 },
                {-1,-1,-1,-1,-1,-1,-1,-1 },
                {-1,-1,-1,-1,-1,-1,-1,-1 },
                {-1,-1,-1,-1,-1,-1,-1,-1 },
                {-1,-1,-1,-1,-1,-1,-1,-1 },
                { 7, 7, 7, 7, 7, 7, 7, 7 },
                {11, 5, 1, 9, 3, 1, 5, 11}
            };
        }
        else if (layoutColor == PieceColor::Black)
        {
            chessLayout = {
                {11, 5, 1, 9, 3, 1, 5, 11},
                { 7, 7, 7, 7, 7, 7, 7, 7 },
                {-1,-1,-1,-1,-1,-1,-1,-1 },
                {-1,-1,-1,-1,-1,-1,-1,-1 },
                {-1,-1,-1,-1,-1,-1,-1,-1 },
                {-1,-1,-1,-1,-1,-1,-1,-1 },
                { 6, 6, 6, 6, 6, 6, 6, 6 },
                {10, 4, 0, 2, 8, 0, 4, 10}
            };
        }

        // start populating pieces from the layout
        for (int y = 0; y < 8; y++)
        {
            for (int x = 0; x < 8; x++)
            {
                PieceColor pieceColor;

                if (chessLayout[y][x] == -1)
                {
                    pieceColor = PieceColor::Neutral;
                }
                else if (chessLayout[y][x] % 2)
                {
                    pieceColor = PieceColor::White;
                }
                else
                {
                    pieceColor = PieceColor::Black;
                }
                
                Piece piece {
                    .coords = sf::Vector2i(x, y),
                    .type = PieceType(chessLayout[y][x]),
                    .color = pieceColor,
                };
                m_chessPieces_Data[y][x] = piece;
            }
            m_layoutColor = layoutColor;
        }

        m_tileSet = std::make_shared<egn::TileSet>();

        if (!m_tileSet->create(sf::Vector2f(80.0f, 80.0f), 12))
        {
            Logger::error("Failed to create tileset!");
        }

        m_tileSet->loadFromTexture(egn::ResourceManager::getTexture(egn::ResourceKey::WoodPieceBishopB));
        m_tileSet->loadFromTexture(egn::ResourceManager::getTexture(egn::ResourceKey::WoodPieceBishopW));
        m_tileSet->loadFromTexture(egn::ResourceManager::getTexture(egn::ResourceKey::WoodPieceQueenB));
        m_tileSet->loadFromTexture(egn::ResourceManager::getTexture(egn::ResourceKey::WoodPieceQueenW));
        m_tileSet->loadFromTexture(egn::ResourceManager::getTexture(egn::ResourceKey::WoodPieceKnightB));
        m_tileSet->loadFromTexture(egn::ResourceManager::getTexture(egn::ResourceKey::WoodPieceKnightW));
        m_tileSet->loadFromTexture(egn::ResourceManager::getTexture(egn::ResourceKey::WoodPiecePawnB));
        m_tileSet->loadFromTexture(egn::ResourceManager::getTexture(egn::ResourceKey::WoodPiecePawnW));
        m_tileSet->loadFromTexture(egn::ResourceManager::getTexture(egn::ResourceKey::WoodPieceKingB));
        m_tileSet->loadFromTexture(egn::ResourceManager::getTexture(egn::ResourceKey::WoodPieceKingW));
        m_tileSet->loadFromTexture(egn::ResourceManager::getTexture(egn::ResourceKey::WoodPieceRookB));
        m_tileSet->loadFromTexture(egn::ResourceManager::getTexture(egn::ResourceKey::WoodPieceRookW));

        m_chessPieces_TlMap.setTileSet(m_tileSet);
        m_chessPieces_TlMap.mapCellsFrom(chessLayout);
    }
    
    const void setPiece(const Piece& piece, const sf::Vector2i& coords)
    {
        m_chessPieces_TlMap.setCell(piece.type, coords);

        m_chessPieces_Data[coords.y][coords.x] = piece;
    }

    const Piece& getPiece(const sf::Vector2i& coords)
    {
        return m_chessPieces_Data[coords.y][coords.x];
    }

    void removePiece(const sf::Vector2i& coords)
    {
       m_chessPieces_TlMap.remove(coords);

       m_chessPieces_Data[coords.y][coords.x] = Piece();
    }

    const Piece& executeMovement(const Piece& selectedPiece, const PieceMovement& targetMove)
    {
        //m_moveGenerator.findValidMoveByCoords(movement.coords).action
	    switch (targetMove.action)
        {
        case PieceAction::Relocate:
            this->movePiece(selectedPiece.coords, targetMove.coords);
            break;

        case PieceAction::TwoSquaresForward:
            this->movePiece(selectedPiece.coords, targetMove.coords);
            break;

        case PieceAction::Capture:
            this->movePiece(selectedPiece.coords, targetMove.coords);
            break;
            
        case PieceAction::CastleLeft:
            this->castleLeft(selectedPiece.coords, targetMove.coords);
            break;

        case PieceAction::CastleRight:
            this->castleRight(selectedPiece.coords, targetMove.coords);
            break;

        case PieceAction::EnPessantDown:
            this->enPessantDown(selectedPiece.coords, targetMove.coords);
            break;
        
        case PieceAction::EnPessantUp:
            this->EnPessantUp(selectedPiece.coords, targetMove.coords);
            break;
        
        default:
            break;
        }
        return this->getPiece(targetMove.coords);
    }

    void movePiece(const sf::Vector2i& selected, const sf::Vector2i& target)
    {
        Piece selectedPiece = this->getPiece(selected);
        
        selectedPiece.coords = target;

        selectedPiece.isFirstMove = !selectedPiece.isEverMoved;     

        selectedPiece.isEverMoved = true;

        this->setPiece(selectedPiece, target);
        this->removePiece(selected);
    }

    void castleLeft(const sf::Vector2i& selected, const sf::Vector2i& target)
    {
        // move king to designated target coords
        this->movePiece(selected, target);

        // move rook to opposite side of the king
        this->movePiece({ 0, selected.y}, { target.x + 1, selected.y } );
    }

    void castleRight(const sf::Vector2i& selected, const sf::Vector2i& target)
    {
        // move king to designated target coords
        this->movePiece(selected, target);

        // move rook to opposite side of the king
        this->movePiece({ 7, selected.y}, { target.x - 1, selected.y } );
    }

    void EnPessantUp(const sf::Vector2i& selected, const sf::Vector2i& target)
    {
        this->movePiece(selected, target);

        // capture the pawn by en pessant
        this->movePiece(selected, { target.x, target.y + 1 });
    }

    void enPessantDown(const sf::Vector2i& selected, const sf::Vector2i& target)
    {
        this->movePiece(selected, target);

        // capture the pawn by en pessant
        this->movePiece(selected, { target.x, target.y - 1 });
    }

    bool isPieceExists(const sf::Vector2i& coords)
    {
        return m_chessPieces_Data[coords.y][coords.x].type != PieceType::None;
    }

    PieceType getType(const sf::Vector2i& coords)
    {
        return this->getPiece(coords).type;    
    }

    PieceColor getColor(const sf::Vector2i& coords)
    {
        return this->getPiece(coords).color;
    }

    PieceColor getLayoutColor()
    {
        return m_layoutColor;
    }

    egn::TileMap* getTileMap()
    {
        return &m_chessPieces_TlMap;
    }
  
private:
    std::shared_ptr<egn::TileSet> m_tileSet;

    PieceColor m_layoutColor = PieceColor::Neutral;

    std::vector<std::vector<Piece>> m_chessPieces_Data;

    egn::TileMap m_chessPieces_TlMap;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    { 
        states.transform *= getTransform();

        target.draw(m_chessPieces_TlMap, states);
    }
};