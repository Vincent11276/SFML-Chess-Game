cmake_minimum_required(VERSION 3.21)
project(CMakeSFMLProject LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x
)

FetchContent_Declare(EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.9.0
)
FetchContent_MakeAvailable(SFML EnTT)

add_executable(CMakeSFMLProject 
                "src/main.cpp" 
                "src/Utility/Logger.cpp"
                "src/Engine/TileMap.cpp"
                "src/Engine/Resource.cpp"
                "src/Engine/ResourceManager.cpp"
                "src/Engine/RTexture.cpp"
                "src/Inputs/Mouse.cpp"
                "src/Inputs/Keyboard.cpp"
                "src/Scenes/Game.cpp"
                "src/Scenes/Chess.cpp"
                "src/States/GameStateManager.cpp"
                "src/States/InGameState.cpp"
) 

target_link_libraries(CMakeSFMLProject PRIVATE sfml-graphics sfml-audio)
target_compile_features(CMakeSFMLProject PRIVATE cxx_std_20)
if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(TARGET CMakeSFMLProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:CMakeSFMLProject> $<TARGET_FILE_DIR:CMakeSFMLProject> COMMAND_EXPAND_LISTS)
endif()

install(TARGETS CMakeSFMLProject)