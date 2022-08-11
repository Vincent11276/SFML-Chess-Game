#pragma once

#include <TGUI/TGUI.hpp>
#include "TGUI/Backends/SFML.hpp"
#include "Core/Engine/GameStateManager.hpp"
#include "Client/States/MainMenuState.hpp"
#include "Network/ChessClient.hpp"
#include "Client/Scenes/Game.hpp"
#include "Client/UI/RoomListing.hpp"
#include "Client/UI/CreateRoomForm.hpp"
#include "Client/UI/DirectConnectForm.hpp"
#include "InOnlineGameState.hpp"


namespace states
{
    class RoomSelectionState : public egn::GameState
    {
    public:
        void init() override;
        void cleanup() override { }
        void pause() override { }
        void resume() override { }

        void handleEvent(sf::Event& e) override;
        void update(float deltaTime) override { }
        void physicsUpdate(float deltaTime) override { }
        void draw(sf::RenderTarget& target) const override;

        static RoomSelectionState* getInstance();

        void initUI();

        void updateRoomListing(const std::vector<netw::ServerMessage::FetchedAvailableRooms::RoomListing>& listing);

        void on_directConnect_Btn_Pressed();
        void on_createRoom_Btn_Pressed();
        void on_joinRoom_Btn_Pressed();
        void on_refresh_Btn_Pressed();
        void on_cancel_Btn_Pressed();

    private:
        mutable tgui::Gui m_gui;

    protected:
        RoomSelectionState();
    };
}