#include "MoveablePiece.hpp"


std::unordered_map<int, egn::ResourceKey> MoveablePiece::m_resourceKeys
{
    { PieceType::B_PAWN, egn::ResourceKey::WoodPiecePawnB },
    { PieceType::B_ROOK, egn::ResourceKey::WoodPieceRookB },
    { PieceType::B_KNIGHT, egn::ResourceKey::WoodPieceKnightB },
    { PieceType::B_BISHOP, egn::ResourceKey::WoodPieceBishopB },
    { PieceType::B_QUEEN, egn::ResourceKey::WoodPieceQueenB },
    { PieceType::B_KING, egn::ResourceKey::WoodPieceKingB },
    { PieceType::W_PAWN, egn::ResourceKey::WoodPiecePawnW },
    { PieceType::W_ROOK, egn::ResourceKey::WoodPieceRookW },
    { PieceType::W_KNIGHT, egn::ResourceKey::WoodPieceKnightW },
    { PieceType::W_BISHOP, egn::ResourceKey::WoodPieceBishopW },
    { PieceType::W_QUEEN, egn::ResourceKey::WoodPieceQueenW },
    { PieceType::W_KING, egn::ResourceKey::WoodPieceKingW },
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
            egn::ResourceManager::getTexture(m_resourceKeys[pieceType]));
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
