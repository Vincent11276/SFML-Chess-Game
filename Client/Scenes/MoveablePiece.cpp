#include "MoveablePiece.hpp"


std::unordered_map<int, egn::ResourceKey> MoveablePiece::m_resourceKeys
{
    { chess::PieceType::B_PAWN, egn::ResourceKey::WoodPiecePawnB },
    { chess::PieceType::B_ROOK, egn::ResourceKey::WoodPieceRookB },
    { chess::PieceType::B_KNIGHT, egn::ResourceKey::WoodPieceKnightB },
    { chess::PieceType::B_BISHOP, egn::ResourceKey::WoodPieceBishopB },
    { chess::PieceType::B_QUEEN, egn::ResourceKey::WoodPieceQueenB },
    { chess::PieceType::B_KING, egn::ResourceKey::WoodPieceKingB },
    { chess::PieceType::W_PAWN, egn::ResourceKey::WoodPiecePawnW },
    { chess::PieceType::W_ROOK, egn::ResourceKey::WoodPieceRookW },
    { chess::PieceType::W_KNIGHT, egn::ResourceKey::WoodPieceKnightW },
    { chess::PieceType::W_BISHOP, egn::ResourceKey::WoodPieceBishopW },
    { chess::PieceType::W_QUEEN, egn::ResourceKey::WoodPieceQueenW },
    { chess::PieceType::W_KING, egn::ResourceKey::WoodPieceKingW },
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
