#include "MoveablePiece.hpp"


std::unordered_map<int, engine::ResourceKey> MoveablePiece::m_resourceKeys
{
    { chess::PieceType::B_PAWN, engine::ResourceKey::WoodPiecePawnB },
    { chess::PieceType::B_ROOK, engine::ResourceKey::WoodPieceRookB },
    { chess::PieceType::B_KNIGHT, engine::ResourceKey::WoodPieceKnightB },
    { chess::PieceType::B_BISHOP, engine::ResourceKey::WoodPieceBishopB },
    { chess::PieceType::B_QUEEN, engine::ResourceKey::WoodPieceQueenB },
    { chess::PieceType::B_KING, engine::ResourceKey::WoodPieceKingB },
    { chess::PieceType::W_PAWN, engine::ResourceKey::WoodPiecePawnW },
    { chess::PieceType::W_ROOK, engine::ResourceKey::WoodPieceRookW },
    { chess::PieceType::W_KNIGHT, engine::ResourceKey::WoodPieceKnightW },
    { chess::PieceType::W_BISHOP, engine::ResourceKey::WoodPieceBishopW },
    { chess::PieceType::W_QUEEN, engine::ResourceKey::WoodPieceQueenW },
    { chess::PieceType::W_KING, engine::ResourceKey::WoodPieceKingW },
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
            engine::ResourceManager::getTexture(m_resourceKeys[pieceType]));
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
