#include "ChessPieces.hpp"


ChessPieces::ChessPieces()
    : m_chessPieces_TlMap(sf::Vector2i(8, 8), sf::Vector2f(80, 80))
{
    this->m_chessPieces_Data.resize(8, std::vector<chess::Piece>(8, chess::Piece()));
}

void ChessPieces::initialize(const chess::PieceColor layoutColor)
{
    std::vector<std::vector<int>> chessLayout;

    if (layoutColor == chess::PieceColor::White)
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
        m_kingCoords = std::make_pair<sf::Vector2i, sf::Vector2i>({ 3, 7 }, { 3, 0 });
    }
    else if (layoutColor == chess::PieceColor::Black)
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
        m_kingCoords = std::make_pair<sf::Vector2i, sf::Vector2i>({ 3, 0 }, { 3, 7 });
    }

    // start populating pieces from the layout
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            chess::PieceColor pieceColor;

            if (chessLayout[y][x] == -1)
            {
                pieceColor = chess::PieceColor::Neutral;
            }
            else if (chessLayout[y][x] % 2)
            {
                pieceColor = chess::PieceColor::White;
            }
            else
            {
                pieceColor = chess::PieceColor::Black;
            }

            chess::Piece piece{
                .coords = sf::Vector2i(x, y),
                .type = chess::PieceType(chessLayout[y][x]),
                .color = pieceColor,
            };
            m_chessPieces_Data[y][x] = piece;
        }
        m_layoutColor = layoutColor;
    }

    m_tileSet = std::make_shared<engine::TileSet>();

    if (!m_tileSet->create(sf::Vector2f(80.0f, 80.0f), 12))
    {
        Logger::error("Failed to create tileset!");
    }

    m_tileSet->loadFromTexture(engine::ResourceManager::getTexture(engine::ResourceKey::WoodPieceBishopB));
    m_tileSet->loadFromTexture(engine::ResourceManager::getTexture(engine::ResourceKey::WoodPieceBishopW));
    m_tileSet->loadFromTexture(engine::ResourceManager::getTexture(engine::ResourceKey::WoodPieceQueenB));
    m_tileSet->loadFromTexture(engine::ResourceManager::getTexture(engine::ResourceKey::WoodPieceQueenW));
    m_tileSet->loadFromTexture(engine::ResourceManager::getTexture(engine::ResourceKey::WoodPieceKnightB));
    m_tileSet->loadFromTexture(engine::ResourceManager::getTexture(engine::ResourceKey::WoodPieceKnightW));
    m_tileSet->loadFromTexture(engine::ResourceManager::getTexture(engine::ResourceKey::WoodPiecePawnB));
    m_tileSet->loadFromTexture(engine::ResourceManager::getTexture(engine::ResourceKey::WoodPiecePawnW));
    m_tileSet->loadFromTexture(engine::ResourceManager::getTexture(engine::ResourceKey::WoodPieceKingB));
    m_tileSet->loadFromTexture(engine::ResourceManager::getTexture(engine::ResourceKey::WoodPieceKingW));
    m_tileSet->loadFromTexture(engine::ResourceManager::getTexture(engine::ResourceKey::WoodPieceRookB));
    m_tileSet->loadFromTexture(engine::ResourceManager::getTexture(engine::ResourceKey::WoodPieceRookW));

    m_chessPieces_TlMap.setTileSet(m_tileSet);
    m_chessPieces_TlMap.mapCellsFrom(chessLayout);
}

const void ChessPieces::setPiece(const chess::Piece& piece, const sf::Vector2i& coords)
{
    m_chessPieces_TlMap.setCell(piece.type, coords);

    m_chessPieces_Data[coords.y][coords.x] = piece;
}

const chess::Piece& ChessPieces::getPiece(const sf::Vector2i& coords)
{
    return m_chessPieces_Data[coords.y][coords.x];
}

void ChessPieces::removePiece(const sf::Vector2i& coords)
{
    m_chessPieces_TlMap.remove(coords);

    m_chessPieces_Data[coords.y][coords.x] = chess::Piece();
}

const chess::Piece& ChessPieces::executeMovement(const chess::Piece& selectedPiece, const chess::PieceMovement& targetMove)
{
    switch (targetMove.action)
    {
    case chess::PieceAction::Relocate:
        this->movePiece(selectedPiece.coords, targetMove.coords);
        break;

    case chess::PieceAction::TwoSquaresForward:
        this->movePiece(selectedPiece.coords, targetMove.coords);
        break;

    case chess::PieceAction::Capture:
        this->movePiece(selectedPiece.coords, targetMove.coords);
        break;

    case chess::PieceAction::CastleLeft:
        this->castleLeft(selectedPiece.coords, targetMove.coords);
        break;

    case chess::PieceAction::CastleRight:
        this->castleRight(selectedPiece.coords, targetMove.coords);
        break;

    case chess::PieceAction::EnPessantDown:
        this->enPessantDown(selectedPiece.coords, targetMove.coords);
        break;

    case chess::PieceAction::EnPessantUp:
        this->EnPessantUp(selectedPiece.coords, targetMove.coords);
        break;

    default:
        break;
    }
    return this->getPiece(targetMove.coords);
}

void ChessPieces::movePiece(const sf::Vector2i& selected, const sf::Vector2i& target)
{
    chess::Piece selectedPiece = this->getPiece(selected);

    selectedPiece.coords = target;

    selectedPiece.isFirstMove = !selectedPiece.isEverMoved;

    selectedPiece.isEverMoved = true;

    this->setPiece(selectedPiece, target);
    this->removePiece(selected);

    // Update king coords
    if (selectedPiece.type == chess::PieceType::W_KING)
    {
        m_kingCoords.first = target;
    }
    else if (selectedPiece.type == chess::PieceType::B_KING)
    {
        m_kingCoords.second = target;
    }
}

void ChessPieces::castleLeft(const sf::Vector2i& selected, const sf::Vector2i& target)
{
    // move king to designated target coords
    this->movePiece(selected, target);

    // move rook to opposite side of the king
    this->movePiece({ 0, selected.y }, { target.x + 1, selected.y });
}

void ChessPieces::castleRight(const sf::Vector2i& selected, const sf::Vector2i& target)
{
    // move king to designated target coords
    this->movePiece(selected, target);

    // move rook to opposite side of the king
    this->movePiece({ 7, selected.y }, { target.x - 1, selected.y });
}

void ChessPieces::EnPessantUp(const sf::Vector2i& selected, const sf::Vector2i& target)
{
    this->movePiece(selected, target);

    // capture the pawn by en pessant
    this->movePiece(selected, { target.x, target.y + 1 });
}

void ChessPieces::enPessantDown(const sf::Vector2i& selected, const sf::Vector2i& target)
{
    this->movePiece(selected, target);

    // capture the pawn by en pessant
    this->movePiece(selected, { target.x, target.y - 1 });
}

bool ChessPieces::isPieceExists(const sf::Vector2i& coords)
{
    return m_chessPieces_Data[coords.y][coords.x].type != chess::PieceType::None;
}

chess::PieceType ChessPieces::getType(const sf::Vector2i& coords)
{
    return this->getPiece(coords).type;
}

chess::PieceColor ChessPieces::getColor(const sf::Vector2i& coords)
{
    return this->getPiece(coords).color;
}

chess::PieceColor ChessPieces::getLayoutColor()
{
    return m_layoutColor;
}

engine::TileMap* ChessPieces::getTileMap()
{
    return &m_chessPieces_TlMap;
}

sf::Vector2i ChessPieces::getWhiteKingCoords()
{
    return m_kingCoords.first;
}

sf::Vector2i ChessPieces::getBlackKingCoords()
{
    return m_kingCoords.second;
}

void ChessPieces::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_chessPieces_TlMap, states);
}

