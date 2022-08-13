cmake_minimum_required(VERSION 3.21)

project(SFMLChessClient LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(TGUI
    GIT_REPOSITORY https://github.com/texus/TGUI.git
    GIT_TAG v0.9.5
)
FetchContent_MakeAvailable(TGUI)

add_executable(SFMLChessClient 
    "Client/main.cpp" 
    "Client/Scenes/Game.cpp"
    "Client/Scenes/Chess.cpp"
    
    "Client/Scenes/MoveablePiece.cpp"
    "Client/Scenes/ChessBoard.cpp" 
    "Client/Scenes/PieceMarker.cpp" 
    "Core/Utility/Logger.cpp"
    "Core/Utility/ScopedTimer.cpp" 
    "Core/Engine/TileMap.cpp"
    "Core/Engine/ResourceManager.cpp"
    "Core/Inputs/Mouse.cpp"
    "Core/Inputs/Keyboard.cpp"
    "Core/Chess/MoveGenerator.cpp"
    "Client/States/MainMenuState.cpp"
    "Client/States/InOfflineGameState.cpp"
    "Client/States/InOnlineGameState.cpp"
    "Core/Engine/GameStateManager.cpp"      
    "Client/States/RoomSelectionState.cpp" 
    "Client/States/RegisterState.cpp" 
    "Client/UI/RoomListing.cpp" 
    "Client/UI/CreateRoomForm.cpp" 
    "Client/UI/DirectConnectForm.cpp"
    "Network/ChessClient.cpp"
    "Network/ChessServer.cpp"
    "Network/ServerMessage.cpp"
    "Network/ClientMessage.cpp"

 "Client/Scenes/ChessPieces.cpp")

include_directories(${PROJECT_SOURCE_DIR})

target_link_libraries(SFMLChessClient  PRIVATE
    sfml-system
    sfml-graphics 
    sfml-audio
    sfml-network
    tgui
)

target_compile_features(SFMLChessClient PRIVATE cxx_std_20)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET SFMLChessClient POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:SFMLChessClient> 
        $<TARGET_FILE_DIR:SFMLChessClient> COMMAND_EXPAND_LISTS)
endif()

add_custom_command(
    TARGET SFMLChessClient PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/Assets
        ${CMAKE_CURRENT_BINARY_DIR}/Assets)

install(TARGETS SFMLChessClient)