#pragma once

#include <iostream>
#include <TGUI/Backends/SFML.hpp>
#include <TGUI/TGUI.hpp>
#include "Network/ChessClient.hpp"
#include "Client/UI/RoomListing.hpp"
#include "Core/Engine/GameStateManager.hpp"
#include "Client/States/RegisterState.hpp"
#include "Client/States/InOfflineGameState.hpp"

namespace states
{
    class RoomSelectionState;

    class MainMenuState : public engine::GameState
    {
    public:
        void init() override;
        void cleanup() override { };
        void pause() override { };
        void resume() override { };

        void handleEvent(sf::Event& e) override;
        void update(float deltaTime) override { };
        void physicsUpdate(float deltaTime) override { };
        void draw(sf::RenderTarget& target) const override;

        static MainMenuState* getInstance();

        void initUI();

        void on_PlayBtn_Pressed();
        void on_SettingsBtn_pressed();
        void on_OnlineBtn_Pressed();
        void on_ExitBtn_Pressed();

    protected:
        MainMenuState();

    private:
        mutable tgui::Gui gui;
    };
}