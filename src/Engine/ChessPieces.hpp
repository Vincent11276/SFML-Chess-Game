#pragma once

#include <SFML/Graphics.hpp>

#include "../Engine/PieceColor.hpp"
#include "../Engine/PieceType.hpp"
#include "../Engine/ResourceManager.hpp"
#include "../Engine/TileMap.hpp"
#include "../Engine/Piece.hpp"
#include "../Utility/Logger.hpp"



class ChessPieces : public sf::Drawable, public sf::Transformable
{
public:
    ChessPieces()
        : m_chessPieces_TlMap(sf::Vector2i(8, 8), sf::Vector2f(80, 80))
    {
        this->m_chessPieces_Data.resize(8, std::vector<Piece>(8, Piece()));
    }

    void initialize(const PieceColor playerSide)
    {
        std::vector<std::vector<int>> chessLayout;

        if (playerSide == PieceColor::White)
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
        else if (playerSide == PieceColor::Black)
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
        }

        static TileSet tileSet;

        if (!tileSet.create(sf::Vector2f(80.0f, 80.0f), 12))
        {
            Logger::getInstance().log(LogLevel::ERROR, "Failed to create tileset!");
        }

        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceBishopB));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceBishopW));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceQueenB));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceQueenW));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceKnightB));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceKnightW));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPiecePawnB));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPiecePawnW));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceKingB));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceKingW));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceRookB));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceRookW));

        m_chessPieces_TlMap.setTileSet(tileSet);

        m_chessPieces_TlMap.mapCellsFrom(chessLayout);
    }

    const void setPiece(Piece piece, const sf::Vector2i& coords)
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

    void movePiece(const sf::Vector2i& selected, const sf::Vector2i& target)
    {
        Piece selectedPiece = this->getPiece(selected);
        
        selectedPiece.isEverMoved = true;
        selectedPiece.coords = target;

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
  
private:
    std::vector<std::vector<Piece>> m_chessPieces_Data;

    TileMap m_chessPieces_TlMap;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();

        target.draw(m_chessPieces_TlMap);
    }
};