#pragma once


namespace chess
{
    enum class PieceAction
    {
        None,
        Relocate,
        TwoSquaresForward,
        Capture,
        CastleLeft,
        CastleRight,
        EnPessantDown,
        EnPessantUp
    };
}