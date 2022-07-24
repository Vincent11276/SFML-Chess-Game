cmake_minimum_required(VERSION 3.21)
project(CMakeSFMLProject LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
)

FetchContent_MakeAvailable(SFML)

add_executable(CMakeSFMLProject 
    "Core/Utility/Logger.cpp"
    "Core/Utility/ScopedTimer.cpp" 
    "Core/Engine/TileMap.cpp"
    "Core/Engine/ResourceManager.cpp"
    "Core/Inputs/Mouse.cpp"
    "Core/Inputs/Keyboard.cpp"

    "Client/main.cpp" 
    "Client/Scenes/Game.cpp"
    "Client/Scenes/Chess.cpp"
    "Client/Scenes/PieceHighlighter.cpp"
    "Client/Scenes/MoveablePiece.cpp"
    "Client/States/InGameState.cpp"
    "Client/States/GameStateManager.cpp"      
)

include_directories("${PROJECT_SOURCE_DIR}")

target_link_libraries(CMakeSFMLProject PRIVATE sfml-graphics sfml-audio)


target_compile_features(CMakeSFMLProject PRIVATE cxx_std_20)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET CMakeSFMLProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:CMakeSFMLProject> $<TARGET_FILE_DIR:CMakeSFMLProject> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS CMakeSFMLProject)