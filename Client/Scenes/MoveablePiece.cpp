#include "MoveablePiece.hpp"


std::unordered_map<int, ResourceKey> MoveablePiece::m_resourceKeys
{
    { PieceType::B_PAWN, ResourceKey::WoodPiecePawnB },
    { PieceType::B_ROOK, ResourceKey::WoodPieceRookB },
    { PieceType::B_KNIGHT, ResourceKey::WoodPieceKnightB },
    { PieceType::B_BISHOP, ResourceKey::WoodPieceBishopB },
    { PieceType::B_QUEEN, ResourceKey::WoodPieceQueenB },
    { PieceType::B_KING, ResourceKey::WoodPieceKingB },
    { PieceType::W_PAWN, ResourceKey::WoodPiecePawnW },
    { PieceType::W_ROOK, ResourceKey::WoodPieceRookW },
    { PieceType::W_KNIGHT, ResourceKey::WoodPieceKnightW },
    { PieceType::W_BISHOP, ResourceKey::WoodPieceBishopW },
    { PieceType::W_QUEEN, ResourceKey::WoodPieceQueenW },
    { PieceType::W_KING, ResourceKey::WoodPieceKingW },
};

MoveablePiece::MoveablePiece()
{
    // default..
}

void MoveablePiece::changeType(int pieceType)
{
    if (m_resourceKeys.find(pieceType) != m_resourceKeys.end())
    {
        moveablePiece_Spr.setTexture(
            ResourceManager::getTexture(m_resourceKeys[pieceType]));
    }
    else
    {
        Logger::error("Unknown piece type enum value received for setting moveable chess piece sprite");
    }
    Helper::centerSprite(&moveablePiece_Spr);
}

void MoveablePiece::followMouse()
{
    auto mouse = MouseInput::getRelativePosition();

    moveablePiece_Spr.setPosition(mouse.x, mouse.y);
}

void MoveablePiece::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(moveablePiece_Spr);
}
