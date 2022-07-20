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
    ChessPieces(const PieceColor playerSide)
    {
        this->initialize(playerSide);
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

        this->setPiece(selectedPiece, target);

        this->removePiece(selected);
    }

private:
    std::vector<std::vector<Piece>> m_chessPieces_Data;

    TileMap m_chessPieces_TlMap;

    void initialize(const PieceColor playerSide)
    {
        std::vector<std::vector<int>> chessLayout;

        if (playerSide == PieceColor::White)
        {
            chessLayout = {
                {10, 4, 0, 2, 8, 0, 4, 10},
                { 6, 6, 6, 6, 6, 6, 6, 6 },
                {-1,-1,-1,-1,-1,-1,-1,-1 },
                {-1,-1,-1,-1,-1,-1,-1,-1 },
                {-1,-1,-1,-1,-1,-1,-1,-1 },
                {-1,-1,-1,-1,-1,-1,-1,-1 },
                { 7, 7, 7, 7, 7, 7, 7, 7 },
                {11, 5, 1, 3, 9, 1, 5, 11}
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
                {10, 4, 0, 8, 2, 0, 4, 10}
            };
        }

        // start populating pieces from the layout
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; i++)
            {
                m_chessPieces_Data[j][i] = { 
                    PieceType(chessLayout[j][i]) 
                };
            }
        }

        static TileSet tileSet;

        if (!tileSet.create(sf::Vector2f(80.0f, 80.0f), 12))
        {
            Logger::getInstance().log(LogLevel::ERROR, "Failed to create tileset!");
        }

        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceBishopB));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceBishopW));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceKingB));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceKingW));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceKnightB));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceKnightW));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPiecePawnB));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPiecePawnW));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceQueenB));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceQueenW));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceRookB));
        tileSet.loadFromTexture(ResourceManager::getTexture(ResourceKey::WoodPieceRookW));

        m_chessPieces_TlMap.setTileSet(tileSet);

        m_chessPieces_TlMap.mapCellsFrom(chessLayout);
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();

        target.draw(m_chessPieces_TlMap);
    }
};